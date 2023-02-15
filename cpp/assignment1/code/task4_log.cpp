#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>

int molar_mass(std::string sequence){
    /*
     input: DNA sequence (only A, T, C, G in upper or lower case)
     returns: Molecular mass of sequence or error if other types of characters are used
    */

    int backbone = 180;
    int result = backbone*static_cast<int>(sequence.size());
    for (auto i=0; i < static_cast<int>(sequence.size()); ++i){
        switch (sequence[i]) {
            case 'A':
            case 'a':
                result+=135;
                break;
            case 'G':
            case 'g':
                result+=151;
                break;
            case 'C':
            case 'c':
                result+=111;
                break;
            case 'T':
            case 't':
                result+=126;
                break;
            default:
                throw "Please use only A, G, C, T characters";
        }
    }
    return result;
}


bool comp (std::string a, const std::string b){
    /*
     input: 2 DNA sequences (only A, T, C, G)
     returns: true if th first one waits less than the second one
     or if it is not bigger aphabesically

     First idea with size is based on the assumption that we have only a, g, c, t
     If one is 1.2 longer than another, it is definately havier
     min: M(C) = 111, max: M(G) = 151, backbone 180 (111+180)*2*1.2>(151+180)
    */

    //if one is 1.2 longer than another
    double size_difference = (a.size()) / static_cast<double >(b.size());
    if ( (size_difference >=1.2) || (size_difference <=0.83) ){
        return (a.size()<b.size());
    }else{
        int  molar_mass_a = molar_mass(a);
        int molar_mass_b = molar_mass(b);
        int end_of_string = static_cast<int>(std::min(a.size(), b.size()));

        //if molar mass is equal, put in alphabetical order
        if (molar_mass_a == molar_mass_b){
            for(int i = 0; i < end_of_string; i++){
                char c_a, c_b;
                c_a = std::tolower(a[i]);
                c_b = std::tolower(b[i]);
                if (c_a != c_b){return c_a < c_b;}
                else if ( i == end_of_string ) {
                    return (a.size()<b.size());
                }
            }
            //if molar mass is different
        }else{
            return (molar_mass_a < molar_mass_b);
        }
        return true;
    }
}

int main(int argc, char *argv[]) {
    std::vector<std::string> v_queries;

    if (argc <= 1){ //if no arguments were given, do nothing
        return 0;

    }else {        //put each query from arguments in the v_queries
        std::string query;
        for (int i = 1; i < argc; i++) {
            query = argv[i];
            v_queries.push_back(query);
        }
    }

    //read input line by line until the empty line and store in vec_of_lines:
    std::string line;
    std::vector<std::string> vec_of_lines;
    while (getline(std::cin, line)) {
        if (line.empty()) { break; }
        vec_of_lines.push_back(line);
    }

    //sort vec_of_lines
    std::sort(vec_of_lines.begin(), vec_of_lines.end(), comp);

    //search for query with binary search
    std::string my_query;
    std::string on_left;
    int end_of_string;
    for (auto query = v_queries.begin(); query!=v_queries.end(); query++){
        auto left = std::lower_bound(vec_of_lines.begin(), vec_of_lines.end(), *query, comp);

        on_left = *(left-1);
        my_query = *query;

        //if sizes are different immidiately not found
        if (on_left.size()!= my_query.size()){
            std::cout << "not found\n";
        }else{
            //check for upper/lower case:bring both char to lower, compare one by one
            for(int i = 0; i < my_query.size(); i++){
                char c_left, c_query;
                c_left = std::tolower(on_left[i]);
                c_query = std::tolower(my_query[i]);
                if (c_left != c_query){
                    std::cout << "not found\n";
                    break;
                }else if (i== (my_query.size()-1)){
                    std::cout << (left - vec_of_lines.begin()) << "\n";
                }
            }

        }

    }
    return 0;
}
