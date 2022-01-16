//
// Created by ajix on 05/01/2022.
//

#include <iostream>
#include <fstream>
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

std::string load(const std::string &filename) { //loads shader as a string from src directory
    std::string path = "src/" + filename;
    std::ifstream input_src_file(path, std::ios::in);
    std::string ligne;
    std::string file_content="";
    if (input_src_file.fail()) {
        std::cerr << "FAIL\n";
        return "";
    }
    while(getline(input_src_file, ligne)) {
        file_content = file_content + ligne + "\n";
    }
    file_content += '\0';
    input_src_file.close();
    return file_content;
}

bool Scene::init_scene_elements() {
    if (!init_shaders_bunny()
        //|| !init_shaders_wave()
        || !init_shader_skybox(&skybox_prog_id)
        || !init_cubemap_texture())
        return false;
    init_object_vbo_bunnywave();
    init_object_vbo_skybox();

    return true;
}

bool Scene::init_cubemap_texture() { // cree la cubemap
    glGenTextures(1, &cubemap_tex_id); // on cree la texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_tex_id); // c'est une cubemap
    std::vector<std::string> filenames { // texture de Emil Persson
        "texture/posx.jpg",
        "texture/negx.jpg",
        "texture/posy.jpg",
        "texture/negy.jpg",
        "texture/posz.jpg",
        "texture/negz.jpg"
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

    GLint vertex_location = glGetAttribLocation(skybox_prog_id, "position");TEST_OPENGL_ERROR();
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

    GLint vertex_location = glGetAttribLocation(bunny_prog_id[0],"position");TEST_OPENGL_ERROR();
    GLint normal_smooth_location = glGetAttribLocation(bunny_prog_id[0],"normalSmooth");TEST_OPENGL_ERROR();
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

bool Scene::init_shader_compute(GLuint *program_id) {
    std::string compute_src = load("compute.shd");

    GLuint shader_id;
    GLint compile_status = GL_TRUE;
    char *compute_shd_src = (char*)std::malloc(compute_src.length()*sizeof(char));

    compute_src.copy(compute_shd_src, compute_src.length());
    shader_id = glCreateShader(GL_COMPUTE_SHADER); TEST_OPENGL_ERROR();
    glShaderSource(shader_id, 1, (const GLchar**)&(compute_shd_src), 0);TEST_OPENGL_ERROR();

    glCompileShader(shader_id);TEST_OPENGL_ERROR();
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status); TEST_OPENGL_ERROR();
    if(compile_status != GL_TRUE) {
        GLint log_size;
        char *shader_log;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_size);
        shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
        if(shader_log != 0) {
            glGetShaderInfoLog(shader_id, log_size, &log_size, shader_log);
            std::cerr << "SHADER COMPUTE: " << shader_log << std::endl;
            std::free(shader_log);
        }
        std::free(compute_shd_src);
        glDeleteShader(shader_id);
        return false;
    }
    std::free(compute_shd_src);

    GLint link_status=GL_TRUE;
    *program_id = glCreateProgram();TEST_OPENGL_ERROR();
    if (*program_id == 0) return false;
    glAttachShader(*program_id, shader_id);TEST_OPENGL_ERROR();

    glLinkProgram(*program_id);TEST_OPENGL_ERROR();
    glGetProgramiv(*program_id, GL_LINK_STATUS, &link_status);
    if (link_status!=GL_TRUE) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(*program_id, GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
        if(program_log != 0) {
            glGetProgramInfoLog(*program_id, log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }
        glDeleteProgram(*program_id);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id);TEST_OPENGL_ERROR();
        *program_id = 0;
        return false;
    }
    glUseProgram(*program_id);TEST_OPENGL_ERROR();
    return true;

}

bool Scene::init_shader_bunny(std::string geometry, GLuint *program_id) {
    std::string vertex_src = load("vertex_bunny.shd");
    std::string geometry_src = load(geometry);
    std::string fragment_src = load("fragment.shd");

    GLuint shader_id[3];
    GLint compile_status = GL_TRUE;
    char *vertex_shd_src = (char*)std::malloc(vertex_src.length()*sizeof(char));
    char *geometry_shd_src = (char*)std::malloc(geometry_src.length()*sizeof(char));
    char *fragment_shd_src = (char*)std::malloc(fragment_src.length()*sizeof(char));

    vertex_src.copy(vertex_shd_src,vertex_src.length());
    geometry_src.copy(geometry_shd_src,geometry_src.length());
    fragment_src.copy(fragment_shd_src,fragment_src.length());

    shader_id[0] = glCreateShader(GL_VERTEX_SHADER);TEST_OPENGL_ERROR();
    shader_id[1] = glCreateShader(GL_GEOMETRY_SHADER);TEST_OPENGL_ERROR();
    shader_id[2] = glCreateShader(GL_FRAGMENT_SHADER);TEST_OPENGL_ERROR();

    glShaderSource(shader_id[0], 1, (const GLchar**)&(vertex_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[1], 1, (const GLchar**)&(geometry_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[2], 1, (const GLchar**)&(fragment_shd_src), 0);TEST_OPENGL_ERROR();

    for(int i = 0 ; i < 3 ; i++) {
        glCompileShader(shader_id[i]);TEST_OPENGL_ERROR();
        glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);
        if(compile_status != GL_TRUE) {
            GLint log_size;
            char *shader_log;
            glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
            shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
            if(shader_log != 0) {
                glGetShaderInfoLog(shader_id[i], log_size, &log_size, shader_log);
                std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
                std::free(shader_log);
            }
            std::free(vertex_shd_src);
            std::free(geometry_shd_src);
            std::free(fragment_shd_src);
            glDeleteShader(shader_id[0]);
            glDeleteShader(shader_id[1]);
            glDeleteShader(shader_id[2]);
            return false;
        }
    }
    std::free(vertex_shd_src);
    std::free(geometry_shd_src);
    std::free(fragment_shd_src);


    GLint link_status=GL_TRUE;
    *program_id = glCreateProgram();TEST_OPENGL_ERROR();
    if (program_id[0] == 0) return false;
    for(int i = 0 ; i < 3 ; i++) {
        glAttachShader(program_id[0], shader_id[i]);TEST_OPENGL_ERROR();
    }

    glLinkProgram(program_id[0]);TEST_OPENGL_ERROR();
    glGetProgramiv(program_id[0], GL_LINK_STATUS, &link_status);
    if (link_status!=GL_TRUE) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(program_id[0], GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
        if(program_log != 0) {
            glGetProgramInfoLog(program_id[0], log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }
        glDeleteProgram(program_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[1]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[2]);TEST_OPENGL_ERROR();
        program_id[0] = 0;
        return false;
    }
    glUseProgram(program_id[0]);TEST_OPENGL_ERROR();
    return true;

}

bool Scene::init_shader_tessel(std::string geometry, GLuint *program_id) {
    std::string vertex_src = load("vertex.shd");
    std::string tessel_control_src = load("tessel_control.shd");
    std::string tessel_eval_src = load("tessel_eval.shd");
    std::string geometry_src = load(geometry);
    std::string fragment_src = load("fragment.shd");

    GLuint shader_id[5];
    GLint compile_status = GL_TRUE;
    char *vertex_shd_src = (char*)std::malloc(vertex_src.length()*sizeof(char));
    char *tessel_control_shd_src = (char*)std::malloc(tessel_control_src.length()*sizeof(char));
    char *tessel_eval_shd_src = (char*)std::malloc(tessel_eval_src.length()*sizeof(char));
    char *geometry_shd_src = (char*)std::malloc(geometry_src.length()*sizeof(char));
    char *fragment_shd_src = (char*)std::malloc(fragment_src.length()*sizeof(char));

    vertex_src.copy(vertex_shd_src,vertex_src.length());
    tessel_control_src.copy(tessel_control_shd_src, tessel_control_src.length());
    tessel_eval_src.copy(tessel_eval_shd_src, tessel_eval_src.length());
    geometry_src.copy(geometry_shd_src,geometry_src.length());
    fragment_src.copy(fragment_shd_src,fragment_src.length());

    shader_id[0] = glCreateShader(GL_VERTEX_SHADER);TEST_OPENGL_ERROR();
    shader_id[1] = glCreateShader(GL_TESS_CONTROL_SHADER);TEST_OPENGL_ERROR();
    shader_id[2] = glCreateShader(GL_TESS_EVALUATION_SHADER);TEST_OPENGL_ERROR();
    shader_id[3] = glCreateShader(GL_GEOMETRY_SHADER);TEST_OPENGL_ERROR();
    shader_id[4] = glCreateShader(GL_FRAGMENT_SHADER);TEST_OPENGL_ERROR();

    glShaderSource(shader_id[0], 1, (const GLchar**)&(vertex_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[1], 1, (const GLchar**)&(tessel_control_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[2], 1, (const GLchar**)&(tessel_eval_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[3], 1, (const GLchar**)&(geometry_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[4], 1, (const GLchar**)&(fragment_shd_src), 0);TEST_OPENGL_ERROR();

    for(int i = 0 ; i < 5 ; i++) {
        glCompileShader(shader_id[i]);TEST_OPENGL_ERROR();
        glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);
        if(compile_status != GL_TRUE) {
            GLint log_size;
            char *shader_log;
            glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
            shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
            if(shader_log != 0) {
                glGetShaderInfoLog(shader_id[i], log_size, &log_size, shader_log);
                std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
                std::free(shader_log);
            }
            std::free(vertex_shd_src);
            std::free(tessel_control_shd_src);
            std::free(tessel_eval_shd_src);
            std::free(geometry_shd_src);
            std::free(fragment_shd_src);
            glDeleteShader(shader_id[0]);
            glDeleteShader(shader_id[1]);
            glDeleteShader(shader_id[2]);
            glDeleteShader(shader_id[3]);
            glDeleteShader(shader_id[4]);
            return false;
        }
    }
    std::free(vertex_shd_src);
    std::free(tessel_control_shd_src);
    std::free(tessel_eval_shd_src);
    std::free(geometry_shd_src);
    std::free(fragment_shd_src);


    GLint link_status=GL_TRUE;
    program_id[0] = glCreateProgram();TEST_OPENGL_ERROR();
    if (program_id[0] == 0) return false;
    for(int i = 0 ; i < 5 ; i++) {
        glAttachShader(program_id[0], shader_id[i]);TEST_OPENGL_ERROR();
    }

    glLinkProgram(program_id[0]);TEST_OPENGL_ERROR();
    glGetProgramiv(program_id[0], GL_LINK_STATUS, &link_status);
    if (link_status!=GL_TRUE) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(program_id[0], GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
        if(program_log != 0) {
            glGetProgramInfoLog(program_id[0], log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }
        glDeleteProgram(program_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[1]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[2]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[3]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[4]);TEST_OPENGL_ERROR();
        program_id[0]=0;
        return false;
    }
    glUseProgram(program_id[0]);TEST_OPENGL_ERROR();
    return true;
}

bool Scene::init_shader_skybox(GLuint *program_id) {
    std::string vertex_src = load("vertex_skybox.shd");
    std::string fragment_src = load("fragment_skybox.shd");

    GLuint shader_id[2];
    GLint compile_status = GL_TRUE;
    char *vertex_shd_src = (char*)std::malloc(vertex_src.length()*sizeof(char));
    char *fragment_shd_src = (char*)std::malloc(fragment_src.length()*sizeof(char));

    vertex_src.copy(vertex_shd_src,vertex_src.length());
    fragment_src.copy(fragment_shd_src,fragment_src.length());

    shader_id[0] = glCreateShader(GL_VERTEX_SHADER);TEST_OPENGL_ERROR();
    shader_id[1] = glCreateShader(GL_FRAGMENT_SHADER);TEST_OPENGL_ERROR();

    glShaderSource(shader_id[0], 1, (const GLchar**)&(vertex_shd_src), 0);TEST_OPENGL_ERROR();
    glShaderSource(shader_id[1], 1, (const GLchar**)&(fragment_shd_src), 0);TEST_OPENGL_ERROR();

    for(int i = 0 ; i < 2 ; i++) {
        glCompileShader(shader_id[i]);TEST_OPENGL_ERROR();
        glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);
        if(compile_status != GL_TRUE) {
            GLint log_size;
            char *shader_log;
            glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
            shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
            if(shader_log != 0) {
                glGetShaderInfoLog(shader_id[i], log_size, &log_size, shader_log);
                std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
                std::free(shader_log);
            }
            std::free(vertex_shd_src);
            std::free(fragment_shd_src);
            glDeleteShader(shader_id[0]);
            glDeleteShader(shader_id[1]);
            return false;
        }
    }
    std::free(vertex_shd_src);
    std::free(fragment_shd_src);


    GLint link_status=GL_TRUE;
    *program_id = glCreateProgram();TEST_OPENGL_ERROR();
    if (program_id[0] == 0) return false;
    for(int i = 0 ; i < 2 ; i++) {
        glAttachShader(program_id[0], shader_id[i]);TEST_OPENGL_ERROR();
    }

    glLinkProgram(program_id[0]);TEST_OPENGL_ERROR();
    glGetProgramiv(program_id[0], GL_LINK_STATUS, &link_status);
    if (link_status!=GL_TRUE) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(program_id[0], GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
        if(program_log != 0) {
            glGetProgramInfoLog(program_id[0], log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }
        glDeleteProgram(program_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[0]);TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[1]);TEST_OPENGL_ERROR();
        program_id[0] = 0;
        return false;
    }
    glUseProgram(program_id[0]);TEST_OPENGL_ERROR();
    return true;

}

bool Scene::init_shaders_bunny() {
    return init_shader_bunny("geometry_copy.shd", bunny_prog_id)
        //&& init_shader_bunny("geometry_normals.shd", bunny_prog_id+1)
        //&& init_shader_bunny("geometry_point.shd", bunny_prog_id+2)
        //&& init_shader_bunny("geometry_angora.shd", bunny_prog_id+3)
        && init_shader_compute(&compute_program_id);
}

bool Scene::init_shaders_wave() {
    return init_shader_tessel("geometry_copy.shd", wave_prog_id)
        && init_shader_tessel("geometry_normals.shd", wave_prog_id + 1)
        && init_shader_tessel("geometry_point.shd", wave_prog_id + 2)
        && init_shader_tessel("geometry_angora.shd", wave_prog_id + 3);
}

void Scene::anim_bunny() {
    GLint anim_time_location;
    glUseProgram(compute_program_id);
    anim_time_location = glGetUniformLocation(compute_program_id, "anim_time");
    if (timestop)
        glUniform1f(anim_time_location, 0);
    else
        glUniform1f(anim_time_location, anim_time);
    glBindVertexArray(bunny_vao_id);TEST_OPENGL_ERROR();
    glUseProgram(compute_program_id);TEST_OPENGL_ERROR();
    glDispatchCompute(vertex_buffer_data.size() / (3 * 1024) + 1, 1, 1);TEST_OPENGL_ERROR();
    glMemoryBarrier(GL_ALL_BARRIER_BITS);TEST_OPENGL_ERROR();
    glBindVertexArray(0);TEST_OPENGL_ERROR();
}

void Scene::anim_wave() {
    GLint anim_time_location;
    for (unsigned i = 0; i < 4; i++) {
        glUseProgram(wave_prog_id[i]);
        anim_time_location = glGetUniformLocation(wave_prog_id[i], "anim_time");
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