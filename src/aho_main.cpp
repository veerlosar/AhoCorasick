#include <string>
#include <AhoCorasick.hpp>
#include <Finder.hpp>
#include <iostream>
// Lomaeva Maria, 793197
// c++ 7.5.0
// Ubuntu 18.04.4 LTS int argc, char** argv

int main(int argc, char** argv) {

    std::string trainingFile;
    std::string jsonFile;
    std::string searchFile;


    if (argc != 4) {
        std::cerr << "Invalid number of arguments.\n" << "Make sure you provided the following:\n" <<
        "<path>/<training_file>.txt\n" << "<path>/<file_to_save_model>.json\n" << "<path>/<text_to_search>.txt\n\n" <<
        "File for training should contain only one word per line.\n";
        exit(1);
    } else {

        trainingFile = argv[1];
        jsonFile = argv[2];
        searchFile = argv[3];
    }

    AhoCorasick aho(trainingFile, jsonFile);
    aho.buildFunctions();

   Finder finder(jsonFile, searchFile);
   finder.findWords();


    exit(0);
}
