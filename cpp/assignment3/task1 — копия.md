**1. What is the difference between the keywords typedef and using? Also, state one usage example for both.**


- Typedef declaration is an init-statement. This implies that it can be used with initialization statements. So, for example, in loop:

     for(typedef int Func; Func{} != 0;) {} //valid

- Using is relatively new (since C++ 11)! And it looks more readable. Using is alias-declaration, not an init-statement, so we cannot use in contexts allowing initialization statements. 

	for(using Func = int; Func{} != 0;) {} //error: need expression
	
	
However, using is more flexible comparing to the typedef, we can use it in a template and specify it further (such as std). :

Examples: 

     typedef std::string Str;
     //semantically equivalent to
     using Str = std::string;

**2. What are iterators? Name at least 3 different types of iterators and explain their functionalities.**


Basically it is a way to step through the elements of collections of objects, usually through container or part of it. From the lecture:
Iterator helps to link containers with algorithms. It is a type that supports the “iterator operations”:
- ++ Go to next element
- "*" Get value
- == Does this iterator point to the same element as that iterator?
- and more depending on iterator (e.g. --, +, and [ ])

There are different types of iterators for different containers and algorithms (info below is also from the lecture).

- Input iterators: support equality/inequality comparisons, can be dereferenced as rvalue.

- Outut iterators: can be dereferenced as lvalue, only for mutable iterator types.

- Forward itarators: in addition to input and output iterations possibilities, are default-constructable and allow multi-pass (neither dereferencing nor incrementing affects dereferenceability).

- Bidirectional iterators: can also be decremented.

- Random access: also supports "+" and "-", ">", "<", ">=", "<=", "+=", "-=", and dereference with "[]". Thus the most flexible ones.

**3. What is a crucial difference between an input and a forward iterator?**

- Input iterator can be used in sequential input operations. Every value pointed by the iterator is read-only once. After that the iterator is incremented. 
- Forward iterators are the combination of both input and output iterators. Values can be accessed and modified.

**4. Why are iterators to a std::vector invalided by insert, push_back or erase?**

Because the iterator pointes to elements. When we add/remove an element the iterator still points to the same location. But as we change the values, they shift, we have to invalidate the iterator. Otherwise it would point to God knows what (somtimes we can understand what, but it is not pretty in the script) :)
    
     std::vector<int> vec {1,2,3,4,5};
     auto it = std::find(vec.begin(), vec.end(), 5);
     if(it != vecArr.end()){
         vecArr.erase(5); 
         //now it points to element which is no longer there
         std::cout<<(*it)<<"\n"; // Behaviour unpredictable   
     }
     
     
**5. Which container would you choose and why if**

**a) you need to do sorted traversal and no duplicates are allowed**

Set. Because it implemented as a kind of a tree, has no duplicates, it is sorted and we can find the element faster.

**b) the order is not important and no duplicates are allowed**

If we still only need to find an element, than unordered_set. Because the order is not important, and container has unique keys.

P.S. But if we also may need to find element by index with [], then vector. Traversing vector is faster than list.

**c) you only need to add elements at the end and want fast traversal**

Vector, because we push_back without shifting (like if we push in the middle). Traversing vector is faster than list. And we can use [] comparing to the set.

**d) you need to provide a dictionary that can associate multiple values with the same key**

unordered_multimap. It is a collection of key-value pairs, hashed by keys and several values can be associated with one key. 

**6. How does an STL algorithm usually indicate "not found" or "failure"?**

It will return the end of the sequence. That is a pointer, pointing "right after" the last element. 

**7. What does std::sort(b,e) use as its sorting criterion?**

Comparison function object. This is an object that satisfies the requirements of Compare: it returns ​true if the first argument is ordered before the second in needed vector. By default it sorts in non-descending order. But we can modify the function to make the compatison logic as we want it to be and give it as the parameter of std::sort().

**8. Why is the STL container set missing a [] (subscript) operator?**

Because a set is implemented as kind of a tree, it doesn't have indexing system. Unlike the vector or list, the elements are not following each other there, so we cannot understand where the element is stored just from looking at the index. In vector it's easy: we can just add the index to the adress of the beginning, and we are there. In set it is not a sequence, it's a tree. 

**9. Why does std::map require its key_type to be copy-constructible and its mapped_type to be default constructible?**


Because map is implemented as a hash table. It means that for each key we should have a backet in memory to store the value. The key and value should be copy constructable, so we can copy it and save the initial value.  Also we need to allocate memory, to create the buckets in the memory, where the data is stored. For example we have a map, and then we use operator [] and give the map some new key, that was not there before. In this case we need to initiate respective value (associated with the key), but we don't have any information to pass the object. So mapped_type has to be default constructable. 

**10. Why is std::forward_list missing a size member function and why is the empty member-function still implemented if it is typically equal to container.size() == 0?**

The main idea of forward list is to allow constant time insert and erase operations. The elements are not stored one by one in memory cells, but connected with pointers. If we make a counter of elements for size() function we would increase the working time, as each removal/insertion would be coupled with changes in counter variable, which should be one for all elements. Thus, theoretically it would be possible to implement this, but then we also increase the time, and the main idea of forward least is high speed.
To implement empty we don't need to know exact size. We can just compare end() and begin() pointers. If they are equal, the list has no elements.
