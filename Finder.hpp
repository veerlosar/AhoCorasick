#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//#include <FileManager.hpp>

class Finder {

public:
    Finder(std::string json, std::string search, std::vector<std::string> w) {
        jsonFile = json;
        searchFile = search;
        words = w;
    }

private:

    struct State {

        unsigned int id = -1;
        unsigned int final;
        int failure;
        unsigned int transition_id;
    };

    struct Transition {

        unsigned int from;
        char letter;
        int next;
    };

    struct Output {
        int state_id;
        std::vector<std::string> yields;
    };

    struct Line {
        int line_id;
        int max_string_id;
    };



    std::string jsonFile;
    std::string searchFile;
    std::string text;
    std::vector<std::string> words;
    std::vector<Output> outputs;
    std::vector<Transition> transitions;
    std::vector<State> states;
    std::vector<Line> lines;

    //TODO: probably move to fileManager
    void readJSON() {
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
                outputs.push_back(output);
                output.yields.resize(0);
                state.transition_id = id-1;
                states.push_back(state);
            }
        }

    }

    int findNext(int currentState, char inputChar) {

        int maxID = states[currentState].transition_id;

        while (transitions[maxID].from == currentState) {
            if (transitions[maxID].letter == inputChar) {
                return transitions[maxID].next;
            }
            maxID--;
        }
        return states[currentState].failure;
    }

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

                text += line + " ";
                f_line.line_id = line_id;
                f_line.max_string_id = len + line.size();
                lines.push_back(f_line);
                len = len + line.size() + 1;
                line_id++;
            }
        }
    }

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
    void findWords() {
        readJSON();

        cleanText();

        int current = 0;
        std::string match;
        //std::cout << "#########################\n" << text << "#########################\n";
        for (int i = 0; i < text.size(); ++i)
        {
            current = findNext(current, text[i]);
            match += text[i];
            // If match not found, move to next state
            if ((outputs[current].state_id != current)|(states[current].id != current)) {
                std::cerr << "ERROR: current state doesn't match the ID in outputs or in states";
                exit(1);
            } else if (states[current].final == 0) {
                continue;
            } else {
                for (std::string out : outputs[current].yields) {
                    if (match.find(out) != std::string::npos) {
                        int line_n = findLine(i)+1;
                        std::cout << "\n" << "Search file: " << searchFile << "\t" << "Word: " <<
                        out << "\t" << "Line: " << line_n << "\n";
                    }
                }
            }
        }
    }
};
