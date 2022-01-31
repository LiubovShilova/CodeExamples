import sys
path_to_file = sys.argv[1]                   #read input: the file with long string

k = sys.argv[2]                        #read input: k
path_to_queries = sys.argv[3]                #read input: query
try:               #check if input is correct: integer values
    k = int(k)
except ValueError:
    print('k should be int!')
    sys.exit(1)    #if not exit the program

file1 = open(path_to_file,"r")               #read long string
f = file1.read()                             #put it in variable b
mydict={}                                    #create a dict
for i in range(len(f)-k+1):                  #iterate for numbr of letters -k+1
    if len(f[i:(i+k)])==k:                   #double check that k-mer has right length
        kmer = f[i:(i+k)]                    #make k-mers
        if kmer in mydict.keys():            #if it was made before
            mydict[kmer].append(i)           #just add its index to dictionary, so several indexes are associated with same k-mer
        else:                                #if it is new
            mydict[kmer] = [i]               #add it to dict
with open(r'kmer_indices.txt', "w+") as file:    #create and open a file
    for i in sorted(mydict.keys()):              #print k-mers in alphabetical order
        print(i, end = ' ',file = file)          #on the same line
        print(*mydict[i], sep = ',', file = file)  #print according indexes

with open(path_to_queries, "r") as q:            #open query file
    for line in q:                               #iterate, read line by line
        l = line.rstrip()                        #delete \n on the end of each
        with open(r'queries_indices.txt', "a") as file:     #create and open a file
            if l in mydict.keys():                       #if k-mer is in dict
                print(l, end = ' ', file = file)         #print according k-mer
                print(*mydict[l], sep = ',', file = file) #and all indexes of this k-mer
            else:                                           #if it is not in dict
                print(l, " ", -1 , file = file)             #print according line and -1 instead of indexes

file.close()