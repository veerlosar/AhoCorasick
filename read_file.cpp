#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

// int argc is the number of the arguments, passed through a command line
// char** argv arguments themselves, e.g. path to the file
int main(int argc, char** argv) {
    // Step 1: Check the number of arguments, save the name of the file accordingly
    std::string input_file;
    std::cout << "Number of arguments " << argc << "\n";
    if (argc == 1) {

        input_file = "../file.txt";

    } else if (argc == 2) {

        input_file = argv[1];

    } else {
        std::cerr << "Error: invalid number of arguments";
        exit(2);
    }
    std::cout << "Input file " << input_file << "\n";

    //Step 2: Save the filename into a variable

    // Step 3: Try to open the file
    std::ifstream text_in(input_file.c_str());

    //Step 4: Test if the file is open
    if (!text_in) {
        std::cerr << "Couldnt open the file";
        exit(1);
    }

    //Step 5: Read every line of the file
    std::string line;

    while (text_in) {
        std::getline(text_in, line);
        std::cout << line << "\n";

    }

    exit(0);
}