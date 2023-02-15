#include "cxxopts.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <regex>
#include <filesystem>
#include <unordered_set>

//function to trim the string
static inline void trim(std::string &some_str) {
    some_str.erase(some_str.find_last_not_of(" \n\r")+1);
}

std::vector<std::string> read_in_v(const std::string& file_path, bool need_trim){
    /*
     Input: file path and bool variable (true if lines need to be trimmed from \n or \r
     Returns: vector of lines from the file (read untill empty string)
    */
    std::vector<std::string> v;
    std::ifstream file;
    file.open(file_path);
    if (!file.is_open())
    {
        throw std::invalid_argument("Could not open the pattern file\n");
    }
    if(file.peek() == std::ifstream::traits_type::eof())
    {
        throw std::invalid_argument("The pattern file is empty\n");
    }
    std::string line;
    while (getline(file, line)) {
        if (line.empty()) { break; }
        if (need_trim) trim(line);
        v.push_back(line);
    }
    file.close();
    return v;
}

//func: returns true if a matches b in upper case
bool equal_upper_case(char a, char b) { return(toupper(a) == b);}

bool find_exact_i(std::string s, const std::string pattern) {
    /*
     Input: string and a pattern
     Returns true iff pattern can be found in the string
     */

    std::string upper_pattern;
    transform(pattern.begin(), pattern.end(), back_inserter(upper_pattern), toupper);

    std::string::const_iterator it  = search(s.begin(), s.end(), upper_pattern.begin(),
                                             upper_pattern.end(), equal_upper_case);
    if (it!=s.end()) return true;
    else return false;
}

void Directory(const std::string& file_path, std::vector<std::string> &v){
    std::error_code err;
    for (const auto & entry : std::filesystem::directory_iterator(file_path)){
        if (std::filesystem::is_directory(entry.path(), err)){
            Directory(entry.path(), v);
        }
        else{
            v.push_back(entry.path());
        }
    }
    if (err) {
        throw std::logic_error("cannot check if file is a directory\n");
    }
}


void print_for_file(const std::string& file_name, std::vector<int> line_numb, std::vector<std::string> lines,
                    bool n, bool many_files){
    /*
     Iput: name of the file, vector of line numbers where grep occured, vector of lines where grep occured,
     boolean variables n (if we need to print the number of line), many_files(if we need to print file names)
     Prints: file, line number if needed and the line as in grep.
     */
    if(!lines.empty()){
        if (many_files){
            if (n){
                for (unsigned long int i = 0; i<line_numb.size();i++){
                    std::cout<<file_name<<":"<<line_numb[i]<<":"<<lines[i]<<"\n";
                }
            }
            else{
                for (std::string line:lines){
                    std::cout<<file_name<<":"<<line<<"\n";
                }
            }
        }
        else{
            if (n){
                for (unsigned long int i = 0; i<line_numb.size();i++){
                    std::cout<<line_numb[i]<<":"<<lines[i]<<"\n";
                }
            }
            else{
                for (std::string line:lines){
                    std::cout<<line<<"\n";
                }
            }
        }
    }

}

void remove_duplicates(std::vector<std::string> &lines, std::vector<int> &numb){
    /*
     Removes duplicates in vector of strings by creating a set and keeping unique values there.
     */
    auto it_lines = lines.begin();
    auto it_numb = numb.begin();
    std::unordered_set<std::string> s;

    for (unsigned int i = 0; i<lines.size(); i++){
        if (s.insert(lines[i]).second){
            *it_lines++ = lines[i];
            *it_numb++ = numb[i];
        }
    }
    lines.erase(it_lines, lines.end());
    numb.erase(it_numb, numb.end());
}

int main(int argc, char* argv[]) {

    //check for given arguments
    if (argc<3){ throw std::invalid_argument("Please provide the pattern and the files to look in\n"); }

    int pattern_index = 0;
    for (int i=1; i<argc; i++){
        if((std::string(argv[i]) != "-i")&& (std::string(argv[i])!="-F") && (std::string(argv[i])!="-f")
           && (std::string(argv[i])!="-v") && (std::string(argv[i])!="-n") && (std::string(argv[i])!="-R")){

            pattern_index = i;
            break;
        }
    }

    //set options
    cxxopts::Options options("MyProgram", "One line description of MyProgram");
    options.add_options()
            ("i", "case insensitive", cxxopts::value<bool>())
            ("F", "pattern is a fixed-string")
            ("f", "read pattern(s) from the filename")
            ("v", "non-matching lines")
            ("n", "add line number")
            ("R", "look through the directory")
            ("input", "input files", cxxopts::value<std::vector<std::string>>());

    options.parse_positional({"input"});
    auto result = options.parse(argc, argv);

    //create a vector for files and for directories
    std::vector<std::string> vec_of_files;
    std::vector<std::string> vec_of_dirs;

    //read files to look in into a vector vec_of_files
    for (int i = pattern_index+1; i<argc; i++){
        std::ifstream infile(argv[i]);
        std::filesystem::path path(argv[i]);
        std::error_code err;
        if (std::filesystem::is_directory(path, err))
        {
            if(result["R"].as<bool>()){
                std::vector<std::string> in_dir;
                Directory(path, in_dir);
                for(std::string el:in_dir) {
                    vec_of_files.push_back(el);
                }
            }
            else {
                vec_of_dirs.push_back(argv[i]);}
        }
        if (err) {
            std::cerr << "Cannot check if "<< argv[i] << " is directory\n" << err.message();
            return -1;
        }

        if (std::filesystem::is_regular_file(path, err))
        {
            vec_of_files.push_back(argv[i]);
        }
        if (err) {
            std::cerr << "Cannot check if "<< argv[i] << " is regular file\n" << err.message();
            return -1;
        }
    }

    std::vector<std::string> vec_of_patterns;

    //if we reed patterns from file
    if(result["f"].as<bool>()){
        vec_of_patterns = read_in_v(argv[pattern_index], true);
    }
    else{
        vec_of_patterns.push_back(std::string(argv[pattern_index]));
    }

    //for each file (which we are parsing)
    for (const std::string& file_path:vec_of_files){

        //write lines into a vector
        std::vector<std::string> vec_of_lines = read_in_v(file_path, false);

        int counter = 0; //to count the lines

        std::vector<int> lines_numb; //the numbers of lines store here
        std::vector<std::string> lines; //the lines store here

        //for each pattern
        for (const std::string& pat:vec_of_patterns){
            //for each line in a file
            for(const std::string& line:vec_of_lines){

                counter++;

                //if we need a match
                if(!result["v"].as<bool>()){

                    //if we need exact match
                    if (result["F"].as<bool>()){

                        //case insensitive
                        if (result["i"].as<bool>()){
                            if (find_exact_i(line, pat) ) {
                                lines.push_back(line);
                                lines_numb.push_back(counter);
                            }
                        }

                            //case sensitive
                        else {
                            if (line.find(pat) != std::string::npos) {
                                lines.push_back(line);
                                lines_numb.push_back(counter);
                            }
                        }
                    }

                        //if we just need regex comparison
                    else{

                        //case insensitive
                        if (result["i"].as<bool>()){
                            std::regex reg(pat, std::regex_constants::icase);
                            if (std::regex_search (line,reg)) {
                                lines.push_back(line);
                                lines_numb.push_back(counter);
                            }
                        }
                            //case sensitive
                        else{
                            std::regex reg(pat);
                            if (std::regex_search (line,reg)) {
                                lines.push_back(line);
                                lines_numb.push_back(counter);
                            }
                        }
                    }
                }

                    //if we need a mismatch (we have -v)
                else {

                    //if we need exact mismatch
                    if (result["F"].as<bool>()){

                        //case insensitive
                        if (result["i"].as<bool>()){
                            if (!find_exact_i(line, pat) ) {
                                lines.push_back(line);
                                lines_numb.push_back(counter);
                            }
                        }

                            //case sensitive
                        else {
                            if (line.find(pat) == std::string::npos) {
                                lines.push_back(line);
                                lines_numb.push_back(counter);
                            }
                        }
                    }

                        //if we just need regex comparison (mismatch)
                    else{

                        //case insensitive
                        if (result["i"].as<bool>()){
                            std::regex reg(pat, std::regex_constants::icase);
                            if (!std::regex_search (line,reg)) {
                                lines.push_back(line);
                                lines_numb.push_back(counter);
                            }
                        }
                            //case sensitive
                        else{
                            std::regex reg(pat);
                            if (!std::regex_search (line,reg)) {
                                lines.push_back(line);
                                lines_numb.push_back(counter);
                            }
                        }
                    }
                }
            }
        }
        //if we read patterns from file
        if (result["f"].as<bool>()){
            remove_duplicates(lines, lines_numb);
        }
        //print
        print_for_file(file_path, lines_numb, lines, result["n"].as<bool>(), (argc>(pattern_index+2)));
    }

    //if we have directories, but don't have "-R"
    for (const std::string& el:vec_of_dirs){
        std::cout<<"grep: " << el << ": Is a directory\n";
    }

    return 0;
}