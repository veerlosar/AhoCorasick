# Makefile for AhoCorasick
# Maria Lomaeva, 793197

# Defining variables
CPPCOMP = g++
TRAIN = -train
FIND = -find
DEFINES = -DNDEBUG



all: aho_main

# Receipe for aho_main
aho_main : include/AhoCorasick.hpp include/Finder.hpp include/FileManager.hpp src/aho_main.cpp Makefile
	$(CPPCOMP) -O3 -std=c++11 $(DEFINES) -o bin/aho_main -I include/ src/aho_main.cpp

	@echo AhoCorasick has been succesfully built.

# Installation
install: aho_main man
	sudo cp bin/aho_main /usr/local/bin

# HTML and LateX documentation
documentation: doc/sample_text.conf
	doxygen doc/sample_text.conf

man:
	sudo cp doc/aho_main.man.1 /usr/local/man/man1
	echo AhoCorasick has been succesfully built. \n Run ./aho_main -help for more information.

tests:	aho_main
	# 10000 german words and german text
	-bin/aho_main $(TRAIN) data/deutsch10000.txt data/FSA.json
	-bin/aho_main $(FIND) data/FSA.json data/search_deutsch.txt

# cleaning
clean:
	rm -f bin/aho_main
	rm -f aho_main
