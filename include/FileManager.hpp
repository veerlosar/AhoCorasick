#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

// Lomaeva Maria, 793197
// c++ 7.5.0
// Ubuntu 18.04.4 LTS

/** Class reads and writes the files provided from other instances.
 *  Public constructor is called once the object of the class is defined.
 *
 * @param file The name of the file with the words FSA will be trained on.
 */
class FileManager {

public:

    std::string file_name;

    // public constructor
    FileManager(std::string file) {
        file_name = file;
    }


    /** Function derives the vocabulary from the file with the given words.
     *  Error will occur if the file won't open by any reason.
     *  Function assumes file contains only one word per line.
     *
     *
     * @return A vector with the words (vocabulary).
     */
    std::vector<std::string> getVocabulary() {

        std::ifstream text_in(file_name.c_str());
        if (!text_in) {
            std::cerr << "Couldnt open the file";
            exit(1);
        }

        std::string line;
        std::vector<std::string> vocabulary;

        // word on each line is read and saved into the vector
        while (text_in) {

            std::getline(text_in, line);
            if (line != "") {
                vocabulary.push_back(line);
            }
        }
        return vocabulary;
    }

    /** Function saves built FSA into json emptyJsonFile.
     * Errors occur if system isn't able to open the file.
     *
     *
     * @param emptyJsonFile File to save FSA into.
     * @param goToFunction Vector with the saved transitions for each state.
     * @param finals Vector containing information about finality of the states.
     * @param output Vector with the outputs of each state.
     * @param fail Vector with the failure states of each state.
     *
     * @return A vector with the words (vocabulary).
     */
    void saveJSON(std::string emptyJsonFile,
            std::vector<std::vector<int>> goToFunction,
            std::vector<int> finals,
            std::vector<std::vector<std::string>> output,
            std::vector<int> fail) {

        std::ofstream f;
        f.open(emptyJsonFile); //TODO: rename emptyJsonFile properly

        // logging the information about the current process
        std::cout << "Saving AhoCorasick to a emptyJsonFile..." << "\n";
        f << "{\n";
        for (int i = 0; i < goToFunction.size(); ++i) {

            // saving information about the state
            f << "index: " << i << ",\n";
            f << "final: " << finals[i] << ",\n";
            f << "fail: " << fail[i] << ",\n";

            // saving state's transitions
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

            // saving outputs of the state, if any
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
        // closing the file, logging the information
        f.close();
        std::cout << "Succesfully saved model into given JSON.\n";
    }
};
