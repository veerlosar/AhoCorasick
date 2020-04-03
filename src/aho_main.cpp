#include <string>
#include <AhoCorasick.hpp>
#include <Finder.hpp>
#include <iostream>
// Lomaeva Maria, 793197
// c++ 7.5.0
// Ubuntu 18.04.4 LTS int argc, char** argv

int main(int argc, char** argv) {

 // defining variables for provided arguments
    std::string trainingFile;
    std::string jsonFile;
    std::string searchFile;

    // If number of arguments is not 4: ERROR
    if (argc != 4) {
        std::cerr << "Invalid number of arguments.\n" << "Make sure you provided the following:\n" <<
                  "<path>/<training_file>.txt\n" << "<path>/<file_to_save_model>.json\n\n" <<
                  "File for training should contain only one word per line.\n";
        exit(1);
    } else {

        // if the number of arguments is correct and the mode is -train,
        if (std::string(argv[1]) == "-train") {

            // call AhoCorasick to build the functions and save FSA
            trainingFile = argv[2];
            jsonFile = argv[3];
            AhoCorasick aho(trainingFile, jsonFile);
            aho.buildFunctions();

        } else if (std::string(argv[1]) == "-find") {
           // if the mode is -find, find learnt words in the text file
            jsonFile = argv[2];
            searchFile = argv[3];
            Finder finder(jsonFile, searchFile);
            finder.findWords();

        } else {
            // if the first argument is none of the two: ERROR
            std::cerr << "Invalid first argument.\n" << "Make sure it's either -train or -find\n";
            exit(2);
        }
    }

    exit(0);
}
