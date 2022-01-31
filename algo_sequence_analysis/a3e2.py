
import argparse    #import packages

# **Encoding**

def get_pos_array(s):  #Get pos array. We can sort suffixes, not the rotated string, the result is the same
    suffixes = {}                   #empty dictionarry
    for i in range(len(s)):         #make suffixes as keys and their indexes as values
        suffixes[s[i:]] = i
    key_list = list(suffixes.keys())
    key_list.sort()                #sort lexicographically
    pos = [suffixes[el] for el in key_list]   #get their indexes as pos
    return pos

def compute_bwt (pos , s):     #actually make bwt, function from the lecture
    bwt = ''
    for i in range (len(s)):
        bwt += s[( pos[i] - 1) % len(s)]
    return bwt


def RLE(s):                   #Run length encoding
    encoded_string = ''            #initiate empty string
    for i,c in enumerate(s):
        if i == 0 or c!=s[i-1]:   #if it is the first character or character different from previous
            counter = 1           #set counter to 1
        if (i+1)<len(s):          #if we are not at the end of the string
            if c==s[i+1]:         #if the letter is repeated
                counter+=1        #add 1 to the counter
            else:
                encoded_string+=str(counter) #if not, put the number of repeated characters and the character in a new string
                encoded_string+=c
        else:                     #if not, put the number of repeated characters and the character in a new string
            encoded_string+=str(counter)
            encoded_string+=c
    return encoded_string

def Encoding(s):        #Actually encoding
    if s[-1] != '$':    #Add $ if there is not any
        s += '$'
    pos = get_pos_array(s) #Make pos array
    bwt = compute_bwt (pos , s) #compute btw
    final = RLE(bwt)    #compress
    return final


# **Decoding**
def de_RLE(s):            #Decoding
    l = len(s)
    i = 0
    initial_bwt = ''      #initiate with an empty string
    while i < l:          #While we are not at the end of a string
        s_int = ''        #here we will store a number
        a = s[i]          # a is a character with index i
        while '0' <= a <= '9': #While a is a number
            s_int += a    #put it to the previous number, as a is a string + works (like 1+2 = 12)
            i += 1        #look at the next position
            if i < l:     #if it is not the last one
                a = s[i]  #go to the next character
            else:
                break
        i += 1            #go to the next character 
        if s_int != '':   # if the character was a number
            initial_bwt+=s[i-1]*int(s_int) #we multiply letter by the number before the letter
    return initial_bwt


def Make_C_array(bwt_string):  #Make C array
    C_array =[] #initiate empty list
    L = bwt_string #L and F, just like on the lecture
    F = ''.join(sorted(L))
    for i in sorted(list(set(bwt_string))): #C(character) is equal to the index in sorted F
        new_c = F.find(i)
        C_array.append(new_c)
    return C_array

def LF(bwt , C, sigma):  #compute LF, the function is from the lecture
    char_to_idx = dict ((c, i) for i, c in enumerate (sigma))  #c and i swap
    counts = list (C)                              # copy list
    LF = []                                        #initialise empty list
    for i, c in enumerate (bwt):                   #for each character in bwt
        char_idx = char_to_idx [c]                 #find the position in F
        LF.append (counts [char_idx])            #put in list
        counts[char_idx] += 1                    #add 1 to counts, as we used this character already
    return LF

def decode_bwt (bwt , LF ):  #Decode, taken from the lecture
    s = '$'                  #start building string from the end
    i = 0
    while len(s) < len(bwt ): #while our string is smaller than bwt
        s = bwt[i] + s        #add the character that stands in L in the beginning
        i = LF[i]             #look at the next i
    return s

def Decoding(encoded_string):   #Overall decoding
    initial_bwt = de_RLE(encoded_string) #decompress
    C = Make_C_array(initial_bwt)        #make C array
    lf = LF(initial_bwt, C, sorted(list(set(initial_bwt)))) #Compute LF
    return decode_bwt (initial_bwt , lf)

#read arguments
parser = argparse.ArgumentParser(description='Exercise 2')
parser.add_argument('--encode', required = False, help = 'From string to bwt', action="store", dest="file_for_enc")
parser.add_argument('--decode', required = False, help = 'From bwt to string', action="store", dest="file_for_dec")
args = parser.parse_args()

#if we have encode option
if args.file_for_enc != None:           
    file = open(args.file_for_enc)  #open file
    s = file.read()                 #reed content
    s = s.strip()                   #strip the line
    print(Encoding(s))              #run encoding function

#if we have decode option
if args.file_for_dec != None:
    file = open(args.file_for_dec)  #open file
    s = file.read()                 #reed content
    s = s.strip()                   #strip the line
    print(Decoding(s))              #run decoding function

#if somebody does not give any arguments to the poor program
if (args.file_for_enc == None) and (args.file_for_dec == None):
    print("Please choose --encode or --decode and give a path to the string!")




