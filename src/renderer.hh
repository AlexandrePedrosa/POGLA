//
// Created by ajix on 17/01/2022.
//

#ifndef POGLA_RENDERER_HH
#define POGLA_RENDERER_HH

#include "shader.hh"

static const std::vector<GLfloat> quad_vertex_buffer_data {
        -1.0, -1.0, 0.0,
        -1.0, 1.0, 0.0,
        1.0, -1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, -1.0, 0.0,
        -1.0, 1.0, 0.0
};

class Renderer {
public:
    Renderer();
    bool init_render_elements();
    void switch_bloom();
    void switch_flare();
    GLuint color_FBO; // le framebuffer du premier rendu
    GLuint effects_FBO;
    GLuint color_buffer; // les trois textures utilisees pour les pre-rendus
    GLuint effects_buffers[2];
    GLuint depth_buffer; // le depth buffer utilise pour le premier rendu

    GLuint quad_vao_id;

    Program blur_prog[2];
    Program sum_prog;
    Program flare_prog;
    Program threshold_prog;

    bool bloom = true;
    bool lensflare = true;
private:
    void init_color_FBO();
    void init_effects_FBO();
    bool init_shader_blur();
    bool init_shader_flare();
    bool init_shader_sum();
    bool init_shader_threshold();
    void init_object_vbo_quad();
};

extern Renderer renderer;

#endif //POGLA_RENDERER_HH
