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
        // TODO: convert all non ascii to ascii
        std::ifstream text_in(file_name.c_str());
        if (!text_in) {
            std::cerr << "Couldnt open the file";
            exit(1);
        }

        std::string line;
        std::vector<std::string> vocabulary;

        while (text_in) {

            std::getline(text_in, line);
            if (line != "") {
                vocabulary.push_back(line);
            }

        }

        return vocabulary;
    }

    void saveJSON(std::string file,
            std::vector<std::vector<int>> goToFunction,
            std::vector<int> finals,
            std::vector<std::vector<std::string>> output,
            std::vector<int> fail) {

        std::ofstream f;
        f.open(file); //TODO: rename file properly
        std::cout << "Saving AhoCorasick to a file..." << "\n";
        f << "{\n";
        for (int i = 0; i < goToFunction.size(); ++i) {

            f << "index: " << i << ",\n";
            f << "final: " << finals[i] << ",\n";
            f << "fail: " << fail[i] << ",\n";

            f << "transitions:{";
            std::string s = "";
            for (int j = 33; j < goToFunction[i].size(); ++j) {
                if (goToFunction[i][j] != -1) {
                    if (j == 34 | j == 39 ) {continue;}
                    if (s.find(" : ") != std::string::npos) {
                        f << ",\n" << (char)j << " : " << goToFunction[i][j];
                    } else {
                        f << "\n" << (char)j << " : " << goToFunction[i][j];
                    }
                    s += " : ";
                }
            } f << "\n},\n";

            if (finals[i] != 0) {
                f << "outputs:[\n";
                for (int k = 0; k < output[i].size(); ++k) {
                    if (k == output[i].size()-1) {
                        f << output[i][k];
                    } else {f << output[i][k] << " ,\n";}
                }
                f << "\n]";
            } else {
                f << "outputs:[\n" << "]";
            }
            if (i == goToFunction.size()-1) {
                f << "\n";
            } else {f << ",\n";}
        }
        f << "}";
        f.close();
        std::cout << "Succesfully saved model into given JSON.";
    }
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