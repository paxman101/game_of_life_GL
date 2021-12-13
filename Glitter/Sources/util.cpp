#include "util.h"

#include <fstream>
#include <cerrno>
#include <iostream>

char* readShader(const std::string& filename) {
    std::ifstream cur_file(filename);
    if (cur_file.fail()) {
        std::cerr << filename << ": "
            << strerror(errno) << std::endl;
    }
 
    std::string contents((std::istreambuf_iterator<char>(cur_file)), 
        std::istreambuf_iterator<char>());
    char* content_str = new char[contents.size()];
    strcpy(content_str, contents.c_str());
    
    return content_str;
}
