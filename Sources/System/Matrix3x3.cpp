#include "System/Matrix3x3.hpp"

namespace System {
    Matrix3x3::Matrix3x3() {
        m[0] = 1.f; m[3] = 0.f; m[6] = 0.f;
        m[1] = 0.f; m[4] = 1.f; m[7] = 0.f;
        m[2] = 0.f; m[5] = 0.f; m[8] = 1.f;
    }
    Matrix3x3::Matrix3x3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22) {
        m[0] = m00; m[1] = m10; m[2] = m20;
        m[3] = m01; m[4] = m11; m[5] = m21;
        m[6] = m02; m[7] = m12; m[8] = m22;
    }
    Matrix3x3 Matrix3x3::transpose() const {
        Matrix3x3 T;
        T.m[0] = m[0];  T.m[1] = m[3];  T.m[2] = m[6];
        T.m[3] = m[1];  T.m[4] = m[4];  T.m[5] = m[7];
        T.m[6] = m[2];  T.m[7] = m[5];  T.m[8] = m[8];
        return T;
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