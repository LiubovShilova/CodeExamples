#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

bool is_rna(std::string str){
    //checks if given string is rna
    for(int i=0; i<str.size();i++){
        if ((str[i]!='A') && (str[i]!='G') && (str[i] != 'U') && (str[i] != 'C')){
            return false;
        }
    }
    return true;
}

bool complementary(char a, char b){
    /*
     input:two characters representing nucleotides
     returns: true iff they are complementary (a-u or a-t and g-c)
    */
    if ((((a == 'A')||(a == 'G')) && (b== 'U'))
        || (((b == 'A')||(b == 'G')) && (a == 'U'))
        || ((b == 'C') && (a == 'G'))
        || ((b == 'G') && (a == 'C'))) {return true;}
    else {return false;}
}

std::vector<std::vector<int> > nussinov_matr(std::string some_seq, int l){
    /*
     Input: RNA sequence in upper case and minimal loop length l
     Output: nussinov matrix as discribed on the slides for the task
     */

    std::vector<std::vector<int> > matr;

    //initialize the "left side" of the matrix with 0;
    for (int i=1; i<=some_seq.size(); i++){
        std::vector<int> v2(i+1, 0);
        matr.push_back(v2);
    }

    //now fill in in diagonal manner, untill the right upper corner is reached
    //values are from the formula on slides.
    for (int a = some_seq.size()-1; a>0; a--){
        for (int i = 0; i < a; i++){
            int j = matr[i+1].size()-1;
            int Ni_j_1 = matr[i][j-1];

            int k = -1;

            //find max k
            for(int k_itr = j-2-l; k_itr >= i; k_itr--){
                if(complementary(some_seq[j-1], some_seq[k_itr])) {
                    k = k_itr;
                    break;
                }
            }

            //if nothing below is complementary
            if (k == -1){matr[i].push_back(Ni_j_1);}

            //if something is compare and push the bigger value
            else{
                int value = (matr[i][k]+matr[k+1][j-1]+1);
                int m = std::max(Ni_j_1, value);
                matr[i].push_back(m);
            }
        }
    }
    return  matr;
}

void traceback(int i, int j, int l, std::string seq,
               std::vector<std::vector<int> > matr,
               std::vector<std::string> & brackets,
               std::string& scheme){
    /*
    traverses the nussinov matrix in recursive manner.
    the positions of the bounding are marked in string scheme with brackets,
    the additional possible variants of scheme are stored in vector brackets
    */

    if (j<=i){return;}

    for (int k = i; k <= j-2-l; k ++) {

        //if from this position 2 ways are possible(the cell on the left is equal to the sum)
        if (complementary(seq[k], seq[j - 1])
            && (matr[i][j - 1] == matr[i][k] + matr[k + 1][j - 1] + 1)) {

            std::string other_scheme = scheme;

            other_scheme[k] = '(';
            other_scheme[j - 1] = ')';
            traceback(i, k, l, seq, matr, brackets, other_scheme);
            traceback(k + 1, j - 1, l, seq, matr, brackets, other_scheme);
            brackets.push_back(other_scheme);
        }
    }

    //if we came from left (nothing complementary)
    if (matr[i][j] == matr[i][j-1]){
        traceback(i,(j-1), l, seq, matr, brackets, scheme);
        return;
    }

    //if we came from bottom (complementary Sk and Sj)
    else{
        for (int k = i; k <= j-2-l; k++){
            if (complementary(seq[k], seq[j-1])
                &&(matr[i][j] == matr[i][k]+matr[k+1][j-1]+1)){
                scheme[k] = '(';
                scheme[j-1] = ')';
                traceback(i, k, l, seq, matr, brackets, scheme);
                traceback(k+1,j-1, l, seq, matr, brackets, scheme);
                break;
            }
        }
    }
}

std::vector<std::string> get_brackets(int i, int j, int l, std::string seq,
                                      std::vector<std::vector<int> > matr){
    /*
    Function gets actual values from traverse algorithm
    Input: starting position in the matrix: i and j, rna sequence and nussinov matrix
    Output: vector with strings representing all optimal structures
    */
    std::string scheme (seq.size(), '.');
    std::vector<std::string> brackets;
    traceback(i, j, l, seq, matr, brackets, scheme);
    brackets.push_back(scheme);
    return brackets;
}

void nussinov(std::string some_sec, std::vector<std::vector<int> > matr, int l){
    //Prints all optimal rna structures given sequence, nussinov matrix and min length l

    int score = matr[0][some_sec.size()];
    std::cout<<score<<"\n";
    std::vector<std::string > brackets;
    brackets = get_brackets(0, some_sec.size(), l,  some_sec, matr);

    for(auto i = brackets.begin(); i!= brackets.end(); i++){std::cout<<*i<<"\n";}               
}

int main(int argc, const char *argv[])
{
    //Check the number of arguments
    if (argc <= 1||argc>=3){
        throw std::invalid_argument("Give 1 RNA sequence as an argument");
    }

    //Check if argv[1] is an rna
    std::string rna = argv[1];
    std::transform(rna.begin(), rna.end(), rna.begin(), ::toupper); //all letters to upper case
    if (!is_rna(rna)){
        throw std::invalid_argument("The string is not RNA");
    }

    //minimal length
    int l = 2;

    //build the matrix
    std::vector<std::vector<int> > matr = nussinov_matr(rna,l);

    //print the optimal score and structures for this score
    nussinov(rna, matr, l);

    return 0;
}

