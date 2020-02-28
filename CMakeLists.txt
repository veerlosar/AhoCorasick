cmake_minimum_required(VERSION 3.15)
project(AhoCorasick)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(
        AhoCorasick
        unigram_count.cpp
        read_file.cpp
        bigram_counts.cpp)
