
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>


float find_min(std::vector <float> v){
    /*
     input: sorted vector of floats
     returns: minimal (first) value
    */
    float min_value = 0;
    if (!v.empty()){
        min_value = v[0];
    }
    return min_value;
}

float find_max(std::vector <float> v){
    /*
     input: sorted vector of floats
    returns: maximal (last) value
    */
    float max_value = 0;
    if (!v.empty()){
        max_value = v.back();
    }
    return max_value;
}

float find_mean(std::vector <float> v){
    /*
     input: vector of floats
     returns: mean value
    */
    float mean;
    mean = accumulate( v.begin(), v.end(), 0.0)/ v.size();
    return mean;
}

float find_third_quartile(std::vector <float> v){
    /*
     input: sorted vector of floats
     returns: 3rd quartile by 1st wiipidia method
    */
    int unsigned long  vector_size = v.size();
    float third_quartile;
    if (vector_size%2 == 0){
        if (vector_size%4 == 0){
            third_quartile = (static_cast<float>(v[3*vector_size/4-1]+v[3*vector_size/4]))/2;
        }else{
            third_quartile = v[3*vector_size/4];
        }
    }else{
        //do not include the median in either half
        if ((vector_size-1)%4 ==0){
            third_quartile = (static_cast<float>(v[(vector_size-1)*3/4]+v[(vector_size-1)*3/4+1]))/2;
        }else{
            third_quartile = v[(vector_size/2)+(vector_size/4+1)];
        }
    }
    return third_quartile;
}

void nth_smallest(std::vector<float> v, int n){
    /*
     input: sorted vector of floats, integer n
     returns: nth smallest (index n-1)
    */
    if (n > v.size()){
        std::cout<<"In";
    }else{
        std::cout<<v[n-1];
    }
}

void mth_largest(std::vector<float> v, int m){
    /*
     input: sorted vector of floats, integer m
     returns: mth largest (index size of the vector-m)
    */
    if (m > v.size()){
        std::cout<<"Im";
    }else{
        std::cout<<v[v.size() - m];
    }
}

void print_metrics(std::vector <float> v, int n, int m){
    /*
     input: unsorted vector of floats, integers n, m
     prints minimum, maximum, mean, third quartile, nth smallest and mth largest values,
     each separated by one space
    */
    sort(v.begin(), v.end());
    std::cout << find_min(v) << " " << find_max(v);
    std::cout << " " << find_mean(v) << " " << find_third_quartile(v) << " ";
    nth_smallest(v, n);
    std::cout << " ";
    mth_largest(v, m);
    std::cout << "\n";
}

int main(int argc, char *argv[]) {

    int n{}, m{};

    if (argc <= 1) { //if no arguments were given
        n = 0;
        m = 0;
    } else {
        std::stringstream n_string{argv[1]}, m_string{argv[2]};
        if (!(n_string >> n)) { n = 0; } //if cannot convert, set n and m to 0
        if (!(m_string >> m)) { m = 0; }
    }

    std::string line;
    std::string element;
    std::vector<std::string> vec_of_lines;
    std::vector<float> v_numbers; //temporary vector
    std::vector<std::vector<float> > matrix;

    //read input line by line until the empty line and store in vec_of_lines:
    while (getline(std::cin, line)) {
        if (line.empty()) { break; }
        vec_of_lines.push_back(line);
    }

    //from each line numbers are stored in vector v_numbers and in matrix, metrics for raws are printed
    for (auto i = vec_of_lines.begin(); i != vec_of_lines.end(); ++i) {
        line = *i;
        std::string string_number;
        std::stringstream ss(line);
        while (ss >> string_number) {
            v_numbers.push_back(std::stof(string_number)); //v_numbers is a raw vector
        }
        matrix.push_back(v_numbers);
        print_metrics(v_numbers, n, m);
        v_numbers.clear();
    }

    for (int j = 0; j<matrix[0].size();j++){
        for (int i = 0; i<matrix.size();i++){
            v_numbers.push_back(matrix[i][j]);  //v_numbers is a column vector
        }
        print_metrics(v_numbers, n, m); //print metrics for columns
        v_numbers.clear();
    }

    for (int i = 0; i<(matrix.size());i++){
        v_numbers.push_back(matrix[i][i]); //v_numbers is a diagonal vector
    }

    print_metrics(v_numbers, n, m); //print metrics for diagonal
    v_numbers.clear();

    for (unsigned long int i = 0; i<(matrix.size()-1); i++){
        for (unsigned long int k = 1; k<(matrix.size()-i); k++){
            v_numbers.push_back(matrix[i][i+k]); //v_numbers is upper triangle vector
        }
    }

    print_metrics(v_numbers, n, m); //print metrics for upper triangle
    v_numbers.clear();

    return 0;
}