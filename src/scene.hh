//
// Created by ajix on 05/01/2022.
//

#ifndef POGLA_SCENE_HH
#define POGLA_SCENE_HH

#include <GL/glew.h>

#include "matrix4.hh"
#include "bunny.hh"
#include "skybox.hh"

static const std::vector<GLfloat> quad_vertex_buffer_data {
        -0.5, 0.0, +0.5,
        +0.5, 0.0, +0.5,
        +0.5, 0.0, -0.5,
        -0.5, 0.0, -0.5
};

class Scene {
public:
    Scene();
    bool init_scene_elements();


    void anim();

    void switch_bunny();
    void switch_wave();
    void switch_points();
    void switch_normals();
    void switch_angora();
    void switch_timestop();

    void yaw_camera(GLfloat angle);
    void pitch_camera(GLfloat angle);
    mygl::matrix4 compute_view_matrix();

    GLuint bunny_vao_id;
    GLuint wave_vao_id;
    GLuint skybox_vao_id;

    GLuint ssbo_ids[2];
    GLuint bunny_prog_id[4];
    GLuint wave_prog_id[4];
    GLuint skybox_prog_id;
    GLuint compute_program_id;

    mygl::matrix4 model;
    mygl::matrix4 view;
    GLfloat anim_time;
    GLuint cubemap_tex_id;
    GLuint threshold_tex_id;

    mygl::point3 camera_pos = mygl::point3(0.0f, 1.0f, 5.0f);
    GLfloat camera_angle;
    GLfloat camera_pitch;

    mygl::point3 camera_vector(int camdir);
    void move_camera(GLfloat l, int camdir);

    bool bunny = true;
    bool normals = false;
    bool points = false;
    bool angora = false;
    bool timestop = true;
private:
    bool init_shader_compute(GLuint *program_id);
    bool init_shader_bunny(std::string geometry, GLuint *program_id);
    bool init_shader_tessel(std::string geometry, GLuint *program_id);
    bool init_shader_skybox(GLuint *program_id);
    bool init_cubemap_texture();
    bool init_threshold_texture();
    void init_object_vbo_bunnywave();
    void init_object_vbo_skybox();
    bool init_shaders_bunny();
    bool init_shaders_wave();
    void anim_bunny();
    void anim_wave();

};

enum camera_direction{
    CAMERA_FORWARD,
    CAMERA_BACKWARD,
    CAMERA_LEFT,
    CAMERA_RIGHT
};

extern Scene scene;

#endif //POGLA_SCENE_HH
