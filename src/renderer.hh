//
// Created by ajix on 17/01/2022.
//

#ifndef POGLA_RENDERER_HH
#define POGLA_RENDERER_HH


class Renderer {
public:
    Renderer();
    void init_render_elements();
    unsigned int color_FBO; // le framebuffer du premier rendu
    unsigned int color_buffer_textures[2]; // les deux textures utilisees pour le premier rendu
    unsigned int depth_buffer; // le depth buffer utilise pour le premier rendu
private:
    void init_color_FBO();
};

extern Renderer renderer;

#endif //POGLA_RENDERER_HH
