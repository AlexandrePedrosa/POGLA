#include <math.h>
#include "matrix4.hh"

namespace mygl {
    matrix4::matrix4() {
        vec = std::vector<GLfloat>(16, 0);
    }

    matrix4 matrix4::identity()
    {
        auto result = matrix4();
        for (unsigned i = 0; i < 4; i++) {
            result.at(i, i) = 1;
        }
        return result;
    }

    void matrix4::operator*=(const matrix4& rhs) {
        auto result = matrix4();
        for (unsigned i = 0; i < 4; i++) {
            for (unsigned j = 0; j < 4; j++) {
                for (unsigned k = 0; k < 4; k++) {
                    result.at(i, j) += this->get(i, k) * rhs.get(k, j);
                }
            }
        }
        this->vec = result.vec;
    }

    GLfloat matrix4::get(unsigned i, unsigned j) const {
        return vec[j * 4 + i];
    }

    GLfloat& matrix4::at(unsigned i, unsigned j) {
        return vec[j * 4 + i];
    }

    GLfloat* matrix4::ptr()
    {
        return vec.data();
    }
}

std::ostream& operator<<(std::ostream& out, const mygl::matrix4& m)
{
    for (unsigned i = 0; i < 4; i++) {
        out << "| ";
        for (unsigned j = 0; j < 4; j++) {
            out << m.get(i, j) << ' ';
        }
        out << "|" << '\n';
    }
    return out;
}

void frustum(mygl::matrix4& mat,
    const GLfloat& left, const GLfloat& right,
    const GLfloat& bottom, const GLfloat& top,
    const GLfloat& z_near, const GLfloat& z_far) {
    auto perspective = mygl::matrix4();
    perspective.at(0, 0) = (2 * z_near) / (right - left);
    perspective.at(1, 1) = (2 * z_near) / (top - bottom);
    perspective.at(0, 2) = (right + left) / (right - left);             //A
    perspective.at(1, 2) = (top + bottom) / (top - bottom);             //B
    perspective.at(2, 2) = -((z_far + z_near) / (z_far - z_near));      //C
    perspective.at(2, 3) = -((2 * z_far * z_near) / (z_far - z_near));  //D
    perspective.at(3, 2) = -1;
    mat *= perspective;
}

void look_at(mygl::matrix4& mat,
    const GLfloat& eyeX, const GLfloat& eyeY, const GLfloat& eyeZ,
    const GLfloat& centerX, const GLfloat& centerY, const GLfloat& centerZ,
    GLfloat upX, GLfloat upY, GLfloat upZ) {
    GLfloat Fx = centerX - eyeX;
    GLfloat Fy = centerY - eyeY;
    GLfloat Fz = centerZ - eyeZ;
    GLfloat norm = sqrt(Fx * Fx + Fy * Fy + Fz * Fz);
    Fx = Fx / norm;
    Fy = Fy / norm;
    Fz = Fz / norm;
    norm = sqrt(upX * upX + upY * upY + upZ * upZ);
    upX = upX / norm;
    upY = upY / norm;
    upZ = upZ / norm;
    GLfloat sX = Fy * upZ - Fz * upY;
    GLfloat sY = Fz * upX - Fx * upZ;
    GLfloat sZ = Fx * upY - Fy * upX;
    norm = sqrt(sX * sX + sY * sY + sZ * sZ);
    GLfloat uX = sY / norm * Fz - sZ / norm * Fx;
    GLfloat uY = sZ / norm * Fx - sX / norm * Fz;
    GLfloat uZ = sX / norm * Fy - sY / norm * Fx;
    auto M = mygl::matrix4();
    M.at(0, 0) = sX;
    M.at(0, 1) = sY;
    M.at(0, 2) = sZ;
    M.at(1, 0) = uX;
    M.at(1, 1) = uY;
    M.at(1, 2) = uZ;
    M.at(2, 0) = -Fx;
    M.at(2, 1) = -Fy;
    M.at(2, 2) = -Fz;
    M.at(3, 3) = 1;
    mat *= M;
    translate(mat, -eyeX, -eyeY, -eyeZ);
}

void translate(mygl::matrix4& mat,
    const GLfloat& x, const GLfloat& y, const GLfloat& z) {
    auto M = mygl::matrix4::identity();
    M.at(0, 3) = x;
    M.at(1, 3) = y;
    M.at(2, 3) = z;
    mat *= M;
}

void rotate(mygl::matrix4& mat,
    const GLfloat& angle, const GLfloat& x, const GLfloat& y, const GLfloat& z) {
    GLfloat rad = angle * 3.14159265 / 180.0f;
    GLfloat c = cos(rad);
    GLfloat s = sin(rad);
    //GLfloat norm = sqrt(x * x + y * y + z * z);
    //GLfloat X = x / norm;
    //GLfloat Y = y / norm;
    //GLfloat Z = z / norm;
    auto M = mygl::matrix4::identity();
    M.at(0, 0) = x * x * (1 - c) + c;
    M.at(0, 1) = x * y * (1 - c) - z * s;
    M.at(0, 2) = x * z * (1 - c) + y * s;

    M.at(1, 0) = y * x * (1 - c) + z * s;
    M.at(1, 1) = y * y * (1 - c) + c;
    M.at(1, 2) = y * z * (1 - c) - x * s;

    M.at(2, 0) = z * x * (1 - c) - y * s;
    M.at(2, 1) = z * y * (1 - c) + x * s;
    M.at(2, 2) = z * z * (1 - c) + c;
    mat *= M;
}
