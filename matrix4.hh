#ifndef MATRIX4_H
#define MATRIX4_H

#include <GL/freeglut.h>
#include <vector>
#include <ostream>

namespace mygl {

    struct point3 {
        point3(GLfloat a, GLfloat b, GLfloat c) {
            x = a; y = b; z = c;
        }
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
    
    struct point4 {
        point4(GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
            x = a; y = b; z = c; w = d;
        }
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat w;
    };

    class matrix4 {
    public:
        matrix4();
        static matrix4 identity();

        void operator*=(const matrix4& rhs);

        GLfloat get(unsigned i, unsigned j) const;
        GLfloat& at(unsigned i, unsigned j);
        GLfloat* ptr();
    private:
        std::vector<GLfloat> vec;
        //GLfloat arr[16] = { 0 };
    };
}

std::ostream& operator<<(std::ostream& out, const mygl::matrix4& m);

void frustum(mygl::matrix4& mat,
    const GLfloat& left, const GLfloat& right,
    const GLfloat& bottom, const GLfloat& top,
    const GLfloat& z_near, const GLfloat& z_far);

void look_at(mygl::matrix4 &mat,
    const GLfloat &eyeX, const GLfloat &eyeY, const GLfloat &eyeZ,
    const GLfloat &centerX, const GLfloat &centerY, const GLfloat &centerZ,
    GLfloat upX, GLfloat upY, GLfloat upZ
);

void translate(mygl::matrix4& mat,
               const GLfloat& x, const GLfloat& y, const GLfloat& z);

void rotate(mygl::matrix4& mat,
            const GLfloat& angle, const GLfloat& x, const GLfloat& y, const GLfloat& z);

#endif
