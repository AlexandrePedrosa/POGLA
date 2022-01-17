//
// Created by ajix on 17/01/2022.
//

#ifndef POGLA_RENDERER_HH
#define POGLA_RENDERER_HH


class Renderer {
public:
    Renderer();
    bool init_render_elements();
    unsigned int color_FBO; // le framebuffer du premier rendu
    unsigned int color_buffer_textures[3]; // les trois textures utilisees pour les pre-rendus
    unsigned int depth_buffer; // le depth buffer utilise pour le premier rendu
    GLuint blur_prog_id[2];
private:
    void init_color_FBO();
    bool init_shader_blur(GLuint *prog_id);
};

extern Renderer renderer;

#endif //POGLA_RENDERER_HH
