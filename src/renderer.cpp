//
// Created by ajix on 17/01/2022.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "renderer.hh"
#include "test_opengl_error.hh"
#include "load_shader.hh"

Renderer::Renderer() {}

bool Renderer::init_render_elements() {
    init_color_FBO();
    if (!init_shader_blur(blur_prog_id))
        return false;
    return true;
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

bool Renderer::init_shader_blur(GLuint *program_id) {
    std::string compute_src[2] = {load("compute_blur.shd"),load("compute_blur_vertical.shd")};
    GLuint shader_id[2];
    GLint compile_status = GL_TRUE;

    char *compute_shd_src[2] = {(char*)std::malloc(compute_src[0].length()*sizeof(char)),
                                (char*)std::malloc(compute_src[1].length()*sizeof(char))};
    for (int i = 0; i < 2; i++) {
        compute_src[i].copy(compute_shd_src[i], compute_src[i].length());
        shader_id[i] = glCreateShader(GL_COMPUTE_SHADER); TEST_OPENGL_ERROR();
        glShaderSource(shader_id[i], 1, (const GLchar**)&(compute_shd_src[i]), 0);TEST_OPENGL_ERROR();
        glCompileShader(shader_id[i]);TEST_OPENGL_ERROR();
        glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status); TEST_OPENGL_ERROR();
        if(compile_status != GL_TRUE) {
            GLint log_size;
            char *shader_log;
            glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
            shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
            if(shader_log != 0) {
                glGetShaderInfoLog(shader_id[i], log_size, &log_size, shader_log);
                std::cerr << "SHADER COMPUTE: " << shader_log << std::endl;
                std::free(shader_log);
            }
            std::free(compute_shd_src[i]);
            glDeleteShader(shader_id[i]);
            return false;
        }
        std::free(compute_shd_src[i]);
        GLint link_status=GL_TRUE;
        program_id[i] = glCreateProgram();TEST_OPENGL_ERROR();
        if (program_id[i] == 0) return false;
        glAttachShader(program_id[i], shader_id[i]);TEST_OPENGL_ERROR();
        glLinkProgram(program_id[i]);TEST_OPENGL_ERROR();
        glGetProgramiv(program_id[i], GL_LINK_STATUS, &link_status);
        if (link_status!=GL_TRUE) {
            GLint log_size;
            char *program_log;
            glGetProgramiv(program_id[i], GL_INFO_LOG_LENGTH, &log_size);
            program_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
            if(program_log != 0) {
                glGetProgramInfoLog(*program_id, log_size, &log_size, program_log);
                std::cerr << "Program " << program_log << std::endl;
                std::free(program_log);
            }
            glDeleteProgram(program_id[i]);TEST_OPENGL_ERROR();
            glDeleteShader(shader_id[i]);TEST_OPENGL_ERROR();
            program_id[i] = 0;
            return false;
        }
    }
    //glUseProgram(*program_id);TEST_OPENGL_ERROR();
    //int uni_loc = glGetUniformLocation(*program_id, "input_image");
    //glUniform1i(uni_loc, 0);
    //uni_loc = glGetUniformLocation(*program_id, "output_image");
    //glUniform1i(uni_loc, 1);
    return true;

}

Renderer renderer = Renderer();