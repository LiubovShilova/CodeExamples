#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

bool adjacent_or_empty(int diagonal_value){
    //check if diagonal value between 2 adjacent ones is also 1
    //If it is, prints "Invalid Input"
    if (diagonal_value == 1){
        std::cout<<"Invalid Input";
        return true;
    }
    return false;
}

bool equal( unsigned int a, unsigned int b){
    //check if 2 raw sizes are equal
    if (a!=b){
        std::cout<<"Invalid Input";
        return false;
    }
    return true;
}

bool smaller_than_3 (unsigned int size){
    //check if size or raw/col is less than 3
    if (size<3) {
        std::cout << "Invalid input";
        return true;
    }
    else return false;
}

bool validity_check(std::vector<std::vector<int>> matr){
    /*
     * Returns true if given vector of vectors represents a matrix bigger than 3x3,
     * if it contains only 0, 1, 2 and if 1 form areas in a valid way.
     * One big function, in order to decrease the time (scan through matr once).
     */
    //to be big
    if (smaller_than_3(matr.size())) return false;

    unsigned int raw_size = matr[0].size();

    if (smaller_than_3(raw_size)) return false;
    //for each raw
    for (unsigned int i = 0; i<matr.size(); i++){
        //to be a matrix (not just vect of vect)
        if (!equal(matr[i].size(), raw_size)) return false;
        //to have valid raws of 1
        for (unsigned int j = 0; j<raw_size; j++){

            //for each 1 in matrix
            if (matr[i][j]==1){

                //calculate the neighbors = 1
                int ones = 0; //# of 1 around

                //if we have 1 on the left/right/up/down respectively
                bool left = false;
                bool right = false;
                bool up = false;
                bool down = false;

                //check all four neighbors, remember how many of them == 1 and where. Beware of angles.
                if (i!=0){
                    if (matr[i-1][j] == 1) {
                        ones++;
                        up =true;
                    }
                }

                if (i!=matr.size()-1){
                    if (matr[i+1][j] == 1) {
                        ones++;
                        down = true;
                    }
                }

                if (j!=0){
                    if (matr[i][j-1]==1) {
                        ones++;
                        left = true;
                    }
                }

                if (j!=raw_size-1){
                    if (matr[i][j+1]==1) {
                        ones++;
                        right = true;
                    }
                }

                //to have no non-closed spaces: every one should have 2 one neighbors
                if (ones != 2){
                    std::cout<<"Invalid Input";
                    return false;
                }

                //check for adjacent lines of 1 (4 ones in a square)
                if (up&&left){
                    if(adjacent_or_empty(matr[i-1][j-1])) return false;
                }

                if (up&&right){
                    if(adjacent_or_empty(matr[i-1][j+1])) return false;
                }

                if (down&&left){
                    if(adjacent_or_empty(matr[i+1][j-1])) return false;
                }

                if (down&&right) {
                    if(adjacent_or_empty(matr[i+1][j+1])) return false;
                }
            }
        }
    }
    return true;
}

bool good_direction_up(const std::vector<std::vector<int>> & matr, unsigned int i, unsigned int j,
                       int & in, int & out){
/*
 * Input: matrix, indexes i and j of 2 in matrix, number of ins and outs that was already found
 * Output: true if the direction gives the certain in/out result + updates in/out variables.
 */
    int up = 0;
    for (unsigned int i_up = 0; i_up < i; i_up++){
        if (matr[i_up][j]==1) {
            //check for consecutive ones
            if (matr[i_up+1][j] == 1){
                if(i_up<matr.size()-2){
                    if (matr[i_up+2][j]==1) return false;
                    else continue; //only 2 ones
                }
                else continue; //only 2 ones
            }
            else up++; //no consecutive ones or it's second in a pair
        }
    }

    //if the number is even, the point is out
    if (up%2 == 0) out++;
    else in++;

    return true;
}

bool good_direction_down(const std::vector<std::vector<int>> & matr, unsigned int i, unsigned int j,
                         int & in, int & out){
/*
 * Input: matrix, indexes i and j of 2 in matrix, number of ins and outs that was already found
 * Output: true if the direction gives the certain in/out result + updates in/out variables.
 */
    int down = 0;
    for (unsigned int i_down = i+1; i_down < matr.size(); i_down++){
        if (matr[i_down][j]==1) {
            //check for consecutive ones
            if (matr[i_down-1][j] == 1){
                if(i_down<matr.size()-1){
                    if (matr[i_down+1][j]==1) return false;
                    else continue; //only 2 ones
                }
                else continue; //only 2 ones
            }
            else down++; //no consecutive ones or it's second in a pair
        }
    }

    //if the number is even, the point is out
    if (down%2 == 0) out++;
    else in++;
    return true;
}

bool good_direction_left(const std::vector<std::vector<int>> & matr, unsigned int i, unsigned int j,
                         int & in, int & out){
/*
 * Input: matrix, indexes i and j of 2 in matrix, number of ins and outs that was already found
 * Output: true if the direction gives the certain in/out result + updates in/out variables.
 */
    int left = 0;
    for (unsigned int j_left = 0; j_left < j; j_left++){
        if (matr[i][j_left]==1) {
            //check for consecutive ones
            if (matr[i][j_left+1] == 1){
                if(j_left<matr[i].size()-2){
                    if (matr[i][j_left+2]==1) return false;
                    else continue; //only 2 ones
                }
                else continue; //only 2 ones
            }
            else left++; //no consecutive ones or it's second in a pair
        }
    }

    //if the number is even, the point is out
    if (left%2 == 0) out++;
    else in++;
    return true;
}

bool good_direction_right(const std::vector<std::vector<int>> & matr, unsigned int i, unsigned int j,
                          int & in, int & out){
/*
 * Input: matrix, indexes i and j of 2 in matrix, number of ins and outs that was already found
 * Output: true if the direction gives the certain in/out result + updates in/out variables.
 */
    int right = 0;
    unsigned int raw_size = matr[0].size();
    for (unsigned int j_right = j+1; j_right < raw_size; j_right++){
        if (matr[i][j_right]==1) {
            //check for consecutive ones
            if (matr[i][j_right-1] == 1){
                if(j_right<raw_size-1){
                    if (matr[i][j_right+1]==1) return false;
                    else continue; //only 2 ones
                }
                else continue; //only 2 ones
            }
            else right++; //no consecutive ones or it's second in a pair
        }
    }

    //if the number is even, the point is out
    if (right%2 == 0) out++;
    else in++;
    return true;
}


int main() {

    std::string line;
    std::vector<std::vector<int> > matr;

    //read input until getting the empty line
    while (getline(std::cin, line)) {
        if (line.empty()) { break; }

        std::vector<int> temp;
        std::string string_number;
        std::stringstream ss(line);
        while (ss >> string_number) {
            //invalid character check: not a letter + 0, 1, 2 only
            if (!(string_number >= "0" && string_number <= "3")){
                std::cout<<"Invalid Input";
                return 0;
            }
            int number = std::stoi(string_number);
            temp.push_back(number);
        }
        matr.push_back(temp);
    }

    //check for validity
    if (!validity_check(matr)) return 0;

    int out = 0;
    int in = 0;
    int unsure=0;

    //find the 2s in the matrix
    for (unsigned int i = 0; i<matr.size(); i++) {
        for (unsigned int j = 0; j < matr[i].size(); j++) {

            //for each 2 find if it's in/out
            if (matr[i][j] == 2){

                //check directions one by one. Once one of them suits, algo moves to another 2
                if (!good_direction_right(matr, i, j, in, out)){
                    if(!good_direction_left(matr, i, j, in, out)){
                        if (!good_direction_down(matr, i, j, in, out)){
                            if(!good_direction_up(matr, i, j, in, out)){
                                //none of directions suit: unsure
                                unsure++;
                            }
                        }
                    }
                }
            }
        }
    }

    //print the results
    std::cout<<"in:"<<in<<"\n";
    std::cout<<"out:"<<out<<"\n";
    std::cout<<"unsure:"<<unsure;

    return 0;
}
