import argparse                                 #import packages and reed the arguments
parser = argparse.ArgumentParser()
parser.add_argument('input')
parser.add_argument('output')
args = parser.parse_args()

def edit_distance (s,t): #mainly is taken from lecture 2
    """
    Performes the alignment with costs: 0 for match, 1 for mismatch and gap. 
    Input: 2 strings
    Output: the list with aligning table and the cost of alignment (edit distance)
    """
    m, n = len (s), len (t)
    T = [] # create empty matrix
    T. append ([j for j in range (m +1)]) # init row 0
    for i in range (1, n +1):
        T. append ([ None ] * (m +1)) # add empty row i
        T[i ][0] = i # init 0th column of row i
        for j in range (1,m +1):
            T[i][j] = min(
                T[i -1][j -1] + (0 if s[j -1]== t[i -1] else 1),
                T[i -1][ j] + 1,
                T[i][j -1] + 1
                )
    edit_d = T[n][m]
    return [T, edit_d]

def align(t,s):        #visualisation of pairwise alignment, mainly similar to assignment 1

    """
    Performes the alignment with costs: 0 for match, 1 for mismatch and gap. 
    Input: 2 strings
    Output: two lists with letters, that illustrate the alignment
    """
    T = edit_distance(s,t)[0] #make the alignment table
    i = len(t)                #stand at the bottom right
    j = len(s)
    list1 = []                #initiate both strings with empty lists
    list2 = []
    while (i!=0 or j!=0):     #Until we come to the beginning
        if min(T[i-1][j], T[i][j-1], T[i-1][j-1]) == T[i][j]:
            list1.append(t[i-1])
            list2.append(s[j-1])
            i = i-1
            j = j-1
        elif min(T[i-1][j], T[i][j-1], T[i-1][j-1]) == T[i-1][j]:
            list1.append(t[i-1])
            list2.append('-')
            i = i-1
            j = j
        elif min(T[i-1][j], T[i][j-1], T[i-1][j-1]) == T[i][j-1]:
            list1.append('-')
            list2.append(s[j-1])
            i = i
            j = j-1
        elif min(T[i-1][j], T[i][j-1], T[i-1][j-1]) == T[i-1][j-1]: #find the min of three up-left-diagonal squares and go to this min
            list1.append(t[i-1])
            list2.append(s[j-1])
            i = i-1
            j = j-1
            
    list1.reverse() #read from right to left and get the alignment
    list2.reverse()
    return [list1, list2]

with open(args.input) as f:                #read the lines
        lines = f.readlines()              
lines = [line.strip() for line in lines]   #delete \n
l_numbers = {}                             #l_numbers is a dictonary, which stores the string and its number (e.g. ind '5')
strings = []                               #just list of strings
str_name = []                              #strings names (the thing starting with ">")
for i in range(0, len(lines), 2):          #each second line is a string
    strings.append(lines[i+1])
    str_name.append(lines[i])
    l_numbers[lines[i+1]] = i


distances = []                             #here we store the sum of distances for each string
dic = {}                                   #here the same, but we associate it with the string
for p in strings:                          #take each string and calculate the sum distance
    s = 0
    for q in strings:
        s+=edit_distance (p,q)[1]
    distances.append(s)                    #store in the list and check a few things to store it in dic:
    if s in dic.keys():            #if it was made before
            dic[s].append(p)       #just add its index to dictionary, so several indexes are associated with same k-mer
    else:                          #if it is new
        dic[s] = [p]               #add it to dict
print(*distances, sep = ',')               #print

the_center_star = dic[min(dic.keys())][0]  #the string, which is the closest to all others is THE STAAAAR

strings.remove(the_center_star)    #It has nothing to do with all these not cool simple strings

paired_als = []                    #initiate the list for paired alignments
for line in strings:               #THE STAR will be paired with every single one of them (shock)
    paired_als.append(align(the_center_star, line))

#here comes the fun
multialignment = paired_als[0]     #take the first pair                
for i in range(1, len(paired_als)):#and then add others to it one by one
    star_form = paired_als[i][0]   #the one that comes star
    
    star_main = multialignment[0]  #the one that we have
    if star_form == star_main:     #if they are equal no problem just put them together
        multialignment.append(paired_als[i][1])
    else:                          #if they are not, you are going to deal with their gaps
        #Which gaps do we need to add to upcomming pair?
        
        for n in range(max(len(star_main), len(star_form))+1):
            
            if n >= len(star_main):
                for al in multialignment:
                    al.append('-')
                    
            elif n >= len(star_form):
                for al in paired_als[i]:
                    al.append('-')
            else:
                if star_main[n]!=star_form[n]:
                    if star_main[n] == '-':
                        for al in paired_als[i]:
                            al.insert(n, '-')
                            
                    else:
                        for al in multialignment:
                            al.insert(n, '-')
                            
        
        multialignment.append(paired_als[i][1])  #after we put all needed gaps, we can add the string to our alignment


star_ind = int(l_numbers[the_center_star])    #need to print the alignment in the right order. Where was star (Ac) initially?

star = multialignment[0]
multialignment.remove(star)                      #take the Ac from the beginning and put where it should be
multialignment.insert(star_ind, star)


with open(args.output, 'w') as file:             #print it all in the needed way
    for ind_string in range(len(multialignment)):
        print(str_name[ind_string], file = file)
        print(*multialignment[ind_string], sep ='', file = file)
