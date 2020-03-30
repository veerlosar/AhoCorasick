#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>


class FileManager {

public:

    std::string file_name;

    FileManager(std::string file) {
        file_name = file;
    }


    std::vector<std::string> getVocabulary() {
        // Docstrings here:
        // input file is supposed to have one word per line - make a warning

        std::ifstream text_in(file_name.c_str());
        if (!text_in) {
            std::cerr << "Couldnt open the file";
            exit(1);
        }

        std::string line;
        std::vector<std::string> vocabulary;

        while (text_in) {
            std::getline(text_in, line);
            vocabulary.push_back(line);
            //std::cout << line << "\n";

        }

        return vocabulary;
    };
};
//
//// int argc is the number of the arguments, passed through a command line
//// char** argv arguments themselves, e.g. path to the file
//int main(int argc, char** argv) {
//    // Step 1: Check the number of arguments, save the name of the file accordingly
//    std::string input_file;
//    std::cout << "Number of arguments " << argc << "\n";
//    if (argc == 1) {
//
//        input_file = "../file.txt";
//
//    } else if (argc == 2) {
//
//        input_file = argv[1];
//
//    } else {
//        std::cerr << "Error: invalid number of arguments";
//        exit(2);
//    }
//    std::cout << "Input file " << input_file << "\n";
//
//    //Step 2: Save the filename into a variable
//
//    // Step 3: Try to open the file
//    std::ifstream text_in(input_file.c_str());
//
//    //Step 4: Test if the file is open
//    if (!text_in) {
//        std::cerr << "Couldnt open the file";
//        exit(1);
//    }
//
//    //Step 5: Read every line of the file
//    std::string line;
//
//    while (text_in) {
//        std::getline(text_in, line);
//        std::cout << line << "\n";
//
//    }
//
//    exit(0);
//}