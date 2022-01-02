#include "OBJloader.hh"

#include <fstream>
#include <iostream>

OBJloader::OBJloader(std::string filename) {
    std::ifstream in(filename.c_str());
    if (!in) { throw(errno); }
    char linedump[256]; // for dumping comment lines
    std::string linetype;
    in >> linetype;
    while (!in.eof()) {
        if (in.peek() == '#') {
            in.getline(linedump, 256);
            continue;
        }
        if (linetype.compare("v") == 0) {
            eat_vertex(in);
        } else if (linetype.compare("vt") == 0) {
            eat_uv_coord(in);
        } else if (linetype.compare("vn") == 0) {
            eat_normal(in);
        } else if (linetype.compare("f") == 0) {
            eat_index_line(in);
        } else {
            in.getline(linedump, 256);
        }
        std::skipws(in);
        in >> linetype;
    }
    vertex_buffer.clear();
    vertex_buffer.shrink_to_fit();
    uv_buffer.clear();
    uv_buffer.shrink_to_fit();
    normal_buffer.clear();
    normal_buffer.shrink_to_fit();
}

GLfloat* OBJloader::data() {
    return complete_buffer.data();
}

std::size_t OBJloader::size() {
    return complete_buffer.size();
}

unsigned int OBJloader::n_vertex() {
    return size() / 8;
}

void OBJloader::clear() {
    complete_buffer.clear();
    complete_buffer.shrink_to_fit();
}

void OBJloader::parse_index(const std::string& vertex) {
    std::size_t start = 0;
    std::size_t end;
    std::size_t pos_index = stoull(vertex, &end) - 1;
    start = end + 1;
    std::size_t uv_index = stoull(vertex.substr(start), &end) - 1;
    start = start + end + 1;
    std::size_t normal_index = stoull(vertex.substr(start)) - 1;
    complete_buffer.push_back(vertex_buffer.at(pos_index * 3));
    complete_buffer.push_back(vertex_buffer.at(pos_index * 3 + 1));
    complete_buffer.push_back(vertex_buffer.at(pos_index * 3 + 2));
    complete_buffer.push_back(uv_buffer.at(uv_index * 2));
    complete_buffer.push_back(uv_buffer.at(uv_index * 2 + 1));
    complete_buffer.push_back(normal_buffer.at(normal_index * 3));
    complete_buffer.push_back(normal_buffer.at(normal_index * 3 + 1));
    complete_buffer.push_back(normal_buffer.at(normal_index * 3 + 2));
}

void OBJloader::eat_index_line(std::ifstream& in) {
    std::string vertex;
    in >> vertex;
    parse_index(vertex);
    in >> vertex;
    parse_index(vertex);
    in >> vertex;
    parse_index(vertex);
}

void OBJloader::eat_vertex(std::ifstream& in) {
    float current;
    in >> current;
    vertex_buffer.push_back(current);
    in >> current;
    vertex_buffer.push_back(current);
    in >> current;
    vertex_buffer.push_back(current);
}

void OBJloader::eat_uv_coord(std::ifstream& in) {
    float current;
    in >> current;
    uv_buffer.push_back(current);
    in >> current;
    uv_buffer.push_back(current);
}

void OBJloader::eat_normal(std::ifstream& in) {
    float current;
    in >> current;
    normal_buffer.push_back(-current);
    in >> current;
    normal_buffer.push_back(-current);
    in >> current;
    normal_buffer.push_back(-current);
}
