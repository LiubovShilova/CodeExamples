#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

bool is_rna(std::string str){
    for(int i=0; i<str.size();i++){
        if ((str[i]!='A') && (str[i]!='G') && (str[i] != 'U') && (str[i] != 'C')){
            return false;
        }
    }
    return true;
}

bool is_dna(std::string str){
    for(int i=0; i<str.size();i++){
        if ((str[i]!='A') && (str[i]!='G') && (str[i] != 'T') && (str[i] != 'C')){
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
    if ((std::toupper(a) == 'A' && (std::toupper(b) == 'U'||std::toupper(b) == 'T'))
        || (std::toupper(b) == 'A' && (std::toupper(a) == 'U'||std::toupper(a) == 'T'))
        || (std::toupper(a) == 'G' && std::toupper(b) == 'C')
        || (std::toupper(a) == 'C' && std::toupper(b) == 'G'))
    {return true;}
    else{return false;}
}

std::string hairpin(std::string seq){
    /*
     input:string of a, u, g, c, t characters
     returns: first half of the string iff it can make a hairpin,
     empty string otherwise
    */
    for (unsigned long i = 0; i < (seq.size()/2); i++){
        char base_1, base_2;
        if (seq.size()%2 == 0){
            base_1 = seq[seq.size()/2 - 1 - i];
            base_2 = seq[seq.size()/2 + i];
        }else{
            base_1 = seq[seq.size()/2 - 1 - i];
            base_2 = seq[seq.size()/2 + 1 + i];
        }
        if(!complementary(base_1,base_2)){return"";}
    }
    return seq.substr(0,(seq.size()/2));
}

int main(int argc, char *argv[]) {

    if (argc <= 1){ //if no arguments do nothing
        return 0;
    }

    //check if the file is empty
    std::ifstream filecheck(argv[1]);
    if(filecheck.peek() == std::ifstream::traits_type::eof()){
        std::cout<<"File is empty"<<"\n";
        return 0;
    }

    //read paths to source and sink files
    std::string source_path = argv[1];
    std::string sink_path = argv[2];
    std::ifstream source_file;
    std::ofstream sink_file;

    source_file.open(source_path);

    //check if it opened
    if (!source_file.is_open()){
        throw std::invalid_argument("Could not open the source file");
    }

    //read from file
    std::string line;
    std::vector<std::string> vec_of_lines; //full valid sequences
    std::vector<std::string> sequence_names; //names of sequences, starting with >
    std::string str;  //temporary variable to write the seq from several lines in
    int counter = 0; //counter for overall #lines


    while (getline(source_file, line)) {
        if (line.empty()) {break;}              //read until empty string
        counter++;
        if (line[0] == '>') {                   //if name save in sequence_names
            sequence_names.push_back(line);
            if ((!str.empty())&&(is_rna(str) || is_dna(str))){ //sequences that came between ">" (str) save in vec_of_lines
                vec_of_lines.push_back(str);
                str.clear();
            }
        }else{
            std::transform(line.begin(), line.end(), line.begin(), ::toupper); //all letters to upper case
            if (is_dna(line)||is_rna(line)){    //check for valid sequences
                str+=line;
            }else{
                std::cerr << sequence_names[sequence_names.size()-1] << "\n";
            }
        }

    }
    if (!str.empty()){
        vec_of_lines.push_back(str);
        str.clear();
    }
    source_file.close();

    //open the file for hairpins
    sink_file.open(sink_path);
    if (!sink_file.is_open()) {
        throw std::invalid_argument("Could not open the sink file");
    }

    //counters for content
    int a = 0;
    int g = 0;
    int c = 0;
    int u = 0;

    //here we analyze each line
    for (unsigned long i = 0; i<vec_of_lines.size(); i++){
        line = vec_of_lines[i];

            //Read char by char.
            for(unsigned long j = 0; j<line.size(); j++) {
                switch (line[j]) {
                    case 'A':
                        a++;
                        break;
                    case 'G':
                        g++;
                        break;
                    case 'C':
                        c++;
                        break;
                    case 'T':
                    case 'U':
                        u++;
                        break;
                }
            }
        //convert to rna
        for (unsigned long j = 0; j<line.size(); j++){
            if (line[j] == 'T'){
                line[j]='U';
            }
        }
        //write valid hairpin halves to file
        if(!hairpin(line).empty()) {
            sink_file << hairpin(line) << "\n";
        }
    }

    sink_file.close();

    //print metrics
    std::cout<<"Lines: "<<counter<<"\n";
    std::cout<<"Sequences: "<<sequence_names.size()<<"\n";
    std::cout<<"GC content: "<<static_cast<float>(g+c)/static_cast<float>(g+c+a+u)<<"\n";
    std::cout<<"A: "<<static_cast<double>(a)/static_cast<double>(g+c+a+u)<<"\n";
    std::cout<<"C: "<<static_cast<double>(c)/static_cast<double>(g+c+a+u)<<"\n";
    std::cout<<"G: "<<static_cast<double>(g)/static_cast<double>(g+c+a+u)<<"\n";
    std::cout<<"U: "<<static_cast<double>(u)/static_cast<double>(g+c+a+u)<<"\n";

    return 0;
}