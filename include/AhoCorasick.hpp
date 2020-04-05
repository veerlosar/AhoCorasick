#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <FileManager.hpp>


/** Class builds finite-state machine with the following functions:
 * go-to (transition function), failure function and output function.
 * The automata is built based on the vocabulary derived from the file
 * provided through command line. Once machine is built, it is passed to
 * the File Manager to be saved in .json format.
 * Public constructor is called to receive and initialise parameters.
 *
 *
 * @param input String containing the name of the file.
 * @param json String with the name of the file to save FSA into.
 */
class AhoCorasick {

public:
    AhoCorasick(std::string input, std::string json) {
        inputFile = input;
        jsonFile = json;
    }

private:


    // vocabulary and necessary files
    std::vector<std::string> vocabulary;
    std::string inputFile;
    std::string jsonFile;

    // initially the number of states is 1 (only start state is there)
    int numberOfStates = 1;
    int numberOfChars = 192;

    // vectors where the functions will be saved
    std::vector<int> finals;
    std::vector<int> fail;
    std::vector<std::vector<int>> goToFunction;
    std::vector<std::vector<std::string>> output;


    /** Finds the summed length of all the words in the given file.
     * This will also define the maximum possible number of states
     * the future FSA will contain. Based on that the vectors of the functions
     * will be initialised.
     *
     * @return Summed length of all the words in vocabulary.
     */
    int findLength() {

        int length = 0;
        for (int i = 0; i < vocabulary.size(); ++i) {
            length += vocabulary[i].size();
        }
        return length+1;
    }


    /** Function builds the go-to functions. In other words,
     * it memorises the transitions each state has from with a given character.
     */
    void buildGoTo() {

        // iterating over all the words in vocabulary
        for (int i = 0; i < vocabulary.size(); ++i)
        {
            std::string word = vocabulary[i];

            // always starting from the first state
            int fromState = 0;

            // iterating over chars in a word
            for (int j = 0; j < word.size(); ++j)
            {
                // saving a char into an integer.
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
            output[fromState].push_back(word);
        }
        // all the characters that are not included into the vocabulary will have a transition back to 0
        for (int s = 0; s < numberOfChars; ++s) {
            if (goToFunction[0][s] == -1) {
                goToFunction[0][s] = 0;
            }
        }
    }

    /** Failure function will tell where to continue the search,
     * if the current state doesn't go to any state with the current character.
     */
    void buildFailure() {

        // vector to save the states yet to be processed
        std::vector<int> queue;

        // children of the root node will fail to the root node
        for (int i = 0; i < numberOfChars; ++i) {
            if (goToFunction[0][i] != 0) {
                fail[goToFunction[0][i]] = 0;
                queue.push_back(goToFunction[0][i]);
            }
        }
        while (!queue.empty()) {
            // accessing the first element of the vector and
            // removing it from the queue,
            // saving its fail state into the variable
            int node = queue.front();
            queue.erase(queue.begin());
            int failState = fail[node];

            for (int i = 0; i < numberOfChars; ++i) {

                if (goToFunction[node][i] != -1) { // if current node goes into any state, e.g. state 0 goes to 1

                    // save this state into the variable
                    int child = goToFunction[node][i]; // e.g. child = 1

                    // childs input = i
                    // until there is no transition from the node's failure state with i,
                    // send search to the next failure state
                    while (goToFunction[failState][i] == -1) {
                        failState = fail[failState];
                    }
                    // once transition is found for some state,
                    // the state is saved as a failure state of the child
                    fail[child] = goToFunction[failState][i];
                    // now child will wait for its turn in the queue to process its children
                    queue.push_back(child);

                    // the output of the found failure state will be added to the output of the child
                    if (!output[fail[child]].empty()) {
                        output[child].insert(output[child].begin(),
                                output[fail[child]].begin(),
                                output[fail[child]].end());
                        finals[child] = 1; //
                    }
                }
            }
        }
        // making 0 state fail to itself. This in case some chars are not saved/read/recognised correctly,
        // but still present in the search file. Having 0 failing into -1 could result in
        // interruption of the search in the middle of the string.
        fail[0] = 0;
    }


public:

    /** Function coordinates the build of finite-state machine.
     */
    void buildFunctions() {

        // reading provided file, saving it into vocabulary
        FileManager fm(inputFile);
        vocabulary = fm.getVocabulary();

        // finding the maximum number of states
        int length = findLength();

        // initialising a goToFunction array with "length" rows and 192 columns
        // columns = UTF characters
        goToFunction.resize(length, std::vector<int>(numberOfChars, -1));

        // array for final states
        finals.resize(length, 0);
        // array for failure function
        fail.resize(length, -1);
        // array for outputs
        output.resize(length);

        // building functions and saving FSA
        buildGoTo();
        buildFailure();
        fm.saveJSON(jsonFile, goToFunction, finals, output, fail);

        std::vector<int>().swap(finals);
        std::vector<int>().swap(fail);
        std::vector<std::vector<int>>().swap(goToFunction);
        std::vector<std::vector<std::string>>().swap(output);
    }

};




