#include "simple_node.hpp"

//default initializer for leaves
SimpleNode::SimpleNode(double content):content_(content){}

//A constructor that allows you to initialize the node
SimpleNode::SimpleNode(std::vector<std::unique_ptr<SimpleNode>> children, double content)
        :children_{std::move(children)}, content_{content}{}

//deep copy
// Copy constructor.
SimpleNode::SimpleNode (const SimpleNode& other)//:children_(std::vector<std::unique_ptr<SimpleNode>>())
{
    content_ = other.content_;  //content is not a pointer, so just copy

    children_ = std::vector<std::unique_ptr<SimpleNode>>(); //create an empty vector
    for (auto i = other.children_.begin(); i!=other.children_.end(); i++)
    {
        children_.push_back(std::make_unique<SimpleNode>(**i));
    }//fill in the vector
}

//Copy assignment operator (operator overloading)
SimpleNode& SimpleNode :: operator = (const SimpleNode& other)
{
    content_ = other.content_;

    children_ = std::vector<std::unique_ptr<SimpleNode>> (); //make empty vector
    for (auto i = other.children_.begin(); i!=other.children_.end(); i++)
    {
        children_.push_back(std::make_unique<SimpleNode>(**i));
    } //push the values

    return *this;
}

//Move constructor
SimpleNode::SimpleNode(SimpleNode&& other)
        :children_{std::move(other.children_)}, content_{other.content_}
{
    other.content_ = 0;
}

//Move assignment operator (overload)
SimpleNode& SimpleNode::operator = (SimpleNode&& other)
{
    content_ = other.content_;
    other.content_ = 0;
    children_ = std::move(other.children_);
    return *this;
}

//nchild function: how many children does a node have
std::size_t SimpleNode::nchild() const {
    return children_.size();
}

//hight function: the longest path from root to a leaf
std::size_t SimpleNode::height() const {
    int counter{};
    int children_number = nchild();

    //for each child see how deep you can go and rase the counter (recursive algo)
    for (int i=0; i<children_number; i++){
        SimpleNode down_node = *this->children_[i];
        int hight = down_node.height()+1;
        if (counter<=hight){counter = hight;}
    }
    return counter;
}

//is_binary function: check if all nodes have less than 2 children
bool SimpleNode::is_binary() const {
    int children_number = nchild();
    //check recursively
    if (children_number > 2){return false;}
    for (int i=0; i<children_number; i++) {
        SimpleNode down_node = *this->children_[i];
        if (!down_node.is_binary()) { return false; }
    }
    return true;
}

//Operator() to access and/or modify the stored content.
double SimpleNode::operator()() const {
    return content_;
}

double &SimpleNode::operator()() {
    return content_;
}

