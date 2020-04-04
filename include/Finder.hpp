#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

// Lomaeva Maria, 793197
// c++ 7.5.0, g++ 10.0.1
// Ubuntu 18.04.4 LTS, macOS Mojave 10.14.6

/** The goal of the class is to look for the words in the provided .txt file.
 * Finder reads FSA from .json into the variable and performs the search,
 * outputting all the occurences of the words in the file along with given lines.
 * Public constructor is called when defining a class object.
 *
 *
 * @param json The file to read FSA from.
 * @param search The file to perform a search on.
 * @param w Vocabulary of the words to look for in a search file. */
class Finder {


public:
    Finder(std::string json, std::string search) {

        jsonFile = json;
        searchFile = search;
    }

private:

    /** State includes its id,
     * information, if its final or not,
     * its failure state
     * and index in transition vector */
    struct State {


        unsigned int id = -1;
        unsigned int final;
        int failure;
        unsigned int transition_id;
    };

    /** Transition includes the state transition starts from,
     * the character it outputs
     * and the destination state. */
    struct Transition {

        unsigned int from;
        char letter;
        int next;
    };

    /** State includes the id of the state
     * and the vector of words it outputs.*/
    struct Output {
        int state_id;
        std::vector<std::string> yields;
    };



    std::string jsonFile;
    std::string searchFile;
    std::vector<Output> outputs;
    std::vector<Transition> transitions;
    std::vector<State> states;


    /** Function reads FSA saved in json file into the class variables.
     * Assumes json looks like the following:
     *
     *           index: 4,
     *           final: 1,
     *           fail:  3,
     *           transitions:{
     *           t : 5
     *           },
     *           outputs:[
     *           nicht
     *           ]
     * Function results in an error if the file is corrupted or doesn't exist.
     */
    //TODO: probably move to fileManager
    void readJSON() {

        // Error if file isn't open
        std::ifstream json_in(jsonFile.c_str());
        if (!json_in) {
            std::cerr << "Couldnt open JSON file. Make sure it's not corrupted.";
            exit(1);
        }
        std::string line;

        std::string s;
        State state;
        Transition trans;
        Output output;
        int id = 0;

        // Encountered problem while reading .json:
        // spaces weren not read correctly. The first intuitive explanation would be the fact,
        // that '>>' avoids white spaces. However, the problem remained even if the json_in.get() was used.
        // Therefore, spaces (char 32) were excluded from the file.

        std::cout << "Reading FSA from .json file...\n";
        while (json_in) {
            // actions performed depending on the line
            // reading the json file between spaces (faster than char by char)
            json_in >> s;

            if (s == "," | s == "{") {continue;}
            else if (s.find("index") != std::string::npos) {
                json_in >> state.id;
                trans.from = state.id;
                output.state_id = state.id;
            } else if (s.find("final") != std::string::npos) {
                json_in >> state.final;
            } else if (s.find("fail") != std::string::npos) {
                json_in >> state.failure;
            } else if (s.find("transitions") != std::string::npos) {
                while (s != "},") {
                    json_in >> s;
                    if (s != "},") {
                        trans.letter = s[1];
                        json_in >> s;
                        json_in >> trans.next;
                        // once the block with transitions is read,
                        // transition is pushed back to the transition vector
                        transitions.push_back(trans);
                        ++id;
                        json_in >> s;
                    }
                }
            } else if (s.find("outputs") != std::string::npos) {
                while (s != "]," & s != "]") {
                    json_in >> s;
                    if (s != "]," & s != "]") {
                        output.yields.push_back(s.substr(1, s.size()-2));
                        json_in >> s;
                    }
                }
                // in the end each output and state is pushed back to its corresponding vector
                outputs.push_back(output);
                output.yields.resize(0);
                state.transition_id = id-1;
                states.push_back(state);
            }
        }
    }


    /** Function returns the next state given the current state
     * and the character. If there is no transition from the current state,
     * it will return the failure state.
     *
     * @param currentState State where the search is currently located.
     * @param inpuChar Current character of the string,
     * on which search is being performed.
     * @return The next state (integer state_id).
     */
    int findNext(int currentState, char inputChar) {

        int maxID = states[currentState].transition_id;

        while (transitions[maxID].from == currentState) {
            // return the state from goToFunction, if found
            if (transitions[maxID].letter == inputChar) {
                return transitions[maxID].next;
            }
            maxID--;
        }
        // return a failure state,
        // if transition from the current state with the current character doesn't exist
        return states[currentState].failure;
    }


    /** Function performs the search on the given line.
     *
     * @param string_id Current index in the string.
     * @return Number of the line accordingly.
     */
    std::string matchOutput(int current, std::string result, std::string line, int line_id) {

        std::string match;

        for (int i = 0; i < line.size(); ++i)
        {
            // looking for the next state
            current = findNext(current, line[i]);
            match += line[i];

            if ((outputs[current].state_id != current)|(states[current].id != current)) {
                std::cerr << "ERROR: current state doesn't match the ID in outputs or in states";
                exit(1);
            } else if (states[current].final == 0) {
                continue; // continue the search if current state doesn't yield anything
            } else {
                // try to match current word with the outputs the current state yields
                for (std::string out : outputs[current].yields) {

                    // if match found...
                    if (match.find(out) != std::string::npos) {
                        if (result.find("|"+ out + "| ") == std::string::npos) {
                            // output the result
                            std::cout << "\n" << "Search file: " << searchFile << "\t" << "Word: " <<
                                      out << "\t" << "Line: " << line_id << "\n";
                            result += "|"+ out + "| ";
                        }
                    }
                }
            }
        }
        return result;
    }

public:

    /** Function looks for the vocabulary words in the given text file.
     * It outputs the name of the file, the word and the number of the line
     * all separated by tab.
     */
    void findWords() {

        // function reads saved FSA
        readJSON();

        std::string line;
        int current = 0;
        std::string result;
        int line_id = 1;

        std::ifstream search_in(searchFile.c_str());
        if (!search_in) {
            std::cerr << "Couldnt open Search file. Make sure it's not corrupted.\n";
            exit(1);
        }
        while (search_in) {
            std::getline(search_in, line);
            if (!line.empty()) {
                result = matchOutput(current, result, line, line_id);
                line_id++;
            }
        }
        if (result.empty()) {
            std::cout << "Couldn't find any word in the given search file\n";
        }
    }
};
