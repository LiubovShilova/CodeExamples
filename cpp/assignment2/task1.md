## Liubov Shilova

### 7003130

## Task1

**1.** 

- Class members are private by default, but we can specify otherwise.
- Struct is a class, which members are public by default (often can have any value).

**2.**

- So that user cannot break anything and so the data is protected.
- To provide a clean interface: to hide complicated and messy code by making it private
- We make sure that only certain functions have access to the private data: it helps to protect the inviroment, to debug the program and makes changing of the representation easier (we change only these public functions). Also, it is easier to understand the logic of a datastructure.

**3.**

- Static members of a class are not directly associated with particular objects of a class. This means, that these members are accessible for any object of a class. For example, I have a class "cats". And I have many cats as objects of this class. To keep a track how many cats I have overall, I can have a static variable counter, which will become bigger by 1 with creation of any new cat object. And this variable will be one for all of these objects.
- Const implies that the value of a member cannot be changed, for example, by some function. Const member functions cannot change the object, one can be sure, that it stays the same.

**4.**

- If the functions are const member it cannot modify an object. It also means that other non-const functions would not be able to modify the result of this function work.

- Example: I create a class Cat with the const function get_eye_color(). I want to make sure, that now function further will be able to change the initial color of cat's eyes that I set. (In the world of this program they stay the same forever). If I add something like eye_color+='a' in the get_eye_color function, I will get an error. I will also get an error if I create another function that tries to change the variable eye_color. 


        class Cat{
    
            std::string eye_color;
        public:
            Cat(){}
            Cat(std::string some_color):eye_color{some_color} {}
            void set_eye_color(std::string some_color) {eye_color = some_color;}
            std::string get_eye_color() {return eye_color;} };
        int main(){
            Cat MyCat;
            MyCat.set_eye_color("green");
            std::cout<< MyCat.get_eye_color()<<"\n";
        };
    

**5.**

- Generally "inline": If the function is small and simple we can use inline. It will tell the compiler to put the code of the function right in the code of the main(), where it is used, though the code will still be structured. This may reduce run-time.

- In classes: we can include the full definition for our function in the class definition (inline), or we can have it outside the class. 
- If we include it, it reduces run-time, however we may make file definition messy and if we want to change this function, we should recompile everything that uses it. It will also increase memory space required for binary file. 
- So if the function is simple, short and unlikely to be changed, we can have it inline. If it is long and messy, it is better to have it outside of the class definition. Also, the decision is always up to the compiler.


**6.**

- Destructor is called when the object goes out of scope. If we allocated memory for an object on the heap using new, and then we call delete[], the destructor will be called. Else if we allocated memory in the stack, it will be called at the end of a scope, when we are exiting the function, where the object was created.
- Usually it deallocates or releases the memory, by deleting what we have created in constructor, and uninitializing anything that we initialized in constructor. However, we can set some special behaviour for it (we define the destructor using ~Name_of_class(){//behaviour}).

**7.**

If the behavior of the function in the child class is special we use virtual function. Virtual functions allow us to override methods in subclasses, it postpones function call resolution until runtime. If we want to override a function from mother class, with a new behaviour of subclass (but the same type and name), we need to mark the one in the mother class as virtual. It tells the compiler to create a v-table, so when it is overriden, it points to the correct function.

**8.**

Friend functions are not member functions, but can access private members of a class. Virtual functions are member functions, which can be overwritten in "subclass". But friendship is not inherited! So if we use it in a class as a method, the "friend" mark makes no sence, the function is already in a class, it already has access to other members of the class. If the function is outside of the class, it will not necesserilly have access to future subclasses, so the "virtual" part makes no sense. 

**9.**

When I make the abstract class, I am planning to use functions and objects from it in subclasses. The abstract class allows to not write the same code several times for each of the similar subclasses. Pure virtual function has only declaration in the mother class, without full definition. We might not know the exact behaviour which we need from this function in a "child class", then we do not set any behaviour strait away, we will override it in subclass. And this makes the class with this function abstract.


**10.**

Because it may be ambiguous. It is not clear which behaviour (which function) should be inherited from which class. And it is difficult do debug, because it will just work the wrong way. Making the inheritance virtual can solve the issue.

**11.**

Concrete class has a certain behaviour written for every member. We can create an object of this class and the class specifies what this object has and what it does. Interface is a kind of abstruction: it does may not specify the particular behaviour. It just settles a contour for a future subclasses. Abstract class is a public interface. 
