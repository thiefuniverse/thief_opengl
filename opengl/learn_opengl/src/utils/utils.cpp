#include "utils.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string.h>

bool ReadFile(const char *path, std::unique_ptr< char[] > &content, int &len) {
    std::ifstream f;
    f.open(path);
    if (!f.is_open()) {
        std::cout << "file read failed" << std::endl;
        return false;
    }
    std::stringstream fs;
    fs << f.rdbuf();
    f.close();
    std::string content_str = fs.str();
    char *result_ptr = new char[content_str.size() + 1];
    if (result_ptr == nullptr) {
        return false;
    }
    memcpy(result_ptr, content_str.c_str(), content_str.size());
    result_ptr[content_str.size()] = '\0';
    content.reset(result_ptr);
    return true;
}
