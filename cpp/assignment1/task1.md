## Liubov Shilova
### 7003130

------------------------------------------------------------------------------------------------------

## Task 1

**1.** 

Type (short version, example):
- Integer (int, 1). Takes 4 bytes, values from -2^31 to 2^31-1
unsigned int has no sign, sinse the range is shifted to the right, from 0 to 2*2^31
short/long - less or more memory. Guess I don't need to describe all of them here.
- Character(char, "a"). takes 1 byte.
- Boolean (bool, True). 1 or 0.
- Floating Point (float, 1.23)
- Double Floating Point (double, 1.23, requires twice as much space and more accurate as float)
- Valueless or Void (void, without value)
- Wide Character (wchar_t, "a", but size bigger than 8 bits)
**2.**
- Expression: sequence of operands (data for the operators to work with) and operators (what is to be done).
 
Example:

    1+2
    
- Statements: parts of code executed in one by one, such as in body of a function. 
Example:

    std::cout<<"Hello world";

The expression, followed by semisolon, is also a statement. But not every statement is an expression.

**3.**

 Every object will be used only according to its type, which we have set when initializing the variable. 
It will not allow us to use any procedures developed for other types on this variable. Variable always has a valid value. It helps us to avoid errors which otherwize would be difficult to debug.

**4.**

When there are many (more than two) possible cases, and when we have fixed data values. In in every case the program should react differently. Works faster and is more readable.

**5.**

Case fall-through when not ending with break. Break should end operator switch. 
Without break the program will go through other unneaded expressions. 
Example: 

    switch (age){
    case 16:
	  std::cout << "Child" << std::endl;
	case 70:
	  std::cout << "Elderly" << std::endl;
    }
    
Will output both "Child" and "Elderly"

**6.**

String cannot do multiplication (*), int canot do my_string.end()

**7.** 

- Pointer is a variable, that has some type, name, and space, where it is stored. 
This variable stores the adress of another variable, that is why it is a pointer.

Example: 

   int* ptr; //I named my pointer ptr. Now it does not point to anything.
   int var = 5; // created int variable var equal to 5 with some adress.
   
- '*ptr' gives the reference value of 'ptr'. But now 'ptr' does not point to anything.I want the value of 'ptr' to be the adress of 'var', and this I do with &:

    ptr = &var;
    std::cout << *ptr << std::endl;

 - Now ptr is pointing to var. if I say *ptr, I tell program to follow the adress that is written in ptr and give me the value that is stored there.
So it will give 5. 

- Reference is kind of the different name for a variable, a new way to call the same location in memory.
we declare reference with &, pointers - with *.

    int& ref = var; // a reference of variable var called ref
    std::cout << ref << std::endl; //will give the value of var.

- So the pointer is separate cell in the memory, we can tell it to point not to the var, but to another variable (var_2).

    int var_2 = 10; // Another variable, with another adress
    ptr = &var_2; //now ptr points to var_2 instead of var. 

- But with reference we cannot change the adress it refers to. It is with var adress. 
Hope I explained ok, and you'll understand, that I understand =)

**8.**

- When we create an array of length n, we reserve continious memory locations for n values. 
The array variable is remembering the adress of the first variable. If we print the var of an array, we'll get the adress. 
So the variable is actually a pointer!

    int some_array[3] = {1,2,3}
    std::cout << some_array << std::endl; //print adress of first element.
    std::cout << *some_array << std::endl; //will print 1, exactly the same as std::cout << some_array[0] << std::endl

- So when I write some_array[2] I tell to use the adress stored in some_array and add 2 to get the element with index 2.

    std::cout << some_array[2] << std::endl; //will print 3

- The story with c-strings is similar: 
each character of a string is stored in a memory cell, we point to the first, others are reached with "+1" for the adress.


    const char * some_word = "Word";
    std::cout << some_word[2] << std::endl; // prints "r"
    std::cout << *(some_word + 2) << std::endl; // prints "r"


**9.**

- Pass by value passes the copy, pass by reference - the actual variable
Example:


 void passByValue(int var);
 void passByReference(int& ref);

    int main(){
      int x = 1;
      std::cout << x << std::endl; // prints 1
      passByValue(x);              // prints 2
      std::cout << x << std::endl; // prints 1, initial x was not changed
      passByReference(x);          // prints 2  
      std::cout << x << std::endl; // prints 2, initial x was changed
      return 0;
    }

    void passByValue(int y)
    {
      y = 2;
      std::cout << y << std::endl;
    }

    void passByReference(int& y)
    {
      y = 2;
      std::cout << y << std::endl;
    }

**10.**

- The declaration is a statement, which names an object and specifies its type.
- Definition is a declaration, which fully specifies the entity declared. It gives the object some memory.
Canot define sth twice, but can declare.

**11.**

- Namespace is a scope with a name. It is a method to prevent name conflict in a large project.
If we give some statement, and we want it to work the certain way,
we specify this by giving the namespace we are working in.
In each namespace one command has one algorithm to do things.

- We can do the following if everything will be from one namespace (example - std):

    using namespace std //all words are from this namespace


- or to avoid collision:

    std::cout << "Blabla" << std::endl; //each time ceparately specify in which namespace.

**12.**

Any time we include that header we bring everything in the namespace into scope.
 Thus we get problems of name hiding and name collision.
 
**13.** 

- Compile-time errors: Syntax errors and Type errors.

Example: semicolon missing

    std::cout<<"Hi"<<"\n"

- Link-time errors.

Example: missing function definition

    int limbs(int arms, int legs);
    //some code
    animal(-4)

- Run-time errors, Detected by computer (crash), Detected by library (exceptions), Detected by user code.

Example: wrong function arguments


    int limbs(int arms, int legs){
    	if (legs<0||arms<0){throw std::invalid_argument("no negative numbers of legs please")}
    	return(arms+legs)
    }
    //some code
    animal(-4)

- Logic errors: no errors, but output is not what we expected

Example:
the whole task 2

**14.**

Variable with dynamic lifetime "Live on the heap (free store)".The memory for them is dinamic: the object can have allocated piece of memory without being initialized at the same time. It is used to create (operator "new") an object and delete it. It's useful for data structures such as vectors. We might not know the exact size of it strait away, but we want to allocate some memory. So the vector can grow or shrink. 

**15.**

New() operator allows to allocate memory in the heap. It is better to initialize allocated memory, as otherwize it would contain random values. If we have raw pointers in our program, we can easily make a misake with them, and it can be difficult to find a bug in uninitialized or somehow invalid pointer. We also have to care about memory leak and delete the objects if we don't need them anymore.

Example:

    int* ptr = new float[7]; //allocate
    //some code
    delete[ ] ptr;

**16.** 

We need to initialize the variable, be careful with pointers. Also, though it is not exactly about allocationg memory, we have to delete objects after usage to avoid memory leak.

**17.**

RAII: “Resource Acquisition is initialization” or scoped resource management. Basically it is strategy for working with objects in OOP languages, a solution to track the objects that we have and delete them after usage. It can handle different errors with exceptions and helps to allocate unique memory on free store for an object.
The idea is that we allow the object to use some resource and then once the object is not needed anymore, we delete it and close the resource. 

## Task 2

**2a**
- In this task we get a lot of lines without #, because the function cange_line followed "pass by value" principal and initial line stayed the same. 
- Also, the first triangle was too big. In order to have its corners right above two other triangles, I changed the number of iterations in for cycle to dimention/2. 
- Also, only one of two needed triangles were printed below. It was not specified for the change_line function there must be 2 triangles growing. As they are absolutely equal, I decided that the easiest way is to run function for the "half line" and then to print it twice.
- Also, there are two things that were unnecessary in the code, though they did not cause any errors. The void functions had return command. Furthermore, the vector v was created and never used.

**2b**
- We have min function for ussigned integers and max function for unassigned integers. First of all, the impressions c = a < b ? a : b are confusing and difficult to debug. But actually min function was calculating signed max, a and b were swapped.
The vector was created, all values are equal to 63 (00111111). In the last element we make it 11111100. When we read the values as unsigned we read it as 252, because the value is smaller than 255. It is the bigger value and it is detected as unsigned max. Logical.
- With signed values we can detect positive and negative values only |127| and smaller. The value we set for it (255) is read as negative! And the last value (252 intensionally) is read as -4! But the function is looking for the maximum instead of minimum, so it registers 63 and prints it.
- I changed the function so it looks for min.
- I set min to be 127 initially (as big as possible).
- So the minimal value is -4.