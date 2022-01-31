#import libraries
import sys

#accessing the contents of the input
filename = sys.argv[1]
f = open(filename, 'r')
line = f.readline()

#making a predecessor array out of it
input_list = []
for ele in line.strip().split(","):
    input_list.append(ele)

#Creating lists of None    
pos = [None for i in range(len(input_list))] 

#Putting 0 instead of '-' in input(psi) and pos   
zero_index = input_list.index('-')
pos[zero_index] = 0
input_list[zero_index] = 0

#Making everithing numeric
input_list = [int(i) for i in input_list]

#compliting the pos                           
for val in range(len(input_list)-1): #start from val = 0 (which used to be '-'
    r = pos.index(val)               #r is just an index of value we are looking at
    psi_ind = input_list.index(r)    #r is also our next value
    pos[psi_ind] = val+1             #in front of this next value we have to have a next number
    #val+=1

#Print!
for i in pos:
    if pos.index(i)<(len(pos)-1):
        print(str(i)+',', end = '')
    else:
        print(i, end = '')

