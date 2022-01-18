#include <fstream>
#include "shader.hh"

std::string load(const std::string &filename) {
  std::ifstream input_src_file("src/"+filename, std::ios::in);
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

Shader::Shader(std::string _name, GLenum _type) {
  name = _name;
  type = _type;
}

bool Shader::compile() {
    std::string shader_src = load(name);
    char *src = (char*)std::malloc(shader_src.length());
    shader_src.copy(src, shader_src.length());

    id = glCreateShader(type);TEST_OPENGL_ERROR();
    glShaderSource(id, 1, (const GLchar**)&src, 0);TEST_OPENGL_ERROR();
    glCompileShader(id);TEST_OPENGL_ERROR();

    GLint compile_status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);

    if (compile_status != GL_TRUE) {
      GLint log_size;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_size);
      char *shader_log = (char*)std::malloc(log_size+1);
      if (shader_log != nullptr) {
        glGetShaderInfoLog(id, log_size, &log_size, shader_log);
        std::cerr << "SHADER " << name << ": " << shader_log << std::endl;
        std::cerr << shader_src << std::endl;
        std::free(shader_log);
      }

      std::free(src);
      glDeleteShader(id);
      return false;
    }

    return true;
}

bool Program::link() {
  for (const Shader& shader : shaders) {
    glAttachShader(id, shader.id);TEST_OPENGL_ERROR();
  }

  GLint link_status;
  glLinkProgram(id);TEST_OPENGL_ERROR();
  glGetProgramiv(id, GL_LINK_STATUS, &link_status);

  if (link_status != GL_TRUE) {
    GLint log_size;
    char *program_log;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_size);
    program_log = (char*)std::malloc(log_size+1);
    if(program_log != nullptr) {
      glGetProgramInfoLog(id, log_size, &log_size, program_log);
      std::cerr << "Program " << program_log << std::endl;
      std::free(program_log);
    }
    glDeleteProgram(id);TEST_OPENGL_ERROR();
    for (const Shader& shader : shaders) {
        glDeleteShader(shader.id);TEST_OPENGL_ERROR();
    }
    return false;
  }

  return true;
}

bool Program::add_shader(std::string name, GLenum type) {
  if (!created){
    id = glCreateProgram();TEST_OPENGL_ERROR();
    created = true;
  }

  Shader s = Shader(name, type);
  s.program = id;
  if (!s.compile())
    return false;

  shaders.push_back(s);
  return true;
}

void Program::use(){
  glUseProgram(id);TEST_OPENGL_ERROR();
}
