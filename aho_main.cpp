#include <FileManager.hpp>
#include <AhoCorasick.hpp>


// Driver program to test above
int main() {
    // TODO: start taking arguments
    std::string input_file = "../train_file.txt";
    //TODO: remove empty string at the end of the files
    FileManager fm(input_file);

    std::vector<std::string> train_words = fm.getVocabulary();
    for (int i=0; i < train_words.size(); ++i) {

        std::cout << "################" << "\n" << train_words[i] << "\n";
    }
    AhoCorasick aho(train_words);
    //TODO: lower the case of the vocabulary and search input
    aho.buildFunctions();




    return 0;
}
