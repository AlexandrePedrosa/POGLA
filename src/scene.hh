//
// Created by ajix on 05/01/2022.
//

#ifndef POGLA_SCENE_HH
#define POGLA_SCENE_HH

#include <GL/glew.h>

#include "matrix4.hh"
#include "bunny.hh"

static const std::vector<GLfloat> wave_vertex_buffer_data {
        -0.5, 0.0, +0.5,
        +0.5, 0.0, +0.5,
        +0.5, 0.0, -0.5,
        -0.5, 0.0, -0.5
};

class Scene {
public:
    Scene();
    void init_object_vbo_bunnywave();
    bool init_shaders_bunny();
    bool init_shaders_wave();


    void anim();

    void switch_bunny();
    void switch_wave();
    void switch_points();
    void switch_normals();
    void switch_angora();
    void switch_timestop();

    void rotate_scene(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

    GLuint bunny_vao_id;
    GLuint wave_vao_id;
    GLuint ssbo_ids[2];
    GLuint bunny_prog_id[4];
    GLuint wave_prog_id[4];
    GLuint compute_program_id;
    mygl::matrix4 model;
    GLfloat anim_time;

    bool bunny = false;
    bool normals = false;
    bool points = false;
    bool angora = false;
    bool timestop = false;
private:
    bool init_shader_compute(GLuint *program_id);
    bool init_shader_bunny(std::string geometry, GLuint *program_id);
    bool init_shader_tessel(std::string geometry, GLuint *program_id);
    void anim_bunny();
    void anim_wave();

};

extern Scene scene;

#endif //POGLA_SCENE_HH
