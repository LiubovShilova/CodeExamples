#!/usr/bin/env python3

################################################################################################################
#Author: Liubov Shilova
#Date: 05.02.2022
#Task: Classify an ultrasound video of the heart as either Hypertrophic Cardiomyopathy (HCM) or Cardio Amyloidosis (CA)
#Approach: Training a GRU network on features extracted with a pretrained network (Xception, NasNetMobile ...) from each frame of a video 
#Input data structure: two datasets (taken from youtube and Homburg hospital) have following structure of a directory:
#                CA
#               /
#          train          
#        /      \
#       /        HCM
# directory
#       \      CA
#        \    /
#         test
#             \
#              HCM
#    
#Output: Plots for training the models and .txt file with test accuracies.
###############################################################################################################

import os
import warnings
import numpy as np
import pandas as pd
from tensorflow import keras
import tensorflow as tf
from PIL import Image
import cv2
from matplotlib import pyplot as plt
import vidaug.augmentors as va
from keras import backend as K

from sklearn.model_selection import GroupShuffleSplit
from sklearn.metrics import confusion_matrix

import random
import re

random.seed(42)
np.random.seed(42)
tf.random.set_seed(42)

############################ Hyperparameters #################################################################

EPOCHS = 60
DROPOUT_RATE = 0.3
VALIDATION_SPLIT = 0.2
IMG_SIZE = 224
#BATCH_SIZE = 6


#maximum length of an input video
MAX_SEQ_LENGTH = 70

res_dict = {}

#choosing the datasets used for training and testing. 
youtube = True
main_ds = True
echonet_ds = False

############################ Creating the dataframe with file paths and tags ###################################

def delete_version_number(filepath):
  """
  Identify patient ID from a file path.  
  """
  filename = filepath.split('/')[-1]
  no_extantion = filename.split('.')[0]
  if re.search("_\d+", no_extantion) is not None:
    return no_extantion[:-2]
  else: 
    return no_extantion 



train_rooms = []
test_rooms = []

#if we intend to use the youtube videos
if youtube == True:

  train_path1 = './youtube/train/'
  test_path1 = './youtube/test/'
  
  if os.listdir(train_path1)!=os.listdir(test_path1):
    warnings.warn("Different structure of train and test folders!")

  labels1 = os.listdir(train_path1)
  for class_name in labels1:
    all_train_rooms = os.listdir(train_path1 + class_name)
    for room in all_train_rooms:
      train_rooms.append((class_name, str(train_path1 + class_name) + '/' + room))
    all_test_rooms = os.listdir(test_path1 + class_name)
    for room in all_test_rooms:
      test_rooms.append((class_name, str(test_path1 + class_name) + '/' + room))

#if we intend to use the videos from Homburg hospital
if main_ds == True:
  train_path2 = './train/'
  test_path2 = './test/'

  if os.listdir(train_path2)!=os.listdir(test_path2):  
    warnings.warn("Different structure of train and test folders!")

  labels2 = os.listdir(train_path2)
  for class_name in labels2:
    all_train_rooms = os.listdir(train_path2 + class_name)
    for room in all_train_rooms:
      train_rooms.append((class_name, str(train_path2 + class_name) + '/' + room))
    all_test_rooms = os.listdir(test_path2 + class_name)
    for room in all_test_rooms:
      test_rooms.append((class_name, str(test_path2 + class_name) + '/' + room))


#if we intend to use the videos from Echonet
if echonet_ds == True:
  train_path3 = './echonet/train/'
  test_path3 = './echonet/test/'

  if os.listdir(train_path3)!=os.listdir(test_path3):
    warnings.warn("Different structure of train and test folders!")

  labels3 = os.listdir(train_path3)
  for class_name in labels3:
    all_train_rooms = os.listdir(train_path3 + class_name)
    for room in all_train_rooms:
      train_rooms.append((class_name, str(train_path3 + class_name) + '/' + room))
    all_test_rooms = os.listdir(test_path3 + class_name)
    for room in all_test_rooms:
      test_rooms.append((class_name, str(test_path3 + class_name) + '/' + room))      

#check if same classes in datasets used
if youtube and main_ds:
  if labels1 != labels2:
    warning_message = f"The class faulder structure is different in the 2 datasets! {labels1}, in the other {labels2} and {labels3}"
    warnings.warn(warning_message)

#create a dataframe with paths to videos and their labels
train_df = pd.DataFrame(data=train_rooms, columns=["tag", "video_name"])
test_df = pd.DataFrame(data=test_rooms, columns=["tag", "video_name"])



train_df["Patient_ID"] = train_df["video_name"].apply(delete_version_number)


splitter = GroupShuffleSplit(test_size=0.2, n_splits=1, random_state = 42)
split = splitter.split(train_df, groups=train_df['Patient_ID'])
train_inds, val_inds = next(split)

train_df_small = train_df.iloc[train_inds].reset_index(drop=True)
val_df = train_df.iloc[val_inds].reset_index(drop=True)


#shuffle train df
train_df_small = train_df_small.sample(frac=1, random_state=42).reset_index(drop=True)

train_df_small.to_csv("training dataset")
val_df.to_csv("validating dataset")
test_df.to_csv("testing dataset")



#train_df_small, val_df = sklearn.model_selection.train_test_split(train_df, train_size = 0.8, random_state=42, stratify=train_df["tag"])
print(f"Training set\n{train_df_small.tag.value_counts()}")
print(f"Validation set\n{val_df.tag.value_counts()}")

######################################## GPUs #######################################################################

#check if GPUs are available
gpus = tf.config.experimental.list_physical_devices("GPU")

if gpus:
  try:
    tf.config.experimental.set_virtual_device_configuration(
        gpus[0],[tf.config.experimental.VirtualDeviceConfiguration(memory_limit=5120)])
  except RuntimeError as e:
    print(e)

label_processor = keras.layers.StringLookup(num_oov_indices=0, vocabulary=np.unique(train_df["tag"]))
labels = train_df["tag"].values
labels = label_processor(labels[..., None]).numpy()


######################################## Image Preprocessing ########################################################

#Augmentation using vidaug library
#sometimes = lambda aug: va.Sometimes(0.2, aug) 
seq_aug = va.Sequential([ 
    #sometimes(va.RandomCrop(size=(200, 200))),
    va.RandomRotate(degrees=30),
    #sometimes(va.GaussianBlur(0.01))
])


def make_square(img):
    """
    Make an image square by adding black pixels on top and bottom
    """
    s = max([img.shape[0], img.shape[1]])
    res = np.zeros((s,s,img.shape[2]), np.uint8)
    ax,ay = (s - img.shape[1])//2,(s - img.shape[0])//2
    res[ay:img.shape[0]+ay,ax:ax+img.shape[1],:] = img
    
    return res

def make_square_aug(img):
    """
    Make an image square by adding black pixels on top and bottom
    """
    s = max([img.shape[0], img.shape[1]])
    
    max_shift_x = (s - img.shape[1])//4
    max_shift_y = (s - img.shape[0])//4
    
    shifts_x = random.randint(0, max_shift_x)
    shifts_y = random.randint(0, max_shift_y)
    
    
    res = np.zeros((s, s, img.shape[2]), np.uint8)
    ax, ay = (s - img.shape[1])//2 + shifts_x,(s - img.shape[0])//2 + shifts_y
    
    res[ay:img.shape[0]+ay, ax:ax+img.shape[1], :] = img
    plt.imshow(res[:,:,0], cmap='gray')
    plt.colorbar()
    
    return res    

def load_video(path, max_frames=0, resize=(IMG_SIZE, IMG_SIZE), train = False):
    """
    load video from path and bring it to the needed shape: scale values to [0, 255], 0-padding to make it square, resize, copy into 3 channels.

    """

    print("Loading video ", path)

    cap = np.load(path, allow_pickle=True)

    cap = cv2.normalize(cap, None, alpha = 0, beta = 255, norm_type = cv2.NORM_MINMAX, dtype = cv2.CV_32F)

    #z-scoring
    #cap = (cap - np.mean(cap))/np.std(cap)

    if train:
      cap = make_square_aug(cap)

    else:
      cap = make_square(cap)  

    frames = []

    for i in range(cap.shape[2]):
      #make a frame
      frame = cap[:,:,i]

      if (i>0) and (frame == cap[:,:, i-1]).all():
        print("The same frame!!!")

      #resize to the square needed for transfered learning model
      frame = cv2.resize(frame, resize)

      #copy black and white channel in all 3 RGB channels
      frame = np.repeat(frame[..., np.newaxis], 3, -1)

      #append the preprocessed frame
      frames.append(frame)

    return np.array(frames)


############################## Building and training the feature extracting model and RNN ########################

def recall(y_true, y_pred):
    TP = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
    Positives = K.sum(K.round(K.clip(y_true, 0, 1)))
    
    recall = TP / (Positives+K.epsilon())    
    return recall 


def precision(y_true, y_pred):
    TP = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
    Pred_Positives = K.sum(K.round(K.clip(y_pred, 0, 1)))

    precision = TP / (Pred_Positives+K.epsilon())
    return precision

def f1(y_true, y_pred):       
    prec, rec = precision(y_true, y_pred), recall(y_true, y_pred)
    return 2*((prec*rec)/(prec+rec+K.epsilon()))

def build_feature_extractor(mode, pooling = "avg"):
    """
    Build a model to extract features from each frame by loading the preprocessed model from keras.
    """

    if mode == "Xception":
        feature_extractor = keras.applications.Xception(
            weights="imagenet",
            include_top=False,
            pooling=pooling,
            input_shape=(IMG_SIZE, IMG_SIZE, 3)#,
            )

        preprocess_input = keras.applications.xception.preprocess_input

    if mode == "InceptionV3":
        feature_extractor = keras.applications.InceptionV3(
            weights="imagenet",
            include_top=False,
            pooling=pooling,
            input_shape=(IMG_SIZE, IMG_SIZE, 3)
            )

        preprocess_input = keras.applications.inception_v3.preprocess_input

    if mode == "ResNet101":
        feature_extractor = keras.applications.ResNet101(
            weights="imagenet",
            include_top=False,
            pooling=pooling,
            input_shape=(IMG_SIZE, IMG_SIZE, 3)
            )

        preprocess_input = keras.applications.resnet.preprocess_input

    if mode == "ResNet101V2":
        feature_extractor = keras.applications.ResNet101(
            weights="imagenet",
            include_top=False,
            pooling=pooling,
            input_shape=(IMG_SIZE, IMG_SIZE, 3)
            )
        preprocess_input = keras.applications.resnet.preprocess_input

    if mode == "ResNet152V2":
        feature_extractor = keras.applications.ResNet152V2(
            weights="imagenet",
            include_top=False,
            pooling=pooling,
            input_shape=(IMG_SIZE, IMG_SIZE, 3)
            )
        preprocess_input = keras.applications.resnet_v2.preprocess_input    

    if mode == "DenseNet201":
        feature_extractor = keras.applications.DenseNet201(
            weights="imagenet",
            include_top=False,
            pooling=pooling,
            input_shape=(IMG_SIZE, IMG_SIZE, 3)
            )
    preprocess_input = keras.applications.densenet.preprocess_input 

    if mode == "MobileNetV2":
        feature_extractor = keras.applications.MobileNetV2(
            weights="imagenet",
            include_top=False,
            pooling=pooling,
            input_shape=(IMG_SIZE, IMG_SIZE, 3)
            )
    preprocess_input = keras.applications.mobilenet_v2.preprocess_input

    if mode == "NASNetMobile":
        feature_extractor = keras.applications.NASNetMobile(
            weights="imagenet",
            include_top=False,
            pooling=pooling,
            input_shape=(IMG_SIZE, IMG_SIZE, 3)
            )
        preprocess_input = keras.applications.nasnet.preprocess_input          

    inputs = keras.Input((IMG_SIZE, IMG_SIZE, 3))
    preprocessed = preprocess_input(inputs)
    outputs = feature_extractor(preprocessed)

    return keras.Model(inputs, outputs, name="feature_extractor")


def extract_features_from_frames(frames):
    """
    Extract features from given frames using a pretrained model.
    """

    frame_mask = np.zeros(shape=(1, MAX_SEQ_LENGTH,), dtype="bool") 
    frame_features = np.zeros(
        shape=(1, MAX_SEQ_LENGTH, NUM_FEATURES), dtype="float32"
    )

    for i, batch in enumerate(frames):
        video_length = batch.shape[0]
        length = min(MAX_SEQ_LENGTH, video_length)
        for j in range(length):
            frame_features[i, j, :] = feature_extractor.predict(
                batch[None, j, :], verbose = 0
            )
        frame_mask[i, :length] = 1  # 1 = not masked, 0 = masked

    return (frame_features, frame_mask)  

def extract_features_from_path(path):
    """
    Extract features from video with given path using a pretrained model.
    """
  
    frames = load_video(path)

    frames = frames[None, ...]
    frame_features, frame_mask = extract_features_from_frames(frames)

    return (frame_features, frame_mask)#!!!


def prepare_all_videos(df, train = True):
    """
    Take the videos paths from the given dataframe, load them and preprocess them, extract features.
    Masking: masking all empty frames at the end.
    TODO: look into masking, try to "skip" frames or remove the masking all together to be able to try biderectional layer later in RNN.
    """

    num_samples = len(df)
    video_paths = df["video_name"].values.tolist()
    labels = pd.get_dummies(df["tag"])
    frame_masks = np.zeros(shape=(num_samples, MAX_SEQ_LENGTH), dtype="bool") # 20,60
    frame_features = np.zeros(shape=(num_samples, MAX_SEQ_LENGTH, NUM_FEATURES), dtype="float32") #20,60,2048


    
    for idx, path in enumerate(video_paths):
        
        single_video_features, single_video_mask = extract_features_from_path(path)

        frame_features[idx,] = single_video_features.squeeze()
        frame_masks[idx,] = single_video_mask.squeeze()


    if train:
        aug_frame_masks1 = np.zeros(shape=(num_samples, MAX_SEQ_LENGTH), dtype="bool") # 20,60
        aug_frame_features1 = np.zeros(shape=(num_samples, MAX_SEQ_LENGTH, NUM_FEATURES), dtype="float32") #20,60,2048
        for idx, path in enumerate(video_paths):
            frames = load_video(path, train = True)#False)
            print("Augmentation takes place!")
            frames = np.array(seq_aug(frames))
            frames = frames[None, ...]
            aug_single_video_features, aug_single_video_mask = extract_features_from_frames(frames)
            aug_frame_features1[idx,] = aug_single_video_features.squeeze()
            aug_frame_masks1[idx,] = aug_single_video_mask.squeeze()  

        print("Size of the train dataset without augmentation:", frame_features.shape, frame_masks.shape)   
        frame_features = np.concatenate([frame_features, aug_frame_features1])
        frame_masks = np.concatenate([frame_masks, aug_frame_masks1])
        print("Size of the train dataset with augmentation:", frame_features.shape, frame_masks.shape)

        labels = np.concatenate([labels, labels])
        print(labels.shape)

        
    return (frame_features, frame_masks), labels


def scheduler(epoch, lr):
  """
  Scheduler for the learning rate.
  """
  if epoch < 10:
    return lr
  else:
    return lr * tf.math.exp(-0.1)


def get_sequence_model(drop = DROPOUT_RATE):
    """
    Build RNN with a dropout layer.
    """
    class_vocab = label_processor.get_vocabulary()

    frame_features_input = keras.Input((MAX_SEQ_LENGTH, NUM_FEATURES))
    mask_input = keras.Input((MAX_SEQ_LENGTH,), dtype="bool")

    x = keras.layers.GRU(20, return_sequences=True)(frame_features_input, mask=mask_input)
    x = keras.layers.GRU(10)(x)
    x = keras.layers.Dropout(drop, seed = 42)(x)
    x = keras.layers.Dense(10, activation="relu")(x)
    #x = keras.layers.Dense(15, activation="relu")(x)
    output = keras.layers.Dense(len(class_vocab), activation="softmax")(x)

    rnn_model = keras.Model([frame_features_input, mask_input], output)

    rnn_model.compile(
        loss="categorical_crossentropy", optimizer="adam", metrics=[f1, precision, recall]
    )
    return rnn_model


def run_experiment(model_type, class_weight):
    """
    Train RNN on features extracted from frames. Plot the process of training for each model type.

    """

    filepath = "./tmp/video_classifier"
    checkpoint = keras.callbacks.ModelCheckpoint(
        filepath, monitor='val_loss', save_best_only=True, verbose=1
    )

    callback = tf.keras.callbacks.LearningRateScheduler(scheduler)


    seq_model = get_sequence_model()
    history = seq_model.fit(
        [train_small_data[0], train_small_data[1]],
        train_small_labels,
        validation_data=[[val_data[0], val_data[1]], val_labels],
        epochs=EPOCHS,
        callbacks=[checkpoint, callback],
        class_weight = class_weight
    )

    seq_model.load_weights(filepath)
    _, f1, prec, rec = seq_model.evaluate([test_data[0], test_data[1]], test_labels)

    print(pd.DataFrame(history.history))
    hist_df = pd.DataFrame(history.history)[['loss', 'val_loss','precision','f1','val_f1']]

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.plot(hist_df)
    ax.set_title(model_type + ' with test f1 '+ str(round(f1 * 100, 2)))
    ax.set_xlabel('epoch')
    ax.set_ylim(ymin=0)
    ax.legend(hist_df.columns, loc = 'upper left')
    fig.savefig(f'Metrics for {model_type}')

    #print("Test accuracy: ", accuracy)
    global res_dict
    res_dict[model_type] = round(f1 * 100, 2)

    y_pred = tf.argmax(seq_model.predict([test_data[0], test_data[1]]), axis = 1)
    y_true = tf.argmax(test_labels, axis = 1)

    with open('results.txt', 'a') as f:
        print(f'{model_type} with {EPOCHS} epochs shows confusion matrix:\n pred CA  pred HCM \n {confusion_matrix(y_true, y_pred)}', file = f)

    return history, seq_model

##FUNCTIONS TO SEE THE OUTPUT DOR A SINGLE VIDEO
def prepare_single_video(frames):
    """
    Prepare the video
    """
    frames = frames[None, ...]
    frame_mask = np.zeros(shape=(1, MAX_SEQ_LENGTH,), dtype="bool")
    frame_features = np.zeros(shape=(1, MAX_SEQ_LENGTH, NUM_FEATURES), dtype="float32")

    for i, batch in enumerate(frames):
        video_length = batch.shape[0]
        length = min(MAX_SEQ_LENGTH, video_length)
        for j in range(length):
            frame_features[i, j, :] = feature_extractor.predict(batch[None, j, :])
        frame_mask[i, :length] = 1  # 1 = not masked, 0 = masked

    return frame_features, frame_mask

def sequence_prediction(path):
    """
    Predict the lable for the video
    """
    class_vocab = label_processor.get_vocabulary()

    frames = load_video(os.path.join("test", path))
    frame_features, frame_mask = prepare_single_video(frames)
    probabilities = sequence_model.predict([frame_features, frame_mask])[0]

    for i in np.argsort(probabilities)[::-1]:
        print(class_vocab[i], ' : ', probabilities[i])
    return frames

transfered_models = ["InceptionV3", "Xception", "ResNet101", "ResNet152V2", "DenseNet201", "MobileNetV2", "NASNetMobile"]

features_number = {"InceptionV3":2048, "Xception":2048, "ResNet101":2048, "ResNet152V2":2048, "DenseNet201":1920, "MobileNetV2":1280, "NASNetMobile":1056}


########################################################### RUNNING THE FUNCTIONS #####################################################

disbalance = len(train_df[train_df["tag"]=='HCM'])/len(train_df[train_df["tag"]=='CA'])

class_weight = {0: disbalance,
                1: 1.}

for mod in transfered_models:
  NUM_FEATURES = features_number[mod]  
  feature_extractor = build_feature_extractor(mod)
  train_small_data, train_small_labels = prepare_all_videos(train_df_small, train=True)
  val_data, val_labels = prepare_all_videos(val_df, train=False)
  test_data, test_labels = prepare_all_videos(test_df, train=False)
  _, sequence_model = run_experiment(mod, class_weight)