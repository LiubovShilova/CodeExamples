#include "dataframe.hpp"
#include "dataframe.hpp"

//initialization
Dataframe::Dataframe() {}

//copy constructor
Dataframe::Dataframe (const Dataframe& other){
    colnames = std::vector<std::string>(); //create an empty vector
    rownames = std::vector<std::string>();
    //fill in the vector
    for (auto i = other.colnames.begin(); i!=other.colnames.end(); i++)
    {
        colnames.push_back(*i);
    }
    for (auto i = other.rownames.begin(); i!=other.rownames.end(); i++)
    {
        rownames.push_back(*i);
    }
    data = std::list<std::vector<ColType>>();
    for (auto i = other.data.begin(); i!=other.data.end(); i++)
    {
        std::vector<ColType> temp;
        std::vector<ColType> that = *i;
        int size = i->size();
        for (int j=0; j<size; j++){
            temp.push_back(that[j]);
        }
        data.push_back(temp);
    }
}

//Copy assignment operator (operator overloading)
Dataframe& Dataframe :: operator = (const Dataframe& other)
{
    colnames = std::vector<std::string>(); //create an empty vector
    rownames = std::vector<std::string>();
    //fill in the vector
    for (auto i = other.colnames.begin(); i!=other.colnames.end(); i++)
    {
        colnames.push_back(*i);
    }
    for (auto i = other.rownames.begin(); i!=other.rownames.end(); i++)
    {
        rownames.push_back(*i);
    }
    data = std::list<std::vector<ColType>>();
    for (auto i = other.data.begin(); i!=other.data.end(); i++)
    {
        std::vector<ColType> temp;
        std::vector<ColType> that = *i;
        int size = that.size();
        for (int j=0; j<size; j++){
            temp.push_back(that[j]);
        }
        data.push_back(temp);
    }
    return *this;
}

//Move constructor
Dataframe::Dataframe(Dataframe&& other)
{
    colnames = std::move(other.colnames);
    rownames = std::move(other.rownames);
    data = std::move(other.data);
}

//Move assignment operator (overload)
Dataframe& Dataframe::operator = (Dataframe&& other)
{
    colnames = std::move(other.colnames);
    rownames = std::move(other.rownames);
    data = std::move(other.data);
    return *this;
}

//providing the number of columns
std::size_t Dataframe::ncols() const {
    int vec_size = 0;
    for (auto i = data.begin(); i!=data.end(); i++){
        auto that = *i;
        if (that.size()>vec_size){vec_size = that.size();}
    }
    return vec_size;
}

//providing the number of rows
std::size_t Dataframe::nrows() const {
    return data.size();
}

//access and/or modify the element of type T stored at row i and column j
template<typename T>
T & Dataframe::get(const size_t i, const size_t j) {
    auto row = data.begin();
    std::advance(row, i);
    auto that = *row;
    return &(that[j]);
}

//access and/or modify the element stored at row named r and column named c
template<typename T>
T & Dataframe::get(const std::string &r, const std::string &c) {
    int col_ind = -1;
    for (int i = 0; i<colnames.size(); i++){
        if (colnames[i] == r){
            col_ind = i;
            break;
        }
    }
    if (col_ind == -1){
        throw std::invalid_argument("The column with the given name was not found");
    }
    int row_ind = -1;
    for (int i =0; i<colnames.size(); i++){
        if (colnames[i] == r){
            row_ind = i;
            break;
        }
    }
    if (row_ind == -1){
        throw std::invalid_argument("The row with the given name was not found");
    }
    auto row = data.begin();
    std::advance(row, row_ind);
    auto that = *row;
    return &(that[col_ind]);
}

//set the column names
void Dataframe::set_colnames(const std::vector<std::string> &cnames) {
    colnames = std::vector<std::string> ();
    for (int i=0; i<cnames.size(); i++){
        colnames.push_back(cnames[i]);
    }
}

//set the raw names
void Dataframe::set_rownames(const std::vector<std::string> &rnames) {
    rownames = std::vector<std::string> ();
    for (int i=0; i<rnames.size(); i++){
        colnames.push_back(rnames[i]);
    }
}

//remove column names.
void Dataframe::clear_colnames() {
    colnames.clear();
}

//remove row names.
void Dataframe::clear_rownames() {
    rownames.clear();
}

//check if column names have been specified.
bool Dataframe::has_colnames() const {
    if (!colnames.empty()){return true;}
    else { return false;}
}

//check if row names have been specified.
bool Dataframe::has_rownames() const {
    if (!rownames.empty()){return true;}
    else { return false;}
}

//adds a new column with entries of type T.
template<typename T>
void Dataframe::add_column(const std::vector<T> &col_values) {

    if(!data.empty()) {
        for (int i = 0; i < col_values.size(); i++) {
            auto row = data.begin();
            std::advance(row, i);
            auto that = *row;
            that.push_back(col_values[i]);
        }
    }
    else{
        for (int i = 0; i < col_values.size(); i++) {
            std::vector<ColType, std::allocator<ColType>> temp {};
            temp.push_back(col_values[i]);
            data.push_back(temp);
        }
    }
}

//remove a column given by index
void Dataframe::remove_column(const size_t i) {
    for (auto j = data.begin(); j!=data.end(); j++){
        auto that = *j;
        that.erase(that.begin() + i);
    }
}

//remove a column given by name
void Dataframe::remove_column(const std::string &c) {
    int col_ind = -1;
    for(int i=0; i<colnames.size();i++){
        if (colnames[i]==c){
            col_ind = i;
            break;
        }
    }
    if (col_ind == -1){
        throw std::invalid_argument("there is no column with a given name");
    }else{
        remove_column(col_ind);
    }
}

//swap columns given by indexes
void Dataframe::swap_columns(const size_t i, const size_t j) {
    for (auto v = data.begin(); v!=data.end(); v++){
        auto that = *v;
        if ((i<that.size())&&(j<that.size())){
            auto x = that[i];
            auto y = that[j];
            that[i] = y;
            that[j] = x;
        }
        else{
            throw std::invalid_argument("i or j are out of range");
        }
    }
}

//swap columns given by names
void Dataframe::swap_columns(const std::string &i, const std::string &j) {
    int col_ind_1 = -1;
    int col_ind_2 = -1;
    for (int c = 0; c<colnames.size(); c++){
        if (colnames[c] == i){
            col_ind_1 = c;
        }
        else if( colnames[c] == j){
            col_ind_2 = c;
        }
    }
    if ((col_ind_1 == -1)||(col_ind_2 == -1)){
        throw std::invalid_argument("The column with the given name was not found");
    }
    return swap_columns(col_ind_1, col_ind_2);
}

//check the elements of the dataframes for equality
bool Dataframe::operator==(const Dataframe &other) const {

    for (int i = 0; i < data.size(); i++) {
        auto row = data.begin();
        std::advance(row, i);
        auto that = *row;
        auto row2 = other.data.begin();
        std::advance(row2,i);
        auto that2 = *row2;
        for(int k=0; k<that.size(); k++){
            if ( that[k] == that2[k]){
                continue;
            }
            else {return false;}
        }
    }
    return true;
}

//check the elements of the dataframes for inequality
bool Dataframe::operator!=(const Dataframe &other) const {

    for (int i = 0; i < data.size(); i++) {
        auto row = data.begin();
        std::advance(row, i);
        auto that = *row;
        auto row2 = other.data.begin();
        std::advance(row2,i);
        auto that2 = *row2;
        for(int k=0; k<that.size(); k++){
            if ( that[k] == that2[k]){
                continue;
            }
            else {return true;}
        }
    }
    return false;
}