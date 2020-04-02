#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <FileManager.hpp>

// TODO BIG: find away to get only printable chars from UTF-8
// TODO ANOTHER BIG: clean arrays from the states, which are useless and empty
class AhoCorasick {

public:
    AhoCorasick(std::string input, std::string json) {
        inputFile = input;
        jsonFile = json;
    }

private:

    std::vector<std::string> vocabulary;
    std::string inputFile;
    std::string jsonFile;
    int numberOfStates = 1;
    std::vector<int> finals;
    std::vector<int> fail;
    std::vector<std::vector<int>> goToFunction;
    std::vector<std::vector<std::string>> output;


    int findLength() {

        int length = 0;
        for (int i = 0; i < vocabulary.size(); ++i) {
            length += vocabulary[i].size();
        }
        return length+1;
    }

    void buildGoTo() {

        // iterating over all the words in vocabulary
        for (int i = 0; i < vocabulary.size(); ++i)
        {
            std::string word = vocabulary[i];
            int fromState = 0;
            // iterating over chars in a word
            for (int j = 0; j < word.size(); ++j)
            {
                int letter = (int)static_cast<unsigned char>(word[j]);

                // assigns a state to the index of the char, if there is no state yet
                // e.g. goToFunction[0][23] will be equal to 1, creating a transition for char 23 from state 0 to the state 1
                if (goToFunction[fromState][letter] == -1) {
                    goToFunction[fromState][letter] = numberOfStates++;
                }

                // after creating a new transition, next transition will begin from the new state, e.g. 1
                fromState = goToFunction[fromState][letter];
            }
            // after iteration over all the chars of the word is finished, the last fromState becomes a final state
            finals[fromState] = 1;
            output[fromState].push_back(word); // TODO:  keep in mind that IDs are not unique
        }
        // all the characters that are not included into the vocabulary will have a transition back to 0
        for (int s = 0; s < 256; ++s) {
            if (goToFunction[0][s] == -1) {
                goToFunction[0][s] = 0;
            }
        }

    }

    void buildFailure() {
        // CREATING A FAILURE FUNCTION

        std::vector<int> queue;

        // children of the root node will fail to the root node
        for (int i = 0; i < 256; ++i) {
            if (goToFunction[0][i] != 0) {
                fail[goToFunction[0][i]] = 0;
                queue.push_back(goToFunction[0][i]);
            }
        }
        while (!queue.empty()) {
            // accessing the first element of the vector and
            // removing it from the queue
            int node = queue.front();
            queue.erase(queue.begin());
            int failState = fail[node];
            for (int i = 0; i < 256; ++i) {

                if (goToFunction[node][i] != -1) {

                    int child = goToFunction[node][i]; // e.g. child = 3
                    // childs input = i
                    while (goToFunction[failState][i] == -1) {
                        failState = fail[failState];
                    }
                    fail[child] = goToFunction[failState][i];
                    queue.push_back(child);
                    if (!output[fail[child]].empty()) {
                        output[child].insert(output[child].begin(),
                                output[fail[child]].begin(),
                                output[fail[child]].end());
                        finals[child] = 1; //
                    }
                }
            }
        }
        fail[0] = 0;
    }


public:

    std::vector<std::string> buildFunctions() {

        FileManager fm(inputFile);
        vocabulary = fm.getVocabulary();
        // finding the maximal number of states, which is the sumed length of all words in vocabulary
        int length = findLength();
        // initialising a goToFunction array with "length" rows and 256 columns
        // 256 is for all possible number of characters
        goToFunction.resize(length, std::vector<int>(256, -1));
        // array for final states
        finals.resize(length, 0);
        // array for failure function
        fail.resize(length, -1);
        // array for outputs
        output.resize(length);
        buildGoTo();
        buildFailure();
        fm.saveJSON(jsonFile, goToFunction, finals, output, fail);

        return vocabulary;
    }

};




