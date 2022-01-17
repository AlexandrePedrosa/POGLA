#include <iostream>

#include "load_shader.hh"

std::string load(const std::string &filename) { //loads shader as a string from src directory
    std::string path = "src/" + filename;
    std::ifstream input_src_file(path, std::ios::in);
    std::string ligne;
    std::string file_content="";
    if (input_src_file.fail()) {
        std::cerr << "FAIL\n";
        return "";
    }
    while(getline(input_src_file, ligne)) {
        file_content = file_content + ligne + "\n";
    }
    file_content += '\0';
    input_src_file.close();
    return file_content;
}
