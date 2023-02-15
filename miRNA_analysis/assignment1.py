#!/usr/bin/env python
# coding: utf-8

# In[1]:


import re                                               #import libraries
import pandas as pd
import numpy as np
import os
import gzip
from os.path import isfile, join
file = open('E:/RNA/mirbase/22/miRNA.xls/miRNA.csv', 'r') #open the table from release 22.0
f = file.read()                                           #and save only raws with needed species
hsa = re.findall(r'.+hsa-.+', f)
mmu = re.findall(r'.+mmu-.+', f)
rno = re.findall(r'.+rno-.+', f)
cel = re.findall(r'.+cel-.+', f)
al = hsa + mmu + rno + cel
al = pd.DataFrame(al)
al = pd.DataFrame(al[al.columns[0]] .str.split(';',10).tolist()) #make it look like a nice DataFrame
al = al.drop(al.columns[10], axis = 1)


# In[2]:


al.columns = ['Precursor_name','Precursor_ID', 'State', 'Precursor_seq', '5p_mature_name', '5p_mature_ID',
              '5p_mature_seq', '3p_mature_name', '3p_mature_ID',
              '3p_mature_sequence']  #Column names


# In[3]:


#Here is basically the code of ex. 1. I look through the files and find .diff files
with os.scandir('E:\RNA\mirbase') as entries:
    filesineed = [entry for entry in entries if entry.name.endswith('0') or entry.name in ['14', '15', '16', '17', '18', '19', '20', '21', '22']]
    onlyfiles = []
    for file in filesineed:
        for f in os.listdir(file):
            if isfile(join(file, f)):
                onlyfiles.append(join(file, f))
diff = []
release = pd.DataFrame(np.nan, index=[0], columns=['Precursor_name', 'ID', 'New', 'Release'])
for file in onlyfiles:
    if file.endswith('diff'):
            diff.append(file)
            fi = open(file, 'r')                   
            f = fi.read()
            #in each .diff file I find the raws, which say, that some new miRNA of needed species was found.
            new_hsa = re.findall('.+hsa.+NEW', f)
            new_mmu = re.findall('.+mmu.+NEW', f)
            new_rno = re.findall('.+rno.+NEW', f)
            new_cel = re.findall('.+cel.+NEW', f)
            al_new = new_hsa + new_mmu + new_rno + new_cel # I make a DataFrame, with all new miRNAs: release + their name.
            al_new = pd.DataFrame(al_new)
            al_new = pd.DataFrame(al_new[al_new.columns[0]] .str.split('\t',2).tolist())
            al_new.columns = ['Precursor_name', 'ID', 'New'] #At the beginning dg has some unneeded columns, as I make it from file string
            al_new['Release'] = file[15:17]
            release = pd.concat([release, al_new])           #Each iteration "release" table becomes bigger
            fi.close()

    elif file.endswith('diff.gz'):      #If file was packed, I reed it with another tool (quicker than unpack)
        with gzip.open(file, 'rt') as f:                
                filecontent = f.read()
                if (('hsa' in filecontent) or ('mmu' in filecontent) or ('rno' in filecontent) or ('cel' in filecontent)):
                    diff.append(file)
                    new_hsa = re.findall('.+hsa.+NEW', filecontent)   #the same as for .diff
                    new_mmu = re.findall('.+mmu.+NEW', filecontent)
                    new_rno = re.findall('.+rno.+NEW', filecontent)
                    new_cel = re.findall('.+cel.+NEW', filecontent)
                    al_new = new_hsa + new_mmu + new_rno + new_cel
                    al_new = pd.DataFrame(al_new)
                    al_new = pd.DataFrame(al_new[al_new.columns[0]] .str.split('\t',2).tolist())
                    al_new.columns = ['Precursor_name', 'ID', 'New']
                    al_new['Release'] = file[15:17].rstrip('\.\\')
                    release = pd.concat([release, al_new])
    
#Thus, we have a table with miRNA names and releases. The table was made from .diff files.          
    


# In[4]:


release = release.drop(['New', 'ID'], axis = 1)


# In[5]:


#Merge the release data with other data. Find out, that first 3 versions of miRBase don't have .diff file.
res_table = pd.merge(al, release, on = 'Precursor_name', how = 'left' )


# In[6]:


#Retrive the data about first three releases by looking at .fa files or at files with species name = myfiles. 
with os.scandir('E:\RNA\mirbase') as entries:
    first_four = [entry for entry in entries if entry.name in ['1.0', '2.0', '3.0']]
    myfiles = []
    for file in first_four:
        for f in os.listdir(file):
            if join(file, f).endswith('precursor.fa') or join(file, f).endswith('precursor.fa.gz') :
                myfiles.append(join(file, f))
            elif (('hsa' in f) or ('mmu' in f) or ('rno' in f) or ('cel' in f)):
                myfiles.append(join(file, f))


# In[7]:


for i in range(len(res_table['Precursor_name'])):
     for text_file in myfiles:                                 #Myfiles = files from first three additions
        if res_table.Release.isna()[i] == True:                #If we still do not no release go through "myfiles"
            pr_name = res_table.Precursor_name[i]              #And find where the miRNA name appears first
            pr_ID = res_table.Precursor_ID[i]
            if text_file.endswith('gz'):                       #If it is packed, read with gz
                with gzip.open(text_file, 'rt') as f:
                    filecontent = f.read()
                    if (pr_name in filecontent) or (pr_ID in filecontent):
                        res_table.Release[i] = text_file[15]   #I take the number of release from the file path
            elif text_file.endswith('fa'):                     #If it is not packed, just read it.
                f = open(text_file)
                filecontent = f.read()
                if (pr_name in filecontent) or (pr_ID in filecontent):
                        res_table.Release[i] = text_file[15]   #And take the number of release from the file path
            
        
        


# In[8]:


res_table = res_table.drop_duplicates(subset = ["Precursor_name"]) #We do not need repeated names
res_table = res_table.reset_index()                                #Reset the indexes
res_table.drop(['index'], axis = 1)                                #And drop unneeded columns


# In[9]:


res_table.insert(0, 'Species', res_table['Precursor_ID']) #Insert the Species
for i in range(len(res_table['Species'])):
    res_table.Species[i] = res_table.Species[i][:3]       #The species is codded in first three letters of ID


# In[10]:


res_table = res_table.drop(['State'], axis = 1)          #Drop unneeded columns


# In[12]:


res_table = res_table.drop('index', axis = 1)


# In[13]:


res_table.to_csv("Master_table.csv", sep = ',')          #Finally make a file

