#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "matrix4.hh"
#include "OBJloader.hh"
#include "scene.hh"
#include "test_opengl_error.hh"
#include "display.hh"

int last_x, last_y; // we keep the last mouse position for dragging



void window_resize(int width, int height) {
    //std::cout << "glViewport(0,0,"<< width << "," << height << ");TEST_OPENGL_ERROR();" << std::endl;
    glViewport(0,0,width,height);TEST_OPENGL_ERROR();
}

void init_glut(int &argc, char *argv[]) {
    glewExperimental = GL_TRUE;
    glutInit(&argc, argv);
    glutInitContextVersion(4,5);
    glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition ( 100, 100 );
    glutCreateWindow("Shader Programming");
    glutDisplayFunc(display_wave);
    glutReshapeFunc(window_resize);
}

bool init_glew() {
    if (glewInit()) {
        std::cerr << " Error while initializing glew";
        return false;
    }
    return true;
}

void init_GL() {
    glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);TEST_OPENGL_ERROR();
    //glEnable(GL_CULL_FACE);TEST_OPENGL_ERROR();
    glClearColor(0.35,0.30,0.4,1.0);TEST_OPENGL_ERROR();
}

void mouse_callback(int x, int y) {
    const float sensi_x = 0.1f;
    const float sensi_y = 0.1f;
    const GLfloat move_x = (float(x) - (float(last_x))) * sensi_x;
    const GLfloat move_y = (float(y) - (float(last_y))) * sensi_y;
    last_x = x;
    last_y = y;
    //glutWarpPointer(width/2, height/2); TEST_OPENGL_ERROR();
    //std::cout << "move_x = " << move_x << " move_y = " << move_y << std::endl;
    if (move_x != 0)
        scene.rotate_scene(move_x, 0.0f, 1.0f, 0.0f);
    if (move_y != 0)
        scene.rotate_scene(move_y, 1.0f, 0.0f, 0.0f);
}

void click_callback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        last_x = x;
        last_y = y;
    }
}

void keyboard_callback(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
    if (key == 'b') // switch or reset to bunny
        scene.switch_bunny();
    else if (key == 'w') // switch or reset to wave
        scene.switch_wave();
    else if (key == 'p') //switch to points / triangles
        scene.switch_points();
    else if (key == 'n') //switch normals on/off
        scene.switch_normals();
    else if (key == 'a') // switch angora on/off
        scene.switch_angora();
    else if (key == 't') // timestop
        scene.switch_timestop();
    glutPostRedisplay();
}


void timer_bunny(int value) {
    (void) value;
    scene.anim();
    glutPostRedisplay();
    glutTimerFunc(16, timer_bunny, 0);
}

int main(int argc, char *argv[]) {
    init_glut(argc, argv);
    if (!init_glew())
        std::exit(-1);
    init_GL();
    if (!scene.init_shaders_bunny())
        std::exit(-1);
    if (!scene.init_shaders_wave())
        std::exit(-1);
    scene.init_object_vbo_bunnywave();
    glPatchParameteri(GL_PATCH_VERTICES, 4);TEST_OPENGL_ERROR();
    glutTimerFunc(16, timer_bunny, 0);
    glutMouseFunc(click_callback);
    glutMotionFunc(mouse_callback);
    glutDisplayFunc(display_wave);
    glutKeyboardFunc(keyboard_callback);
    glutMainLoop();
}
