#include "patterns.h"

#include <fstream>
#include <cerrno>
#include <cstring>
#include <iostream>

std::vector<std::pair<int, int>> ReadPatternFile(const std::string& file_name) {
    std::ifstream cur_file(file_name);
    if (cur_file.fail()) {
        std::cerr << file_name << ": "
            << strerror(errno) << std::endl;
    }
    char dummy_char;
    std::vector<std::pair<int, int>> outVec;
    while (true) {
        std::pair<int, int> current_pair;
        if (cur_file.eof()) break;
        // read {
        cur_file >> dummy_char;
        // read x coord
        cur_file >> current_pair.first;
        // read ,
        cur_file >> dummy_char;
        // read y coord
        cur_file >> current_pair.second;
        // read }
        cur_file >> dummy_char;
        if (current_pair.first != 0 || current_pair.second != 0 || outVec.size() == 0)
        outVec.push_back(current_pair);
    }
    return outVec;
}
