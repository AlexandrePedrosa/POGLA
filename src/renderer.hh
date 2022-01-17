//
// Created by ajix on 17/01/2022.
//

#ifndef POGLA_RENDERER_HH
#define POGLA_RENDERER_HH


class Renderer {
public:
    Renderer();
    bool init_render_elements();
    void switch_bloom();
    unsigned int color_FBO; // le framebuffer du premier rendu
    unsigned int color_buffer_textures[3]; // les trois textures utilisees pour les pre-rendus
    unsigned int depth_buffer; // le depth buffer utilise pour le premier rendu
    GLuint blur_prog_id[2];
    GLuint sum_prog_id;
    GLuint flare_prog_id;
    bool bloom = true;
    bool lensflare = true;
private:
    void init_color_FBO();
    bool init_shader_blur();
    bool init_shader_flare();
    bool init_shader_sum();
};

extern Renderer renderer;

#endif //POGLA_RENDERER_HH
