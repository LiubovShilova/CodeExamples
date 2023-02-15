#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>

bool is_dna(std::string str){
    /*
     Input: a string
     Ounput: true if it has only 'A', 'G', 'C', 'T'
    */
    for(int i=0; i<str.size();i++){
        if ((str[i]!='A') && (str[i]!='G') && (str[i] != 'T') && (str[i] != 'C')){
            return false;
        }
    }
    return true;
}

std::vector<std::string> get_kmers(std::string my_seq, int k){
    /*
     Input: a string and intger k
     Return: vector of k-mers
    */
    std::vector<std::string> vector_of_kmers;
    if (my_seq.size() < k){return vector_of_kmers;}
    for(int i = 0; i<(my_seq.size()-k+1); i++){
        vector_of_kmers.push_back(my_seq.substr(i, k));
    }
    return vector_of_kmers;
}

int number_of_letter(char letter){
    //decode the letter in index of the nucleatide in alphabetical order
    switch (letter) {
        case 'A':
            return 0;
        case 'C':
            return 1;
        case 'G':
            return 2;
        case 'T':
            return 3;

    }
}

std::vector<int> spectrum(std::string my_seq, int k){
    /*
     Input: a string
     Return: the spector of k-mers
     In order to make computations faster, the function uses formula,
     which gives the position of the k-mer in the spector based on the k-mer
     (there is no hash map here)
    */
    std::vector<std::string> vector_of_kmers;
    vector_of_kmers = get_kmers(my_seq, k);
    std::vector<int> spect(std::pow(4,k),0);
    if (vector_of_kmers.empty()){return spect;}
    else{
        for (int i = 0; i<vector_of_kmers.size(); i++){
            int location = 0;
            for (int j = 0; j<vector_of_kmers[i].size(); j++){
                int letter_code = number_of_letter(vector_of_kmers[i][vector_of_kmers[i].size()-1-j]);
                location += letter_code*(std::pow(4,j));// formula to understand the position of kmer in a vector
            }
            spect[location]+=1;
        }
    }
    return spect;
}

double Eucledian(std::vector<int> v1, std::vector<int> v2){
    //calculate the eucledian distance between 2 vectors
    double distance = 0;
    for (int i = 0; i < v1.size(); i++){
        distance+=std::pow((v1[i] - v2[i]),2);
    }
    return std::sqrt(distance);
}

int Manhattan(std::vector<int> v1, std::vector<int> v2){
    //calculate the manhattan distance between 2 vectors
    int distance = 0;
    for (int i = 0; i < v1.size(); i++){
        distance+=std::abs(v1[i] - v2[i]);
    }
    return distance;
}

std::string the_most_distinct (std::vector<std::string> names,
                               std::vector<std::vector<int> > spectors,
                               std::string metric = "eucl"){
    /*
     Input: a vector of sequences  names and a vector of their spectors of the same size + eucl or man for metric
     Return: the name of the most distinct sequance in terms of spectors
    */

     int maximal_distance = 0;
     std::string name_of_string = "";

     for (int i = 0; i<spectors.size(); i++){
         int distance = 0;
         int loc_dist = 0;
         for (int j = 0; j<spectors.size();j++) {
             if (metric == "eucl") {
                 loc_dist = Eucledian(spectors[i], spectors[j]);
             } else if (metric == "man") {
                 loc_dist = Manhattan(spectors[i], spectors[j]);
             }
             distance += loc_dist;
         }

         if (distance>maximal_distance){
             maximal_distance = distance;
             name_of_string = names[i];
         }

     }
    return name_of_string;
}

int main(int argc, const char *argv[])
{
    //Check the number of arguments
    if (argc <= 3||argc>=6){
        std::cerr<<"You should enter 3 arguments";
        return -1;
    }

    //Check if k is an integer
    int k{};
    std::stringstream k_string{argv[3]};
    if (!(k_string >> k))
    {
        throw std::invalid_argument("k is not an integer");
    }

    //check if the file can be opened/not empty
    //read paths to source and sink files
    std::string source_path = argv[1];
    std::string sink_path = argv[2];
    std::ifstream source_file;
    std::ofstream sink_file;

    source_file.open(source_path);
    if (!source_file.is_open())
    {
        throw std::invalid_argument("Could not open the source file");
    }
    if(source_file.peek() == std::ifstream::traits_type::eof())
    {
        throw std::invalid_argument("The source file is empty");
    }


    //read from file
    std::string line = "";
    std::vector<std::string> vec_of_seqs; //full valid sequences
    std::vector<std::string> sequence_names; //names of sequences, starting with >
    std::string str = "";  //temporary variable to write the seq from several lines in

    //the file should start from seq name

    while (getline(source_file, line)) {
        if (line.empty()) {break;}       //read until empty string

        //if name save in sequence_names
        if (line[0] == '>')
        {
            sequence_names.push_back(line);

            //sequences that came between ">" (str) save in vec_of_lines
            if (!str.empty()){
                vec_of_seqs.push_back(str);
                str.clear();

            }

        }else{
            if (is_dna(line)){    //check for valid sequences
                str+=line;
            }else{
                std::cerr << "Some of the sequences are not DNA!" << "\n";
                return -1;
            }
        }

    }
    //for the last seq
    if (!str.empty()){
        vec_of_seqs.push_back(str);
        str.clear();
    }
    source_file.close();

    //check for two headers following each other
    if (vec_of_seqs.size()!=sequence_names.size()){
        std::cerr<<"number of sequences is not equal to number of sequence names in source file!";
        return -1;
    }

    //write the spector to the "sink" file
    sink_file.open(sink_path);
    std::vector<std::vector<int> > vector_of_spect;
    for (int i=0; i<vec_of_seqs.size(); i++){
        sink_file<<sequence_names[i]<<"\t";
        std::vector<int> spect = spectrum(vec_of_seqs[i], k);
        vector_of_spect.push_back(spect);
        for (auto i = spect.begin(); i!= spect.end(); i++ ){
            if (i == spect.end()-1){
                sink_file<<*i;
            }else{
                sink_file<<*i<<"\t";
            }

        }
        sink_file<<"\n";
    }
    sink_file.close();


    //check if we have a 4th argument (distance)
    if (argc == 5){
        std::string distance = argv[4];
        //find the most distinct sequence, print its name
        if (distance == "man"){
            std::cout<<the_most_distinct(sequence_names, vector_of_spect, "man")<<"\n";
        }
        else if (distance == "eucl"){
            std::cout<<the_most_distinct(sequence_names, vector_of_spect, "eucl")<<"\n";
        }
        else{
            //if some unfamiliar distance name is given
            std::cerr<<"The forth argument should be a man or eucl";
            return -1;
        }

    }
    
    return 0;
}
