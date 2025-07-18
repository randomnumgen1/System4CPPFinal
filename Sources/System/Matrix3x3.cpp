#include "System/Matrix3x3.hpp"

namespace System {
    Matrix3x3::Matrix3x3() {
       

    }
    Matrix3x3::Matrix3x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33) {
        M11 = m11;
        M12 = m12;
        M13 = m13;
        M21 = m21;
        M22 = m22;
        M23 = m23;
        M31 = m31;
        M32 = m32;
        M33 = m33;
    }
    Matrix3x3 Matrix3x3::transpose() const {
        return Matrix3x3(
            M11,
            M21,
            M31,
            M12,
            M22,
            M32,
            M13,
            M23,
            M33
        );
    }
    Matrix3x3 Matrix3x3::Invert() const{
        float det =
            m[0] * (m[4] * m[8] - m[5] * m[7]) -
            m[3] * (m[1] * m[8] - m[2] * m[7]) +
            m[6] * (m[1] * m[5] - m[2] * m[4]);

        if (det == 0.f) {
            return Matrix3x3::identity();
        }

        float invDet = 1.f / det;

        Matrix3x3 inv;

        // First column
        inv.m[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
        inv.m[1] = -(m[1] * m[8] - m[2] * m[7]) * invDet;
        inv.m[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;

        // Second column
        inv.m[3] = -(m[3] * m[8] - m[5] * m[6]) * invDet;
        inv.m[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
        inv.m[5] = -(m[0] * m[5] - m[2] * m[3]) * invDet;

        // Third column
        inv.m[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
        inv.m[7] = -(m[0] * m[7] - m[1] * m[6]) * invDet;
        inv.m[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;

        return inv;
    }
    float Matrix3x3::Determinant() const
    {
        return 0.0f;
    }
    Matrix3x3 Matrix3x3::Adjugate() const
    {
        return Matrix3x3();
    }
    Matrix3x3 Matrix3x3::identity() {
        return Matrix3x3(
            1.f, 0.f, 0.f,
            0.f, 1.f, 0.f,
            0.f, 0.f, 1.f
        );
    }







    Matrix3x3 Matrix3x3::Translate(float tx, float ty) {
        return Matrix3x3(
            1.f, 0.f, tx,
            0.f, 1.f, ty,
            0.f, 0.f, 1.f
        );
    }
    Matrix3x3 Matrix3x3::Scale(float sx, float sy) {
        return Matrix3x3(
            sx, 0.f, 0.f,
            0.f, sy, 0.f,
            0.f, 0.f, 1.f
        );
    }
    Matrix3x3 Matrix3x3::Rotate(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);
        return Matrix3x3(
            c, -s, 0.f,
            s, c, 0.f,
            0.f, 0.f, 1.f
        );
    }
}