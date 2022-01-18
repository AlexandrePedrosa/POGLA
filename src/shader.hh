#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>

#include "test_opengl_error.hh"

class Shader {

public:

  Shader(std::string _name, GLenum _type);

  bool compile();

  std::string name;
  GLenum type;
  GLuint program;
  GLuint id;
};


class Program {
private:
  std::vector<Shader> shaders;
  // Necessary because to have lazy creation of program in OpenGL
  bool created = false; 

public:
  bool link();
  bool add_shader(std::string name, GLenum type);
  void use();

  GLuint id;
};

std::string load(const std::string &filename);
