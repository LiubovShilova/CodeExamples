

class Node:
    
    counterID = -1                  #So that root has ID 0

    def __init__(self, parentID, outedges, label, suffixlink=None): #Class Node
        self.parentID = parentID
        self.outedges = outedges
        self.suffixlink = suffixlink
        self.label = label
        Node.counterID += 1
        self.id = Node.counterID  


class SuffixTree:
    
    def __init__(self):        
            self.text=""  
            self.root = Node(None, {}, [0,0])
            self.arrayNodes=[self.root]                  #I store the whole tree in this array.
            
    def findNode(self, ID):                              #Find the node given its ID. Ideally the ID is equal to index in arrayNodes    
            for i in self.arrayNodes:                      
                if (i.id == ID):
                    return i
            return None

    def isLeaf(self, node_id):
    #"""Returns True if the node given is a leaf, False otherwise"""
        n = self.findNode(self, node_id)
        if len(n.outedges) == 0:                         #If no outedges, then leaf
            return True
        else:
            return False

    def children(self,node_id):
        """Returns the children of a node ID given (not only ID, but the list of children)"""
        n = self.findNode(self, node_id)
        return n.outedges.values()                      #Outedges is dictionary{first letter: Node where edge leads}

    def parent(node_id):                                #Returns parent ID
        """Returns the parent ID of a node ID given"""
        n = self.findNode(self, node_id)
        return n.parentID
        
    def edgeLabel(node_id1, node_id2):
        """ Gives the node label between two given nodes’ IDs, or returns None if there’s no edge between them"""
        n1 = findNode(self, node_id1) #find info about both nodes in array
        n2 = findNode(self, node_id2)
        if n1.parent == node_id2:
            print('The first given node id is a child of the second, connected via edge', n1.label)
            return n1.label
        elif n2.parent == node_id1:
            print('The second given node id is a child of the first, connected via edge', n2.label)
            return n2.label
        else:
            print('The given nodes are not directly connected')
            return None
        
    def new_node(self, active_pos, i, j):
        """ 
        Adds a new leaf node to the suffix tree below active_pos , 
        with a node label s[i:j] k (including i, excluding j) leading to the new leaf.
        """
        active_pos_node = self.findNode(active_pos[0])
        
       
        if active_pos[2] == 0:        #if we are in node
            n = findNode(active_pos[0])
            if (n.isLeaf()):         #rule 1: if we are in leaf, we just extend the label
                self.arrayNodes[active_pos[0]].label[1] = j #So we change the last coordinate in label
                print('We just extend the leaf')
                return n.parentID
            else:                                         #If we are in inner node
                newLeaf = Node(active_pos[0], {}, [i, j]) #Create a leaf
                self.arrayNodes.append(newLeaf)           #Put in the tree storage
                self.arrayNodes[newLeaf.parentID].outedges[self.text[i]] = newLeaf # update the outedges info in the parent
                return active_pos[0]                      #The parent of the leaf existed already
                
        else:                        #We are in the moddle of the edge
            active_pos_node = findNode(active_pos[0])     #node parent of a parent - find info              
            nodeParent = Node(active_pos[0], {}, [active_pos_node.label[1], active_pos_node.label[1] + active_pos[2]]) #Create the node in the middle of the edge                               
            newLeaf = Node(nodeParent.ID, {}, [i, j])     #create the leaf                           
            nodeParent.outedges[self.text[i]] = newLeaf   #create the edge from new inner node to the leaf
                
                
            oldEdge = active_pos_node.outedge[active_pos[1]]    #The old edge needs to be split in 2          
            oldEdge.label[0] =  oldEdge.label[0] + active_pos[2]#The node which it led to - update the label              
            oldEdge.parentID = nodeParent.ID                    #and the parent       
                
            nodeParent.outedges[self.text[oldEdge.label[0]]] = oldEdge   #make an edge from newly created inner node to already existing leaf            
                
            active_pos_node.outedge[active_pos[1]] = nodeParent          #make an edge from upper act pos node to new inner node
                
            del self.arrayNodes[oldEdge.id]                              #update the info in array, where all nodes are stored
            self.arrayNodes.insert(oldEdge.id, oldEdge)
                
            del self.arrayNodes[active_pos_node.id]                
            self.arrayNodes.insert(active_pos_node.id, active_pos_node)
            
            self.arrayNodes.append(nodeParent)                     #Put the new nodes in the array
            self.arrayNodes.append(newLeaf)
                
            return nodeParent.ID
        
    
    
    def add_suffix_link(node_i, node_j): #A suffix link from node_i to node_j
        n1 = self.findNode(node_i)       #find the info
        n2 = self.findNode(node_j)
        self.arrayNodes[node_i].suffixlink=n2 #update the info
        print('Suffix link was added between nodes:', node_i, node_j)
    
    def use_suffix_link(active_pos): 
        depth = active_pos[2]                      #active_pose = (Node, first letter, depth)
        active_pos_node = findNode(active_pos[0])  #find info about the active_node
        link_node = active_pos_node.suffixlink     #find the node where the link leads to
        if depth == 0:
            return (link_node.id, '', 0)           #if we are in the node, just use the link
        else:                                      #if we are not in node
            node_below = link_node.outedges[active_pos[1]] #node below act_pos
            if (node_below.label[1] - node_below.label[0])>depth: #if we do not need to go through other nodes
                return (link_node.id, active_pos[1], depth)
            else:
                start = active_node.outedges[active_pos[1]].label[0] #start of the substring of depth
                end = active_node.outedges[active_pos[1]].label[0] + active_pos[2] #end
                
                distance = depth
                isNodeFind = True
                while (isNodeFind):   #go through these nodes
                    node_below = node_below.outedges[start]
                    if ((node_below.label[1] - node_below.label[0])<distance):
                        distance-=(node_below.label[1] - node_below.label[0]) #how far are we from final distination
                        start+=(node_below.label[1] - node_below.label[0]) #change the start positio, so we know which letter goes next
                    else:
                        isNodeFind=False
                return(node_below.parentID, self.text[start], distance)
                        
    def move_down(self, active_pos, c): 
    """ 
    Moves down the tree from node_j by taking an input character c,
    returns the resulting new active position or None if moving down is not possible
    """
        active_node = findNode(active_pos[0])
        if active_pos[2]==0:   #if we are in a node
            return (active_pos[0], c, 1)
        else:
            node_below = active_node.outedges[active_pos[1]]     #look at the node below
            next_letter = self.text[node_below.label[0] + active_pos[2] + 1] #using the label of node below, find the next letter
            if (node_below.label[0] + active_pos[2] + 1) != node_below.label[1]: #if by moving we are not gettong to the node below
                if next_letter == c:                                             #compare the next letter with c
                    return (active_pos[0], active_pos[1], active_pos[1]+1)   #take a step
                else:
                    return None
            else:  #we made a step and we are in a node
                if next_letter == c:                 
                    return (node_below.id, '', 0) #then return this node
                else:
                    return None
