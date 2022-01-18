//
// Created by ajix on 05/01/2022.
//

#include <iostream>
#include <math.h>
#define PI 3.14159265

#include "scene.hh"
#include "display.hh"
#include "test_opengl_error.hh"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Scene::Scene() {
    model = mygl::matrix4::identity();
    camera_angle = 180.0f;
    camera_pitch = 0.0f;
}


bool Scene::init_scene_elements() {
    if (!init_shaders_bunny()
        //|| !init_shaders_wave()
        || !init_shader_skybox(skybox_program)
        || !init_cubemap_texture()
        || !init_threshold_texture())
        return false;
    init_object_vbo_bunnywave();
    init_object_vbo_skybox();

    return true;
}

bool Scene::init_threshold_texture() {
    glGenTextures(1, &threshold_tex_id);TEST_OPENGL_ERROR(); // on cree la texture
    glActiveTexture(GL_TEXTURE1);TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_1D, threshold_tex_id);TEST_OPENGL_ERROR(); // c'est une texture 1D
    int width;
    int height;
    int nb_channels;
    std::string filename = "texture/threshold.jpg";
    unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &nb_channels, 0);
    if (!image_data) { // echec chargement
        stbi_image_free(image_data);
        std::cerr << "Failed to load texture, path" << filename << std::endl;
        return false;
    }
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, width, 0,
                 GL_RED, GL_UNSIGNED_BYTE, image_data);TEST_OPENGL_ERROR();
    stbi_image_free(image_data);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);TEST_OPENGL_ERROR();
    return true;
}

bool Scene::init_cubemap_texture() { // cree la cubemap
    glGenTextures(1, &cubemap_tex_id); // on cree la texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_tex_id); // c'est une cubemap
    std::string dir = "texture/"; // on peut changer la skybox facilement
    std::vector<std::string> filenames { // texture de Emil Persson
        dir + "posx.jpg",
        dir + "negx.jpg",
        dir + "posy.jpg",
        dir + "negy.jpg",
        dir + "posz.jpg",
        dir + "negz.jpg"
    };
    int width; // on va charger des jpg pour la cubemap on a besoin de la largeur
    int height; // la hauteur
    int nb_channels; // le nombre de canaux
    unsigned char *image_data; // les donnees de l'image
    for(GLuint i = 0; i < filenames.size(); i++) {
        image_data = stbi_load(filenames[i].c_str(), &width, &height, &nb_channels, 0);
        if (!image_data) { // echec chargement
            stbi_image_free(image_data);
            std::cerr << "Failed to load texture, path" << filenames[i] << std::endl;
            return false;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,
                     GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, image_data );
        stbi_image_free(image_data);
        // GL_TEXTURE_ETC + i permet d'iterer sur l'enum des faces de la cubemap
    }
    // ci dessous les parametres de la texture de cubemap
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return true;
}

void Scene::init_object_vbo_skybox() {
    GLuint vbo_id;

    GLint vertex_location = glGetAttribLocation(skybox_program.id, "position");TEST_OPENGL_ERROR();
    glGenVertexArrays(1, &skybox_vao_id);TEST_OPENGL_ERROR();
    glBindVertexArray(skybox_vao_id);TEST_OPENGL_ERROR();
    glGenBuffers(1, &vbo_id);TEST_OPENGL_ERROR();
    if (vertex_location!=-1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, skybox_vertex_buffer_data.size()*sizeof(float),
                     skybox_vertex_buffer_data.data(), GL_STATIC_DRAW);TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    glBindVertexArray(0);
}

void Scene::init_object_vbo_bunnywave() {
    int max_nb_vbo = 5;
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];

    GLint vertex_location = glGetAttribLocation(bunny_prog[0].id,"position");TEST_OPENGL_ERROR();
    GLint normal_smooth_location = glGetAttribLocation(bunny_prog[0].id,"normalSmooth");TEST_OPENGL_ERROR();
    //GLint wave_location = glGetAttribLocation(wave_prog_id[0], "position");TEST_OPENGL_ERROR();

    glGenVertexArrays(1, &bunny_vao_id);TEST_OPENGL_ERROR();
    //glGenVertexArrays(1, &wave_vao_id);TEST_OPENGL_ERROR();
    glBindVertexArray(bunny_vao_id);TEST_OPENGL_ERROR();

    if (vertex_location!=-1) nb_vbo++;
    if (normal_smooth_location!=-1) nb_vbo++;
    //if (wave_location != -1) nb_vbo++;
    glGenBuffers(nb_vbo, vbo_ids);TEST_OPENGL_ERROR();
    glGenBuffers(2, ssbo_ids);

    if (vertex_location!=-1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size()*sizeof(float), vertex_buffer_data.data(), GL_STATIC_DRAW);TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_SHADER_STORAGE_BUFFER, vertex_buffer_data.size()*sizeof(float), 0, GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        index_vbo++;
    }

    if (normal_smooth_location!=-1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, normal_smooth_buffer_data.size()*sizeof(float), normal_smooth_buffer_data.data(), GL_STATIC_DRAW);TEST_OPENGL_ERROR();
        glVertexAttribPointer(normal_smooth_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(normal_smooth_location);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_SHADER_STORAGE_BUFFER, normal_smooth_buffer_data.size()*sizeof(float), 0, GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        index_vbo++;
    }
    //glBindVertexArray(wave_vao_id);TEST_OPENGL_ERROR();
    /*
    if (wave_location != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo]);TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, wave_vertex_buffer_data.size() * sizeof(float), wave_vertex_buffer_data.data(),GL_STATIC_DRAW);TEST_OPENGL_ERROR();
        glVertexAttribPointer(wave_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(wave_location);TEST_OPENGL_ERROR();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        index_vbo++;
    }
    */
    glBindVertexArray(0);
}

bool Scene::init_shader_compute(Program &prog) {
    if (!prog.add_shader("compute.shd", GL_COMPUTE_SHADER))
        return false;

    if (!prog.link())
        return false;

    prog.use();
    return true;
}

bool Scene::init_shader_bunny(std::string geometry, Program &prog) {
    if (!prog.add_shader("vertex_bunny.shd", GL_VERTEX_SHADER)
            || !prog.add_shader(geometry, GL_GEOMETRY_SHADER)
            || !prog.add_shader("fragment.shd", GL_FRAGMENT_SHADER))
        return false;

    if (!prog.link())
        return false;

    prog.use();
    return true;
}

bool Scene::init_shader_tessel(std::string geometry, Program &prog) {
    if (!prog.add_shader("vertex.shd", GL_VERTEX_SHADER)
            || !prog.add_shader(geometry, GL_GEOMETRY_SHADER)
            || !prog.add_shader("tessel_eval.shd", GL_TESS_EVALUATION_SHADER)
            || !prog.add_shader("tessel_control.shd", GL_TESS_CONTROL_SHADER)
            || !prog.add_shader("fragment.shd", GL_FRAGMENT_SHADER))
        return false;

    if (!prog.link())
        return false;

    prog.use();
    return true;
}

bool Scene::init_shader_skybox(Program &prog) {
    if (!prog.add_shader("vertex_skybox.shd", GL_VERTEX_SHADER)
            || !prog.add_shader("fragment_skybox.shd", GL_FRAGMENT_SHADER))
        return false;

    if (!prog.link())
        return false;

    prog.use();
    return true;
}

bool Scene::init_shaders_bunny() {
    return init_shader_bunny("geometry_copy.shd", bunny_prog[0])
        //&& init_shader_bunny("geometry_normals.shd", bunny_prog_id+1)
        //&& init_shader_bunny("geometry_point.shd", bunny_prog_id+2)
        //&& init_shader_bunny("geometry_angora.shd", bunny_prog_id+3)
        && init_shader_compute(compute_program);
}

bool Scene::init_shaders_wave() {
    return init_shader_tessel("geometry_copy.shd", wave_prog[0])
        && init_shader_tessel("geometry_normals.shd", wave_prog[1])
        && init_shader_tessel("geometry_point.shd", wave_prog[2])
        && init_shader_tessel("geometry_angora.shd", wave_prog[3]);
}

void Scene::anim_bunny() {
    GLint anim_time_location;
    compute_program.use();
    anim_time_location = glGetUniformLocation(compute_program.id, "anim_time");TEST_OPENGL_ERROR();
    if (timestop) {
        glUniform1f(anim_time_location, 0);TEST_OPENGL_ERROR();
    } else {
        glUniform1f(anim_time_location, anim_time);TEST_OPENGL_ERROR();
    }
    glBindVertexArray(bunny_vao_id);TEST_OPENGL_ERROR();
    compute_program.use();
    glDispatchCompute(vertex_buffer_data.size() / (3 * 1024) + 1, 1, 1);TEST_OPENGL_ERROR();
    glMemoryBarrier(GL_ALL_BARRIER_BITS);TEST_OPENGL_ERROR();
    glBindVertexArray(0);TEST_OPENGL_ERROR();
}

void Scene::anim_wave() {
    GLint anim_time_location;
    for (unsigned i = 0; i < 4; i++) {
        wave_prog[i].use();
        anim_time_location = glGetUniformLocation(wave_prog[i].id, "anim_time");
        glUniform1f(anim_time_location, anim_time);
    }
}

void Scene::anim() {
    if (bunny)
        anim_bunny();
    else
        anim_wave();
    if (!timestop) {
        anim_time += 0.05;
        if (anim_time == 100)
            anim_time = 0;
    }
}

void Scene::switch_bunny() {
    init_object_vbo_bunnywave();
    bunny = true;
    glutDisplayFunc(display_bunny);
    anim_time = 0;
}

void Scene::switch_wave() {
    glutDisplayFunc(display_wave);
    bunny = false;
    anim_time = 0;
}

void Scene::switch_points() {
    points = !points;
}

void Scene::switch_normals() {
    if (!normals)
        angora = false;
    normals = !normals;
}

void Scene::switch_angora() {
    if (!angora) {
        normals = false;
    }
    angora = !angora;
}

void Scene::switch_timestop() {
    timestop = !timestop;
}

void Scene::yaw_camera(GLfloat angle) {
    camera_angle += angle;
    while (camera_angle < 0) {
        camera_angle += 360.0f;
    } while (camera_angle > 0) {
        camera_angle -= 360.0f;
    }
}

void Scene::pitch_camera(GLfloat angle) {
    camera_pitch += angle;
    if (camera_pitch > 89.0f) {
        camera_pitch = 89.0f;
    } else if (camera_pitch < -89.0f) {
        camera_pitch = -89.0f;
    }
}

mygl::matrix4 Scene::compute_view_matrix() {
    mygl::matrix4 lookat = mygl::matrix4::identity();
    mygl::matrix4 transform = mygl::matrix4::identity();
    model = mygl::matrix4::identity();
    look_at(lookat, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f);
    rotate(transform, camera_pitch, 1.0f, 0.0f, 0.0f);
    rotate(transform, camera_angle, 0.0f, 1.0f, 0.0f);
    transform *= lookat;
    view = transform;
    translate(model, -camera_pos.x, -camera_pos.y, -camera_pos.z);
    return transform;
}

mygl::point3 Scene::camera_vector(int camdir) {
    GLfloat alpha;
    GLfloat beta = camera_pitch * PI / 180;
    if (camdir == CAMERA_FORWARD)
        alpha = camera_angle * PI / 180;
    else if (camdir == CAMERA_BACKWARD) {
        alpha = (camera_angle + 180) * PI / 180;
        beta = -beta;
    } else if (camdir == CAMERA_RIGHT)
        alpha = (camera_angle + 90) * PI / 180;
    else
        alpha = (camera_angle - 90) * PI / 180;
    return mygl::point3(-sin(alpha) * cos(beta), -sin(beta), cos(alpha) * cos(beta));
}

void Scene::move_camera(GLfloat l, int camdir) {
    mygl::point3 forward = camera_vector(camdir);
    forward *= l;
    camera_pos += forward;
}

Scene scene = Scene();
