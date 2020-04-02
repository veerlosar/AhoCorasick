#include <AhoCorasick.hpp>
#include <Finder.hpp>
#include <fstream>
// Driver program to test above
int main() {
    // TODO: start taking arguments
    std::string input_file = "../deutsch_train.txt";
    std::string json_file = "../FSA.json";
    std::string searchFile = "../search_deutsch.txt";

    AhoCorasick aho(input_file, json_file);
    std::vector<std::string> words = aho.buildFunctions();

   Finder finder(json_file, searchFile, words);
   finder.findWords();
    //finder.findWords();
//    std::vector<std::string> str;
//    std::vector<int> n;
//    std::string line;
//    std::ifstream search_in(searchFile.c_str());
//    std::string s;
//    int v;
//    while (search_in) {
//
//        search_in>>s;
//        if (s == "transitions:{") {
//            //std::getline(search_in, line);
//            search_in>>s;
//            while (s != "},") {
//                search_in>>s;
//                search_in>>v;
//                //std::getline(search_in, line);
//            }
//        }
//        search_in>>v;
//        std::getline(search_in, line);
//    }
     //Finder f;


    return 0;
}
