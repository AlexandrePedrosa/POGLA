//
// Created by ajix on 17/01/2022.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "renderer.hh"
#include "test_opengl_error.hh"

Renderer::Renderer() {}

void Renderer::init_render_elements() {
    init_color_FBO();
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
    glGenTextures(2, color_buffer_textures);TEST_OPENGL_ERROR();
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, color_buffer_textures[i]);TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);TEST_OPENGL_ERROR();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);TEST_OPENGL_ERROR();
        // on lie les deux textures aux color attachments
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_buffer_textures[i], 0);TEST_OPENGL_ERROR();
    }
    // on genere le depth buffer
    glGenRenderbuffers(1, &depth_buffer);TEST_OPENGL_ERROR();
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);TEST_OPENGL_ERROR();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);TEST_OPENGL_ERROR();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);TEST_OPENGL_ERROR();
    // on dit qu'on va rendre dans les color attachments
    GLenum attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);TEST_OPENGL_ERROR();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);TEST_OPENGL_ERROR();
}

Renderer renderer = Renderer();