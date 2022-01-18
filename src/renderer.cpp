//
// Created by ajix on 17/01/2022.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "renderer.hh"
#include "test_opengl_error.hh"

Renderer::Renderer() {}

bool Renderer::init_render_elements() {

    init_color_FBO();
    if (!init_shader_blur() ||
        !init_shader_sum() ||
        !init_shader_flare())
        return false;
    init_object_vbo_quad();
    return true;
}

void Renderer::init_object_vbo_quad() {
    GLuint vbo_id;

    GLint vertex_location = glGetAttribLocation(flare_prog.id, "position");TEST_OPENGL_ERROR();
    glGenVertexArrays(1, &quad_vao_id);TEST_OPENGL_ERROR();
    glBindVertexArray(quad_vao_id);TEST_OPENGL_ERROR();
    glGenBuffers(1, &vbo_id);TEST_OPENGL_ERROR();
    if (vertex_location!=-1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, quad_vertex_buffer_data.size()*sizeof(float),
                     quad_vertex_buffer_data.data(), GL_STATIC_DRAW);TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    glBindVertexArray(0);
}

void Renderer::init_color_FBO() {
    /*
    ** On va rendre les couleurs dans 2 color attachments un pour le rendu de base et l'autre ou on ne conservera que
    ** les parties lumineuses de l'image. Tout ce qui depend de height et width devra etre regenere a chaque rendu car
    ** la fenetre peut changer de taille.
    */
    const int height = glutGet(GLUT_WINDOW_HEIGHT); TEST_OPENGL_ERROR();
    const int width = glutGet(GLUT_WINDOW_WIDTH); TEST_OPENGL_ERROR();
    // on genere le fbo et on le bind
    glGenFramebuffers(1, &color_FBO);TEST_OPENGL_ERROR();
    glBindFramebuffer(GL_FRAMEBUFFER, color_FBO);TEST_OPENGL_ERROR();
    // on genere/regle les textures
    glGenTextures(3, color_buffer_textures);TEST_OPENGL_ERROR();
    for (unsigned int i = 0; i < 3; i++)
    {
        glBindTexture(GL_TEXTURE_2D, color_buffer_textures[i]);TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);TEST_OPENGL_ERROR();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height,
                     0, GL_RGBA, GL_FLOAT, nullptr);TEST_OPENGL_ERROR();
        // on lie les deux textures aux color attachments
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_buffer_textures[i], 0);TEST_OPENGL_ERROR();
        glBindImageTexture(i, color_buffer_textures[i], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    }
    // on genere le depth buffer
    glGenRenderbuffers(1, &depth_buffer);TEST_OPENGL_ERROR();
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);TEST_OPENGL_ERROR();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);TEST_OPENGL_ERROR();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);TEST_OPENGL_ERROR();
    // on dit qu'on va rendre dans les color attachments
    GLenum attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);TEST_OPENGL_ERROR();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);TEST_OPENGL_ERROR();
}

bool Renderer::init_shader_blur() {
    if (!blur_prog[0].add_shader("compute_blur.shd", GL_COMPUTE_SHADER)
        || !blur_prog[1].add_shader("compute_blur_vertical.shd", GL_COMPUTE_SHADER))
        return false;

    if (!blur_prog[0].link() || !blur_prog[1].link())
        return false;

    return true;
}

void Renderer::switch_bloom() {
    bloom = !bloom;
}

void Renderer::switch_flare() {
    lensflare = !lensflare;
}

bool Renderer::init_shader_flare() {
    if (!flare_prog.add_shader("vertex.shd", GL_VERTEX_SHADER) ||
        !flare_prog.add_shader("fragment_flare.shd", GL_FRAGMENT_SHADER))
        return false;

    if (!flare_prog.link())
        return false;

    return true;
}

bool Renderer::init_shader_sum() {
    if (!sum_prog.add_shader("compute_sum.shd", GL_COMPUTE_SHADER))
        return false;

    if (!sum_prog.link())
        return false;

    sum_prog.use();
    return true;
}

Renderer renderer = Renderer();
