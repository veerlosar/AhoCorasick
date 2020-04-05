#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>


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
            std::cerr << "Couldnt open the file\n";
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

    void makeTransitions(std::ofstream& infile, std::vector<int> goToVector) {

        infile << '"' << "transitions" << '"' << ":{";
        // string helps avoiding trailing commas by saving ":" occurences
        std::string str;
        for (int j = 32; j < goToVector.size(); ++j) {
            if (goToVector[j] != -1) {
                // e.g. "char" : state,
                if (str.find(" : ") != std::string::npos) {
                    infile << ",\n" << '"' << (char)j << '"' << " : " << goToVector[j];
                } else {
                    infile << "\n" << '"' << (char)j << '"' << " : " << goToVector[j];
                }
                str += " : ";
            }
        } infile << "\n},\n";
    }

    void makeOutputs(std::ofstream& infile, std::vector<std::string> output) {
        infile << '"' << "outputs" << '"' << ":[\n";

        for (int k = 0; k < output.size(); ++k) {
            if (k == output.size()-1) {
                infile << '"' << output[k] << '"' ;
            } else {infile << '"' << output[k] << '"' << " ,\n";}
        }
        infile << "\n]";
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
            // only writing the states whih reachable
            if (fail[i] != -1) {
                // saving information about the state
                f << '"' << "index" << '"' << ": " << i << ",\n";
                f << '"' << "final" << '"' << ": " << finals[i] << ",\n";
                f << '"' << "fail" << '"' << ": " << fail[i] << ",\n";
                // saving transitions
                makeTransitions(f, goToFunction[i]);

                // saving outputs of the state, if any
                if (finals[i] != 0) {
                    makeOutputs(f, output[i]);
                } else {
                    f << '"' << "outputs" << '"' << ":[\n" << "]";
                }
                if (i == goToFunction.size()-1) {
                    f << "\n";
                } else {f << ",\n";}
            }
        }
        f << "}";
        // closing the file, logging the information
        f.close();
        std::cout << "Succesfully saved model into given JSON.\n";
    }
};
