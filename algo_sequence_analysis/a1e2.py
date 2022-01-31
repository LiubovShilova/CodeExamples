import sys
t = sys.argv[1] #input of a first string
s = sys.argv[2] #input of a second string
arg_match = int(sys.argv[3])  #input of a cost for match
arg_mismatch = int(sys.argv[4]) #input of a cost for mismatch
arg_gap = int(sys.argv[5])  #input of a cost for gap
if  (isinstance(s, str) != True) or (isinstance(t, str) != True): #check if strings are str
    print('You have to input two strings!')
    sys.exit(1)    #if not exit the program
try:               #check if input is correct: integer values
    match = int(arg_match)
    mismatch = int(arg_mismatch)
    gap = int(arg_gap)

except ValueError:
    print('match, micmatch and string should be int!')
    sys.exit(1)    #if not exit the program

def argmax(a):       #a function, which finds indexes of max values in an array
    winners = []     #empty list
    d=0              #we will put a max value in d
    for i in range(len(a)):            #find max value and put it in d
        if max(a[i])>d:
            d = max(a[i])
    for i in range(len(a)):           #find raws with max
        if max(a[i])==d:
            k = [y for y, x in enumerate(a[i]) if x == max(a[i])]  #find indexes of max in a raw
            for m in range(len(k)):
                winners.append([i, k[m]])     #put indexes in a list winners
    return(winners)

def rename_row_col(table, columns, indexes):   #function that makes table look good
    for el in range(len(table)):               #indexes is the first column
        table[el].insert(0, indexes[el])
    table = [columns] + table                  #columns is the first raw
    return(table)

def table_alignment(s, t, match, mismatch, gap):    #make an allingment table
    m, n = len(s), len(t)
    T = []                                          #empty table
    T.append([0] * (m + 1))                         #init first raw with 0
    for i in range(1, n + 1):
        T.append([None] * (m + 1))                  #add empty raw
        T[i][0] = 0                                 #init first column with 0
        for j in range(1, m + 1):                   # fill in the col with the max possible cost or 0
            T[i][j] = max(0,
                          T[i - 1][j - 1] + (match if s[j - 1] == t[i - 1] else mismatch),
                          T[i - 1][j] + gap,
                          T[i][j - 1] + gap)
    return (T)


a = table_alignment(s, t, match, mismatch, gap) #take the T
columns = [' ', ' '] + list(s)                  #names of columns (string s)
index = [' '] + list(t)                        #indexes (string t)
renamed_table = rename_row_col(a, columns, index) #make a table look like in the example
with open(r'dp_table.txt', "w") as table_file:  #print in file
    for table_el in renamed_table:
        print(*table_el, file = table_file)


m, n = len(s), len(t)                        #I repeat the procedure of getting a table, because cloning it with
T = []                                       #T2 = T[:] does not work (Python changes initial T anyway)
T.append([0] * (m + 1))
for i in range(1, n + 1):
    T.append([None] * (m + 1))
    T[i][0] = 0
    for j in range(1, m + 1):
        T[i][j] = max(0,
                      T[i - 1][j - 1] + (match if s[j - 1] == t[i - 1] else mismatch),
                      T[i - 1][j] + gap,
                      T[i][j - 1] + gap)
winner = argmax(T)                         #find indexes of maximum


b = T                                      #To understand, that this is a new made T, I will give it a new name b
for allign in range(len(winner)):          #traceback: How did we get the max?
    i, j = winner[allign]                  #i,j are indexes of one particular max
    list1 = [t[i - 1]]                     #two lists with s and t elements of the alignment. First element - max.
    list2 = [s[j - 1]]
    while ((b[i][j] >= match) and (max(b[i - 1][j], b[i][j - 1], b[i - 1][j - 1]) != 0)): #iterate untill we have only 0 on top, left and left top diagonal
        if max(b[i - 1][j], b[i][j - 1], b[i - 1][j - 1]) == b[i - 1][j]:   #if next max is one position higher
            list1.append(t[i - 2])                                          #put the occording letter from t in list
            list2.append('_')                                               #put gap in another list
            i = i - 1                                                       #move one position higher
            j = j
        elif max(b[i - 1][j], b[i][j - 1], b[i - 1][j - 1]) == b[i][j - 1]: #if next max is on the left
            list1.append('_')                                               #in list 1 add gap
            list2.append(s[j - 2])                                          #in list 2 according letter from s
            i = i                                                           #move 1 position left
            j = j - 1
        else:                       #if the max is on the top left diadonal
            list1.append(t[i - 2])  #according letters put in the lists
            list2.append(s[j - 2])
            i = i - 1               #move to the left and up
            j = j - 1
    with open(r'alignment.txt', "a") as alignment_file:     #for every max we have two lists and the positions where alignments start
        print('pos ', j - 1, file=alignment_file)           #in file we print position of allignment
        print(*reversed(list1), file=alignment_file)        #alligned letters from string t
        print(*reversed(list2), file=alignment_file)        #alligned letters from string s
