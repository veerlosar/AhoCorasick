#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>


class FileManager;

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
        // initialising the files
        jsonFile = json;
        searchFile = search;
    }

private:

    /** State includes its id,
     * information, if its final or not,
     * its failure state
     * and index in transition vector */
    struct State {


        int id = -1;
        unsigned int final;
        int failure;
        int transition_id;
    };

    /** Transition includes the state transition starts from,
     * the character it outputs
     * and the destination state. */
    struct Transition {

        int from;
        char letter;
        int next;
    };

    /** State includes the id of the state
     * and the vector of words it outputs.*/
    struct Output {
        int state_id;
        std::vector<std::string> yields;
    };

    // necessary files
    std::string jsonFile;
    std::string searchFile;
    // arrays to read FSA into
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
    void readJSON() {

        // Error if file isn't open
        std::ifstream json_in(jsonFile.c_str());
        if (!json_in) {
            std::cerr << "Couldnt open JSON file. Make sure it's not corrupted.\n";
            exit(1);
        }

        std::string s;
        State state;
        Transition trans;
        Output output;
        int id = 0;
        //transitions.resize(1000000, trans);

        std::cout << "Reading FSA from .json file...\n";
        while (json_in) {
            // reading a file line by line and performing action accordingly
            std::getline(json_in, s);

            if (s == "," | s == "{") {continue;}

            // case for state index
            else if (s.find("index") != std::string::npos) {
                state.id = std::stoi(s.substr(s.find(':')+2, s.size()));
                trans.from = state.id;
                output.state_id = state.id;

              // case for finality
            } else if (s.find("final") != std::string::npos) {
                state.final = s[s.size()-2]-'0';
                // case for failure state
            } else if (s.find("fail") != std::string::npos) {
                state.failure = std::stoi(s.substr(s.find(':')+2, s.size()));
                // case for transitions
            } else if (s.find("transitions") != std::string::npos) {
                std::getline(json_in, s);
                while (s != "},") {
                    trans.letter = s[1];
                    trans.next = std::stoi(s.substr(s.find(':', 2)+2, s.size()));
                    // once the block with transitions is read,
                    // transition is pushed back to the transition vector
                    transitions.push_back(trans);
                    ++id;
                    std::getline(json_in, s);
                    }
                // case for outputs
            } else if (s.find("outputs") != std::string::npos) {
                std::getline(json_in, s);
                while (s != "]," & s != "]") {
                    output.yields.push_back(s.substr(1, s.find('"', 1)-1));
                    std::getline(json_in, s);
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

        // index in transition
        int maxID = states[currentState].transition_id;

        while ((transitions[maxID].from == currentState) & (maxID >= 0)) {

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

        std::string processed;

        for (int i = 0; i < line.size(); ++i)
        {
            // looking for the next state
            current = findNext(current, line[i]);
            // processed is the part of the string processed so far
            processed += line[i];

            // Error will occur if current state by some reason doesn't match its id in outputs or states vector
            if ((outputs[current].state_id != current)|(states[current].id != current)) {
                std::cerr << "ERROR: current state doesn't processed the ID in outputs or in states";
                exit(1);
            } else if (states[current].final == 0) {
                continue; // continue the search if current state doesn't yield anything
            } else {
                // try to match current word with the outputs the current state yields
                for (std::string out : outputs[current].yields) {

                    // if processed found...
                    if (processed.find(out) != std::string::npos) {
                        if (result.find("|"+ out + "| ") == std::string::npos) {
                            // output the result if it wasnt outputted already
                            std::cout << "\n" << "Search file: " << searchFile << "\t" << "Word: " <<
                                      out << "\t" << "Line: " << line_id << "\n";
                            // saving the output. "|" since out can always be a part of a bigger word
                            result += "|"+ out + "| ";
                        }
                    }
                }
            }
        }
        processed.clear();
        processed.shrink_to_fit();
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
        std::cout << "Looking for words...\n";
        // reading the file line by line and passing it to another function
        while (search_in) {
            std::getline(search_in, line);
            if (!line.empty()) {
                result = matchOutput(current, result, line, line_id);
                line_id++;
            }
        }
        // if nothing was found, inform
        if (result.empty()) {
            std::cout << "Couldn't find any word in the given search file\n";
        }
    }
};
