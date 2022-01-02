#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include "matrix4.hh"
#include "bunny.hh"
#include "OBJloader.hh"

static const std::vector<GLfloat> wave_vertex_buffer_data {
    -0.5, 0.0, +0.5,
    +0.5, 0.0, +0.5,
    +0.5, 0.0, -0.5,
    -0.5, 0.0, -0.5
};

#define TEST_OPENGL_ERROR()                                                             \
  do {                                                                                  \
    GLenum err = glGetError();                                                          \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR: "                               \
                                      << gluErrorString(err)                            \
                                      << " file " << __FILE__                           \
                                      << " line " << __LINE__ << std::endl;             \
  } while(0)

GLuint bunny_vao_id;
GLuint wave_vao_id;
GLuint ssbo_ids[2];
GLuint bunny_prog_id[4];
GLuint wave_prog_id[4];
GLuint compute_program_id;
mygl::matrix4 model;
GLfloat anim_time;
int last_x, last_y;
bool bunny = false;
bool normals = false;
bool points = false;
bool angora = false;
bool timestop = false;

void window_resize(int width, int height) {
    //std::cout << "glViewport(0,0,"<< width << "," << height << ");TEST_OPENGL_ERROR();" << std::endl;
    glViewport(0,0,width,height);TEST_OPENGL_ERROR();
}

void display_bunny() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();

    const int height = glutGet(GLUT_WINDOW_HEIGHT); TEST_OPENGL_ERROR();
    const int width = glutGet(GLUT_WINDOW_WIDTH); TEST_OPENGL_ERROR();
    const float ratio = float(width) / float(height);

    mygl::matrix4 view = mygl::matrix4::identity();
    look_at(view, 0.0f, 2.0f, 5.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f);
    mygl::matrix4 proj = mygl::matrix4::identity();
    const float zoom = 0.015f;
    frustum(proj, -zoom * ratio, zoom * ratio, -zoom, zoom, 0.1f, 100.0f);

    glBindVertexArray(bunny_vao_id);TEST_OPENGL_ERROR();
    if (!points) {
        glUseProgram(bunny_prog_id[0]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(bunny_prog_id[0], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(bunny_prog_id[0], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr()); TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(bunny_prog_id[0], "projection_matrix"); TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr()); TEST_OPENGL_ERROR();
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else {
        glUseProgram(bunny_prog_id[2]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(bunny_prog_id[2], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(bunny_prog_id[2], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr()); TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(bunny_prog_id[2], "projection_matrix"); TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr()); TEST_OPENGL_ERROR();
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }
    if (normals) {
        glUseProgram(bunny_prog_id[1]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(bunny_prog_id[1], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(bunny_prog_id[1], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr()); TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(bunny_prog_id[1], "projection_matrix"); TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr()); TEST_OPENGL_ERROR();
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else if (angora) {
        glUseProgram(bunny_prog_id[3]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(bunny_prog_id[3], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(bunny_prog_id[3], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr()); TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(bunny_prog_id[3], "projection_matrix"); TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr()); TEST_OPENGL_ERROR();
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }

    glBindVertexArray(0);TEST_OPENGL_ERROR();
    glutSwapBuffers();TEST_OPENGL_ERROR();
}

void display_wave() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();

    const int height = glutGet(GLUT_WINDOW_HEIGHT); TEST_OPENGL_ERROR();
    const int width = glutGet(GLUT_WINDOW_WIDTH); TEST_OPENGL_ERROR();
    const float ratio = float(width) / float(height);

    mygl::matrix4 view = mygl::matrix4::identity();
    look_at(view, 0.0f, 2.0f, 5.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f);
    mygl::matrix4 proj = mygl::matrix4::identity();
    const float zoom = 0.015f;
    frustum(proj, -zoom * ratio, zoom * ratio, -zoom, zoom, 0.1f, 100.0f);

    glBindVertexArray(wave_vao_id);TEST_OPENGL_ERROR();
    if (!points) {
        glUseProgram(wave_prog_id[0]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(wave_prog_id[0], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(wave_prog_id[0], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr());TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(wave_prog_id[0], "projection_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr());TEST_OPENGL_ERROR();
        glDrawArrays(GL_PATCHES, 0, wave_vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else {
        glUseProgram(wave_prog_id[2]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(wave_prog_id[2], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(wave_prog_id[2], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr());TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(wave_prog_id[2], "projection_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr());TEST_OPENGL_ERROR();
        glDrawArrays(GL_PATCHES, 0, wave_vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }
    if (normals) {
        glUseProgram(wave_prog_id[1]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(wave_prog_id[1], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(wave_prog_id[1], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr());TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(wave_prog_id[1], "projection_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr());TEST_OPENGL_ERROR();
        glDrawArrays(GL_PATCHES, 0, wave_vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else if (angora) {
        glUseProgram(wave_prog_id[3]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(wave_prog_id[3], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(wave_prog_id[3], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr());TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(wave_prog_id[3], "projection_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr());TEST_OPENGL_ERROR();
        glDrawArrays(GL_PATCHES, 0, wave_vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }

    glBindVertexArray(0);TEST_OPENGL_ERROR();
    glutSwapBuffers();TEST_OPENGL_ERROR();
}

void init_glut(int &argc, char *argv[]) {
    glewExperimental = GL_TRUE;
    glutInit(&argc, argv);
    glutInitContextVersion(4,5);
    glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition ( 100, 100 );
    glutCreateWindow("Shader Programming");
    glutDisplayFunc(display_wave);
    glutReshapeFunc(window_resize);
}

bool init_glew() {
    if (glewInit()) {
        std::cerr << " Error while initializing glew";
        return false;
    }
    return true;
}

void init_GL() {
    glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);TEST_OPENGL_ERROR();
    //glEnable(GL_CULL_FACE);TEST_OPENGL_ERROR();
    glClearColor(0.35,0.30,0.4,1.0);TEST_OPENGL_ERROR();
}

void init_object_vbo() {
    int max_nb_vbo = 5;
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];

    GLint vertex_location = glGetAttribLocation(bunny_prog_id[0],"position");TEST_OPENGL_ERROR();
    GLint normal_smooth_location = glGetAttribLocation(bunny_prog_id[0],"normalSmooth");TEST_OPENGL_ERROR();
    GLint wave_location = glGetAttribLocation(wave_prog_id[0], "position");TEST_OPENGL_ERROR();

    glGenVertexArrays(1, &bunny_vao_id);TEST_OPENGL_ERROR();
    glGenVertexArrays(1, &wave_vao_id);TEST_OPENGL_ERROR();
    glBindVertexArray(bunny_vao_id);TEST_OPENGL_ERROR();

    if (vertex_location!=-1) nb_vbo++;
    if (normal_smooth_location!=-1) nb_vbo++;
    if (wave_location != -1) nb_vbo++;
    glGenBuffers(nb_vbo, vbo_ids);TEST_OPENGL_ERROR();
    glGenBuffers(2, ssbo_ids);

    if (vertex_location!=-1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(float), vertex_buffer_data.data(), GL_STATIC_DRAW);TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_SHADER_STORAGE_BUFFER, vertex_buffer_data.size()*sizeof(float), 0, GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        index_vbo++;
    }

    if (normal_smooth_location!=-1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, normal_smooth_buffer_data.size()*sizeof(float), normal_smooth_buffer_data.data(), GL_STATIC_DRAW);TEST_OPENGL_ERROR();
        glVertexAttribPointer(normal_smooth_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(normal_smooth_location);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_SHADER_STORAGE_BUFFER, normal_smooth_buffer_data.size()*sizeof(float), 0, GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        index_vbo++;
    }
    glBindVertexArray(wave_vao_id);TEST_OPENGL_ERROR();
    if (wave_location != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, wave_vertex_buffer_data.size() * sizeof(float), wave_vertex_buffer_data.data(),GL_STATIC_DRAW);TEST_OPENGL_ERROR();
        glVertexAttribPointer(wave_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(wave_location);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        index_vbo++;
    }
    glBindVertexArray(0);
}

std::string load(const std::string &filename) {
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

bool init_shader_compute(GLuint *program_id) {
    std::string compute_src = load("compute.shd");

    GLuint shader_id;
    GLint compile_status = GL_TRUE;
    char *compute_shd_src = (char*)std::malloc(compute_src.length()*sizeof(char));

    compute_src.copy(compute_shd_src, compute_src.length());
    shader_id = glCreateShader(GL_COMPUTE_SHADER); TEST_OPENGL_ERROR();
    glShaderSource(shader_id, 1, (const GLchar**)&(compute_shd_src), 0);TEST_OPENGL_ERROR();

    glCompileShader(shader_id);TEST_OPENGL_ERROR();
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status); TEST_OPENGL_ERROR();
    if(compile_status != GL_TRUE) {
        GLint log_size;
        char *shader_log;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_size);
        shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
        if(shader_log != 0) {
            glGetShaderInfoLog(shader_id, log_size, &log_size, shader_log);
            std::cerr << "SHADER COMPUTE: " << shader_log << std::endl;
            std::free(shader_log);
        }
        std::free(compute_shd_src);
        glDeleteShader(shader_id);
        return false;
    }
    std::free(compute_shd_src);

    GLint link_status=GL_TRUE;
    *program_id = glCreateProgram();TEST_OPENGL_ERROR();
    if (*program_id == 0) return false;
    glAttachShader(*program_id, shader_id);TEST_OPENGL_ERROR();

    glLinkProgram(*program_id);TEST_OPENGL_ERROR();
    glGetProgramiv(*program_id, GL_LINK_STATUS, &link_status);
    if (link_status!=GL_TRUE) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(*program_id, GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
        if(program_log != 0) {
            glGetProgramInfoLog(*program_id, log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }
        glDeleteProgram(*program_id);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id);TEST_OPENGL_ERROR();
        *program_id = 0;
        return false;
    }
    glUseProgram(*program_id);TEST_OPENGL_ERROR();
    return true;

}

bool init_shaders_bunny(std::string geometry, GLuint *program_id) {
    std::string vertex_src = load("vertex_bunny.shd");
    std::string geometry_src = load(geometry);
    std::string fragment_src = load("fragment.shd");

    GLuint shader_id[3];
    GLint compile_status = GL_TRUE;
    char *vertex_shd_src = (char*)std::malloc(vertex_src.length()*sizeof(char));
    char *geometry_shd_src = (char*)std::malloc(geometry_src.length()*sizeof(char));
    char *fragment_shd_src = (char*)std::malloc(fragment_src.length()*sizeof(char));

    vertex_src.copy(vertex_shd_src,vertex_src.length());
    geometry_src.copy(geometry_shd_src,geometry_src.length());
    fragment_src.copy(fragment_shd_src,fragment_src.length());

    shader_id[0] = glCreateShader(GL_VERTEX_SHADER);TEST_OPENGL_ERROR();
    shader_id[1] = glCreateShader(GL_GEOMETRY_SHADER);TEST_OPENGL_ERROR();
    shader_id[2] = glCreateShader(GL_FRAGMENT_SHADER);TEST_OPENGL_ERROR();

    glShaderSource(shader_id[0], 1, (const GLchar**)&(vertex_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[1], 1, (const GLchar**)&(geometry_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[2], 1, (const GLchar**)&(fragment_shd_src), 0);TEST_OPENGL_ERROR();

    for(int i = 0 ; i < 3 ; i++) {
        glCompileShader(shader_id[i]);TEST_OPENGL_ERROR();
        glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);
        if(compile_status != GL_TRUE) {
            GLint log_size;
            char *shader_log;
            glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
            shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
            if(shader_log != 0) {
                glGetShaderInfoLog(shader_id[i], log_size, &log_size, shader_log);
                std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
                std::free(shader_log);
            }
            std::free(vertex_shd_src);
            std::free(geometry_shd_src);
            std::free(fragment_shd_src);
            glDeleteShader(shader_id[0]);
            glDeleteShader(shader_id[1]);
            glDeleteShader(shader_id[2]);
            return false;
        }
    }
    std::free(vertex_shd_src);
    std::free(geometry_shd_src);
    std::free(fragment_shd_src);


    GLint link_status=GL_TRUE;
    *program_id = glCreateProgram();TEST_OPENGL_ERROR();
    if (program_id[0] == 0) return false;
    for(int i = 0 ; i < 3 ; i++) {
        glAttachShader(program_id[0], shader_id[i]);TEST_OPENGL_ERROR();
    }

    glLinkProgram(program_id[0]);TEST_OPENGL_ERROR();
    glGetProgramiv(program_id[0], GL_LINK_STATUS, &link_status);
    if (link_status!=GL_TRUE) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(program_id[0], GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
        if(program_log != 0) {
            glGetProgramInfoLog(program_id[0], log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }
        glDeleteProgram(program_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[1]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[2]);TEST_OPENGL_ERROR();
        program_id[0] = 0;
        return false;
    }
    glUseProgram(program_id[0]);TEST_OPENGL_ERROR();
    return true;

}

bool init_shaders_tessel(std::string geometry, GLuint *program_id) {
    std::string vertex_src = load("vertex.shd");
    std::string tessel_control_src = load("tessel_control.shd");
    std::string tessel_eval_src = load("tessel_eval.shd");
    std::string geometry_src = load(geometry);
    std::string fragment_src = load("fragment.shd");

    GLuint shader_id[5];
    GLint compile_status = GL_TRUE;
    char *vertex_shd_src = (char*)std::malloc(vertex_src.length()*sizeof(char));
    char *tessel_control_shd_src = (char*)std::malloc(tessel_control_src.length()*sizeof(char));
    char *tessel_eval_shd_src = (char*)std::malloc(tessel_eval_src.length()*sizeof(char));
    char *geometry_shd_src = (char*)std::malloc(geometry_src.length()*sizeof(char));
    char *fragment_shd_src = (char*)std::malloc(fragment_src.length()*sizeof(char));

    vertex_src.copy(vertex_shd_src,vertex_src.length());
    tessel_control_src.copy(tessel_control_shd_src, tessel_control_src.length());
    tessel_eval_src.copy(tessel_eval_shd_src, tessel_eval_src.length());
    geometry_src.copy(geometry_shd_src,geometry_src.length());
    fragment_src.copy(fragment_shd_src,fragment_src.length());

    shader_id[0] = glCreateShader(GL_VERTEX_SHADER);TEST_OPENGL_ERROR();
    shader_id[1] = glCreateShader(GL_TESS_CONTROL_SHADER);TEST_OPENGL_ERROR();
    shader_id[2] = glCreateShader(GL_TESS_EVALUATION_SHADER);TEST_OPENGL_ERROR();
    shader_id[3] = glCreateShader(GL_GEOMETRY_SHADER);TEST_OPENGL_ERROR();
    shader_id[4] = glCreateShader(GL_FRAGMENT_SHADER);TEST_OPENGL_ERROR();

    glShaderSource(shader_id[0], 1, (const GLchar**)&(vertex_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[1], 1, (const GLchar**)&(tessel_control_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[2], 1, (const GLchar**)&(tessel_eval_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[3], 1, (const GLchar**)&(geometry_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[4], 1, (const GLchar**)&(fragment_shd_src), 0);TEST_OPENGL_ERROR();

    for(int i = 0 ; i < 5 ; i++) {
        glCompileShader(shader_id[i]);TEST_OPENGL_ERROR();
        glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);
        if(compile_status != GL_TRUE) {
            GLint log_size;
            char *shader_log;
            glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
            shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
            if(shader_log != 0) {
                glGetShaderInfoLog(shader_id[i], log_size, &log_size, shader_log);
                std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
                std::free(shader_log);
            }
            std::free(vertex_shd_src);
            std::free(tessel_control_shd_src);
            std::free(tessel_eval_shd_src);
            std::free(geometry_shd_src);
            std::free(fragment_shd_src);
            glDeleteShader(shader_id[0]);
            glDeleteShader(shader_id[1]);
            glDeleteShader(shader_id[2]);
            glDeleteShader(shader_id[3]);
            glDeleteShader(shader_id[4]);
            return false;
        }
    }
    std::free(vertex_shd_src);
    std::free(tessel_control_shd_src);
    std::free(tessel_eval_shd_src);
    std::free(geometry_shd_src);
    std::free(fragment_shd_src);


    GLint link_status=GL_TRUE;
    program_id[0] = glCreateProgram();TEST_OPENGL_ERROR();
    if (program_id[0] == 0) return false;
    for(int i = 0 ; i < 5 ; i++) {
        glAttachShader(program_id[0], shader_id[i]);TEST_OPENGL_ERROR();
    }

    glLinkProgram(program_id[0]);TEST_OPENGL_ERROR();
    glGetProgramiv(program_id[0], GL_LINK_STATUS, &link_status);
    if (link_status!=GL_TRUE) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(program_id[0], GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
        if(program_log != 0) {
            glGetProgramInfoLog(program_id[0], log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }
        glDeleteProgram(program_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[1]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[2]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[3]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[4]);TEST_OPENGL_ERROR();
        program_id[0]=0;
        return false;
    }
    glUseProgram(program_id[0]);TEST_OPENGL_ERROR();
    return true;
}

void mouse_callback(int x, int y) {
    const float sensi_x = 0.1f;
    const float sensi_y = 0.1f;
    const GLfloat move_x = (float(x) - (float(last_x))) * sensi_x;
    const GLfloat move_y = (float(y) - (float(last_y))) * sensi_y;
    last_x = x;
    last_y = y;
    //glutWarpPointer(width/2, height/2); TEST_OPENGL_ERROR();
    //std::cout << "move_x = " << move_x << " move_y = " << move_y << std::endl;
    if (move_x != 0)
        rotate(model, move_x, 0.0f, 1.0f, 0.0f);
    if (move_y != 0)
        rotate(model, move_y, 1.0f, 0.0f, 0.0f);
}

void click_callback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        last_x = x;
        last_y = y;
    }
}

void keyboard_callback(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
    if (key == 'b') { // switch or reset to bunny
        init_object_vbo();
        if (!bunny)
            translate(model, 0.0f, -0.5f, 0.0f);
        bunny = true;
        glutDisplayFunc(display_bunny);
        anim_time = 0;
    } else if (key == 'w') { // switch or reset to wave
        glutDisplayFunc(display_wave);
        if (bunny)
            translate(model, 0.0f, 0.5f, 0.0f);
        bunny = false;
        anim_time = 0;
    } else if (key == 'p') { //switch to points / triangles
        points = !points;
    } else if (key == 'n') { //switch normals on/off
        if (!normals) {
            angora = false;
        }
        normals = !normals;
    } else if (key == 'a') { // switch angora on/off
        if (!angora) {
            normals = false;
        }
        angora = !angora;
    } else if (key == 't') { // timestop
        timestop = !timestop;
    }
    glutPostRedisplay();
}

void anim_bunny() {
    GLint anim_time_location;
    glUseProgram(compute_program_id);
    anim_time_location = glGetUniformLocation(compute_program_id, "anim_time");
    if (timestop)
        glUniform1f(anim_time_location, 0);
    else
        glUniform1f(anim_time_location, anim_time);
    glBindVertexArray(bunny_vao_id);TEST_OPENGL_ERROR();
    glUseProgram(compute_program_id);TEST_OPENGL_ERROR();
    glDispatchCompute(vertex_buffer_data.size() / (3 * 1024) + 1, 1, 1);TEST_OPENGL_ERROR();
    glMemoryBarrier(GL_ALL_BARRIER_BITS);TEST_OPENGL_ERROR();
    glBindVertexArray(0);TEST_OPENGL_ERROR();
}

void anim_wave() {
    GLint anim_time_location;
    for (unsigned i = 0; i < 4; i++) {
        glUseProgram(wave_prog_id[i]);
        anim_time_location = glGetUniformLocation(wave_prog_id[i], "anim_time");
        glUniform1f(anim_time_location, anim_time);
    }
}

void timer_bunny(int value) {
    (void) value;
    if (bunny)
        anim_bunny();
    else
        anim_wave();
    if (!timestop) {
        anim_time += 0.05;
        if (anim_time == 100)
            anim_time = 0;
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer_bunny, 0);
}

int main(int argc, char *argv[]) {
    init_glut(argc, argv);
    if (!init_glew())
        std::exit(-1);
    init_GL();
    if (!init_shaders_bunny("geometry_copy.shd", bunny_prog_id))
        return 0;
    if (!init_shaders_bunny("geometry_normals.shd", bunny_prog_id+1))
        return 0;
    if (!init_shaders_bunny("geometry_point.shd", bunny_prog_id+2))
        return 0;
    if (!init_shaders_bunny("geometry_angora.shd", bunny_prog_id+3))
        return 0;
    if (!init_shader_compute(&compute_program_id))
        return 0;
    if (!init_shaders_tessel("geometry_copy.shd", wave_prog_id))
        return 0;
    if (!init_shaders_tessel("geometry_normals.shd", wave_prog_id + 1))
        return 0;
    if (!init_shaders_tessel("geometry_point.shd", wave_prog_id + 2))
        return 0;
    if (!init_shaders_tessel("geometry_angora.shd", wave_prog_id + 3))
        return 0;
    init_object_vbo();
    model = mygl::matrix4::identity();
    glPatchParameteri(GL_PATCH_VERTICES, 4);TEST_OPENGL_ERROR();
    glutTimerFunc(16, timer_bunny, 0);
    glutMouseFunc(click_callback);
    glutMotionFunc(mouse_callback);
    glutDisplayFunc(display_wave);
    glutKeyboardFunc(keyboard_callback);
    glutMainLoop();
}
