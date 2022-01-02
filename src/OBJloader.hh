#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <vector>
#include <GL/glew.h>

class OBJloader
{
public:
    explicit OBJloader(std::string filename);
    GLfloat* data();
    std::size_t size();
    void clear();
    unsigned int n_vertex();
private:
    void eat_vertex(std::ifstream& in);
    void eat_uv_coord(std::ifstream& in);
    void eat_normal(std::ifstream& in);
    void eat_index_line(std::ifstream& in);
    void parse_index(const std::string& vertex);
    std::vector<GLfloat> complete_buffer;
    std::vector<GLfloat> vertex_buffer;
    std::vector<GLfloat> normal_buffer;
    std::vector<GLfloat> uv_buffer;
};

#endif
