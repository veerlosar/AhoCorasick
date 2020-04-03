# Aho-Corasick Algorithm for Pattern Matching

Project is a  C++ implementation of Aho-Corasick Algorithm for Pattern Matching. More information can be found [here](https://www.researchgate.net/publication/220423622_Efficient_string_matching_An_aid_to_bibliographic_search).

## Dependencies 

Project assumes C++ compiler is installed on your system. In order to extract documentation into .html the project requires doxygen. More information about doxygen and how to install it can be found [here](http://www.doxygen.nl/index.html). On Linux simply run:
```console
$ sudo apt-get update -y
$ sudo apt-get install -y doxygen
``` 


## Extracting and building

After downloading the archive, run the following in the terminal to extract the files: 
```console
$ tar xvzf file.tar.gz
```
Navigate to the root folder of the project and execute the following: 
```console
AhoCorasick$ make
```
The project build will start and documentation will be extracted into the .html file. To see the documentation in browser run: 
```console
AhoCorasick$ <yourBrowserName> doc/html/index.html
```
For running the tests execute the following: 
```console
AhoCorasick$ make tests
```

## Running the program


Running the program in a training mode involves building go-to, failure and output functions for the finite-state machine given the vocabulary .txt file, which contains one word per line. In the end resulting model will be saved in the provided .json file. To execute the training mode the following command should be executed: 
```console
AhoCorasick$ bin/aho_main -train your_vocabulary.txt model.json
```
If the build was successful, the program will output: 
```console
AhoCorasick$ Succesfully saved model into given JSON.
```
The built machine is now ready to search the given words in the text:
```console
AhoCorasick$ bin/aho_main -find model.json text_to_search.txt
```
If the search was successful, it will output the results in the following format: 
```console
AhoCorasick$ Search file: <fileName>	Word: <found word>	Line: <line>
```

