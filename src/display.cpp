#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "display.hh"
#include "scene.hh"
#include "renderer.hh"
#include "test_opengl_error.hh"


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

    glBindVertexArray(scene.wave_vao_id);TEST_OPENGL_ERROR();
    if (!scene.points) {
        glUseProgram(scene.wave_prog_id[0]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(scene.wave_prog_id[0], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, scene.model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(scene.wave_prog_id[0], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr());TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(scene.wave_prog_id[0], "projection_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr());TEST_OPENGL_ERROR();
        glDrawArrays(GL_PATCHES, 0, wave_vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else {
        glUseProgram(scene.wave_prog_id[2]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(scene.wave_prog_id[2], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, scene.model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(scene.wave_prog_id[2], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr());TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(scene.wave_prog_id[2], "projection_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr());TEST_OPENGL_ERROR();
        glDrawArrays(GL_PATCHES, 0, wave_vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }
    if (scene.normals) {
        glUseProgram(scene.wave_prog_id[1]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(scene.wave_prog_id[1], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, scene.model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(scene.wave_prog_id[1], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr());TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(scene.wave_prog_id[1], "projection_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr());TEST_OPENGL_ERROR();
        glDrawArrays(GL_PATCHES, 0, wave_vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else if (scene.angora) {
        glUseProgram(scene.wave_prog_id[3]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(scene.wave_prog_id[3], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, scene.model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(scene.wave_prog_id[3], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr());TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(scene.wave_prog_id[3], "projection_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr());TEST_OPENGL_ERROR();
        glDrawArrays(GL_PATCHES, 0, wave_vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }

    glBindVertexArray(0);TEST_OPENGL_ERROR();
    glutSwapBuffers();TEST_OPENGL_ERROR();
}

void give_uniform_bunny(GLuint program, mygl::matrix4 proj) {
    int model_loc = glGetUniformLocation(program, "model_matrix");TEST_OPENGL_ERROR();
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, scene.model.ptr());TEST_OPENGL_ERROR();
    int view_loc = glGetUniformLocation(program, "view_matrix");TEST_OPENGL_ERROR();
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, scene.view.ptr()); TEST_OPENGL_ERROR();
    int proj_loc = glGetUniformLocation(program, "projection_matrix"); TEST_OPENGL_ERROR();
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr()); TEST_OPENGL_ERROR();
    int cam_loc = glGetUniformLocation(program, "camera_pos");TEST_OPENGL_ERROR();
    glUniform3fv(cam_loc, 1, scene.camera_pos.data());TEST_OPENGL_ERROR();
    int thresh_loc = glGetUniformLocation(program, "threshold");TEST_OPENGL_ERROR();
    glUniform1i(thresh_loc, 1);TEST_OPENGL_ERROR();
}

void give_uniform_skybox(GLuint program, mygl::matrix4 proj) {
    int view_loc = glGetUniformLocation(program, "view_matrix");TEST_OPENGL_ERROR();
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, scene.view.ptr());TEST_OPENGL_ERROR();
    int proj_loc = glGetUniformLocation(program, "projection_matrix");TEST_OPENGL_ERROR();
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr());TEST_OPENGL_ERROR();
    int thresh_loc = glGetUniformLocation(program, "threshold");TEST_OPENGL_ERROR();
    glUniform1i(thresh_loc, 1);TEST_OPENGL_ERROR();
    //int cam_loc = glGetUniformLocation(program, "camera_pos");TEST_OPENGL_ERROR();
    //glUniform3fv(cam_loc, 1, scene.camera_pos.data());TEST_OPENGL_ERROR();
}

void giveUniform1f(GLuint program, std::string name, GLfloat data) {
    int loc = glGetUniformLocation(program, name.c_str());TEST_OPENGL_ERROR();
    glUniform1f(loc, data);TEST_OPENGL_ERROR();
}

void display_bunny() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
    const int height = glutGet(GLUT_WINDOW_HEIGHT); TEST_OPENGL_ERROR();
    const int width = glutGet(GLUT_WINDOW_WIDTH); TEST_OPENGL_ERROR();
    const float ratio = float(width) / float(height);

    scene.compute_view_matrix();
    mygl::matrix4 proj = mygl::matrix4::identity();
    const float zoom = 0.035f;
    frustum(proj, -zoom * ratio, zoom * ratio, -zoom, zoom, 0.1f, 100.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, renderer.color_FBO);TEST_OPENGL_ERROR();
    //glViewport(0, 0, width, height);TEST_OPENGL_ERROR();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
    glDepthMask(GL_FALSE);TEST_OPENGL_ERROR(); // on desactive la depth pour que la skybox s'affiche derriere tout
    glUseProgram(scene.skybox_prog_id);TEST_OPENGL_ERROR();
    glBindVertexArray(scene.skybox_vao_id);TEST_OPENGL_ERROR();
    give_uniform_skybox(scene.skybox_prog_id, proj);
    glActiveTexture(GL_TEXTURE0);TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene.cubemap_tex_id);TEST_OPENGL_ERROR();
    glActiveTexture(GL_TEXTURE1);TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_1D, scene.threshold_tex_id); TEST_OPENGL_ERROR();
    glDrawArrays(GL_TRIANGLES, 0, skybox_vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    glDepthMask(GL_TRUE);

    glBindVertexArray(scene.bunny_vao_id);TEST_OPENGL_ERROR();
    if (!scene.points) {
        glUseProgram(scene.bunny_prog_id[0]);TEST_OPENGL_ERROR();
        give_uniform_bunny(scene.bunny_prog_id[0], proj);
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else {
        glUseProgram(scene.bunny_prog_id[2]);TEST_OPENGL_ERROR();
        give_uniform_bunny(scene.bunny_prog_id[2], proj);
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }
    if (scene.normals) {
        glUseProgram(scene.bunny_prog_id[1]);TEST_OPENGL_ERROR();
        give_uniform_bunny(scene.bunny_prog_id[1], proj);
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else if (scene.angora) {
        glUseProgram(scene.bunny_prog_id[3]);TEST_OPENGL_ERROR();
        give_uniform_bunny(scene.bunny_prog_id[3], proj);
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }
    glBindVertexArray(0);TEST_OPENGL_ERROR();

    glUseProgram(renderer.blur_prog_id[0]);TEST_OPENGL_ERROR();
    glDispatchCompute(width / 1024 + 1, height, 1); TEST_OPENGL_ERROR();
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);TEST_OPENGL_ERROR();
    glUseProgram(renderer.blur_prog_id[1]);TEST_OPENGL_ERROR();
    glDispatchCompute(width, height / 1024 + 1, 1); TEST_OPENGL_ERROR();
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);TEST_OPENGL_ERROR();

    glUseProgram(renderer.sum_prog_id);TEST_OPENGL_ERROR();
    giveUniform1f(renderer.sum_prog_id, "w1", 1);
    giveUniform1f(renderer.sum_prog_id, "w2", 1);
    glDispatchCompute(width / 32 + 1, height / 32 + 1, 1);TEST_OPENGL_ERROR();
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);TEST_OPENGL_ERROR();


    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer.color_FBO);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glutSwapBuffers();TEST_OPENGL_ERROR();
}

