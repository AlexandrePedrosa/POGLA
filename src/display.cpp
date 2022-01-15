#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "display.hh"
#include "scene.hh"
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

void display_bunny() {
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

    glBindVertexArray(scene.bunny_vao_id);TEST_OPENGL_ERROR();
    if (!scene.points) {
        glUseProgram(scene.bunny_prog_id[0]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(scene.bunny_prog_id[0], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, scene.model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(scene.bunny_prog_id[0], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr()); TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(scene.bunny_prog_id[0], "projection_matrix"); TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr()); TEST_OPENGL_ERROR();
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else {
        glUseProgram(scene.bunny_prog_id[2]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(scene.bunny_prog_id[2], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, scene.model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(scene.bunny_prog_id[2], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr()); TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(scene.bunny_prog_id[2], "projection_matrix"); TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr()); TEST_OPENGL_ERROR();
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }
    if (scene.normals) {
        glUseProgram(scene.bunny_prog_id[1]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(scene.bunny_prog_id[1], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, scene.model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(scene.bunny_prog_id[1], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr()); TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(scene.bunny_prog_id[1], "projection_matrix"); TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr()); TEST_OPENGL_ERROR();
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    } else if (scene.angora) {
        glUseProgram(scene.bunny_prog_id[3]);TEST_OPENGL_ERROR();
        int model_loc = glGetUniformLocation(scene.bunny_prog_id[3], "model_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, scene.model.ptr());TEST_OPENGL_ERROR();
        int view_loc = glGetUniformLocation(scene.bunny_prog_id[3], "view_matrix");TEST_OPENGL_ERROR();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ptr()); TEST_OPENGL_ERROR();
        int proj_loc = glGetUniformLocation(scene.bunny_prog_id[3], "projection_matrix"); TEST_OPENGL_ERROR();
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.ptr()); TEST_OPENGL_ERROR();
        glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size()/3);TEST_OPENGL_ERROR();
    }

    glBindVertexArray(0);TEST_OPENGL_ERROR();
    glutSwapBuffers();TEST_OPENGL_ERROR();
}

