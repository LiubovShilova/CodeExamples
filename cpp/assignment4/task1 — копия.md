## Liubov Shilova
### 7003130

------------------------------------------------------------------------------------------------------

## Task 1

**1. Why is it dangerous to return a reference when you must return an object?** 

Because than it becomes difficult to control the memory sources. If the object is created in a function, and we return reference to this object, we need to allocate memory in the heap with "new" (otherwise we return reference of object that no longer exists). However, then we need to care about destroing the object ourselves. And it can be difficult for a user to mention that the function returns a reference rather then the object itself. As a result, we'll have memory leaks, which are hard to track. Also, if we use static object, we may get unexpected results when we call the function several times: we have only one object, so the result previous call of the function becomes not rechable. Again: difficult to track and debug. (examples were given in the lecture).

**2. What is an implicit interface in the context of compile-time polymorphism?**

Unlike the explicit interface, in the implicit one we have a template, in which we can specify classes inherited from any class, that has suitable names of the functions. The semantics may be completely different.
For example, in the code below T may represent any class which has functions with name function1 and function2:


     template<typename T>
     void do_some_functions(T& Object_with_unknown_type){
        T temp(Object_with_unknown_type);
        temp.function1();
        temp.function2()
     }

   
The interface of Object_with_unknown_type here is determined by operations in template (function1 and 2). Thus, the interface of T is implicit. Different template parameters may lead to different behaviour in functions. So the polimorphism occurs during compilation through template instantiation and function overloading resolution, it is compile-time polymorphism.

**3. Why should you prefer range member functions to their single-element counterparts?**

They are easier to write, they express intent more clearly and exhibit higher performance.
For example, if we want to insert some data in the beginning of the vector, the "for loop" implementation will shift all elements of the vector in memory each time. However, if we simply use 

    insert(vecto.begin(), data.begin(), data.end())

thw size of data is nown straight away and the elements of vector are shifted only once. And the code looks nicer :)

**4. Why should you avoid in-place modification of keys in a set?**

The modified element stays at the same place. Then the order maybe incorrect (not sorted anymore). Thus, the container is invalid.  So the correct way is to first find the element that we want to modify, copy it to new element, erase initial element from the keys, modify the lement separate from the set, and insert it back.

**5. Why should predicate functions be pure?**

Because otherwise the behaviour of the function depends not only on the parameters, but on sth else (on the previous times it was called, for example). And then the result of the function may be unexpected.

**6. Why should you avoid the default capture modes of lamdbas?**

Because then there is the dependency on the rest of the code. Lambda takes every variable that it needs from the local scope. But these will be destroed as soon as we leave the scope! So again, unexpected behaviour: we then may have a reference to the value that was destroed or may hav problems with memory allocation.

**7. What do std::move and std::forward do?**

- std::move casts its argument (r or l value) to an rvalue(the value, which adress we cannot take). We take take out all reference information from the argument (so we have only value), add &&, and make it to be of certain type ReturnType. So the data is not necesserily moved, it may be copied (for example, if we use const for the value that we give).
- However, sometimes we want rvalue to be passed to some process as rvalue, and lvalue as lvalue. std::forward allows to do it. 

**8. How are the strings of a std::set<std::string*> sorted? How you would make them sorted lexicographically? Provide the necessary code.**

The set will be sorted by the adresses of the strings (like int comparison). So the resulting ordr will depend on memory, where the string was inserted. To get the ordering we expect we need to provide our own comparison function object (functor), in which we should dereference both strings we are comparing.

The example below works for any pointer-like types (itarators as well).

       struct DerefLess{
         template <template PointerType>
         bool operator()(PointerType ptr_of_str1, PointerType ptr_of_str2) const{
             return *ptr_of_str1 < *ptr_of_str2;
         }
       }

       std::set<std::string*, DerefLess> lexicographically_ordered_set;
