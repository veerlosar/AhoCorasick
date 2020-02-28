#include <string>
#include <iostream>
#include <fstream>


int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Invalid number of arguments";
        exit(2);
    }

    std::string input_file = argv[1];
    std::ifstream text_in(input_file.c_str());

    if (!text_in) {
        std::cerr << "ERROR: couldnt open the file";
        exit(1);
    }

    std::string lines;
    std::string line;
    while (text_in) {
        std::getline(text_in, line);
        //std::cout << "Line: " << line << "\n";
        lines += line + " ";
        //std::cout << "Lines: " << lines << "\n";
    }

    int bigram_freq[256][256];

//    std::string text = "Peter Pepper picked a peck of pickled pepper.";

    for (int i = 0; i < sizeof(bigram_freq[0])/sizeof(int); ++i) {
        for (int j = 0; j < sizeof(bigram_freq[0])/sizeof(int); ++j) {
            bigram_freq[i][j] = 0;
        }
    }

    for (int a = 0; a < lines.length()-1; ++a) {
        std::string bigram = lines.substr(a, 2);
        //std::cout << bigram << "\n";
        //std::cout << bigram_freq[text[a]][text[a+1]] << "\n";
        ++bigram_freq[lines[a]][lines[a+1]];
        //std::cout << bigram_freq[text[a]][text[a+1]] << "\n";

    }

    for (int i = 0; i < sizeof(bigram_freq[0])/sizeof(int); ++i) {
        for (int j = 0; j < sizeof(bigram_freq[0])/sizeof(int); ++j) {
            if (bigram_freq[i][j] != 0) {
                std::cout << char(i) << char(j) << "\t" << "Frequency: " << bigram_freq[i][j] <<"\n";
            }
        }
    }

    exit(0);


    return 0;
}
