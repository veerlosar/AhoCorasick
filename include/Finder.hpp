#include <string>
#include <iostream>
#include <fstream>
#include <vector>

// Lomaeva Maria, 793197
// c++ 7.5.0
// Ubuntu 18.04.4 LTS

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
    Finder(std::string json, std::string search) { //, std::vector<std::string> w

        jsonFile = json;
        searchFile = search;
        //words = w;
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

    /** Struct Line aligns number of line in the file with its string indices.
     * Contains the number of line and maximal string index it contains.*/
    struct Line {
        int line_id;
        int max_string_id;
    };


    std::string jsonFile;
    std::string searchFile;
    std::string text;
    //std::vector<std::string> words;
    std::vector<Output> outputs;
    std::vector<Transition> transitions;
    std::vector<State> states;
    std::vector<Line> lines;


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

        while (json_in) {
            // actions performed depending on the line
            json_in >> s;
            if (s == "index:") {
                json_in >> state.id;
                trans.from = state.id;
                output.state_id = state.id;
            } else if (s == "final:") {
                json_in >> state.final;
            } else if (s == "fail:") {
                json_in >> state.failure;
            } else if (s == "transitions:{") {
                while (s != "},") {
                    json_in >> s;
                    if (s != "},") {
                        trans.letter = s[0];
                        json_in >> s;
                        json_in >> trans.next;
                        // once the block with transitions is read,
                        // transition is pushed back to the transition vector
                        transitions.push_back(trans);
                        ++id;
                        json_in >> s;
                    }
                }
            } else if (s == "outputs:[") {
                while (s != "]," & s != "]") {
                    json_in >> s;
                    if (s != "]," & s != "]") {
                        output.yields.push_back(s);
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


    /** Function reads the search file line by line,
     * memorising the line number and the length the text reaches on it.
     * Function results in an error if file is corrupt or doesn't exist.
     */
    void cleanText() {
        std::ifstream search_in(searchFile.c_str());
        if (!search_in) {
            std::cerr << "Couldnt open Search file. Make sure it's not corrupted.";
            exit(1);
        }
        std::string line;
        text = "";
        Line f_line;
        int line_id = 0;

        int len = 0;

        while (search_in) {
            std::getline(search_in, line);
            if (line != "") {

                // line is added to the current text
                text += line + " ";
                // line number is saved
                f_line.line_id = line_id;
                // the current length of the current string
                f_line.max_string_id = text.size()-1;  //len + line.size();
                lines.push_back(f_line); //len = len + line.size() + 1;
                line_id++;
            }
        }
    }

    /** Function looks for the number of line given the
     * current string index. Error occurs if there is no
     * line containing the current index of the string.
     *
     * @param string_id Current index in the string.
     * @return Number of the line accordingly.
     */
    int findLine(int string_id) {

        for (int i = 0; i < lines.size(); ++i) {
            if (string_id <= lines[i].max_string_id) {
                return lines[i].line_id;
            }
        }
        std::cerr << "String ID was too big for the given number of lines";
        exit(2);
    }

public:

    /** Function looks for the vocabulary words in the given text file.
     * It outputs the name of the file, the word and the number of the line
     * all separated by tab.
     */
    void findWords() {

        // function reads saved FSA
        readJSON();

        // memorises information about the lines in the file
        cleanText();

        int current = 0;
        std::string match;
        // starts the search on the file
        for (int i = 0; i < text.size(); ++i)
        {
            // looking for the next state
            current = findNext(current, text[i]);
            match += text[i];

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
                        int line_n = findLine(i)+1;
                        // TODO: output only the first occurences
                        //TODO: find a way to tell if program didnt find any matches in the text
                        // output the result
                        std::cout << "\n" << "Search file: " << searchFile << "\t" << "Word: " <<
                        out << "\t" << "Line: " << line_n << "\n";
                    }
                }
            }
        }
    }
};
