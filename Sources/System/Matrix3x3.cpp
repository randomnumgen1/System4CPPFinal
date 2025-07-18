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
        Matrix3x3 adj = Adjugate();
        float det = Determinant();
        if (det == 0) {
            return Matrix3x3::identity();
        }
        float invDet = 1.f / det;
        return Matrix3x3(
            adj.raw[0] * invDet, adj.raw[1] * invDet, adj.raw[2] * invDet,
            adj.raw[3] * invDet, adj.raw[4] * invDet, adj.raw[5] * invDet,
            adj.raw[6] * invDet, adj.raw[7] * invDet, adj.raw[8] * invDet);
    }
    float Matrix3x3::Determinant() const{
        return raw[0] * (raw[4] * raw[8] - raw[5] * raw[7]) -
            raw[3] * (raw[1] * raw[8] - raw[2] * raw[7]) +
            raw[6] * (raw[1] * raw[5] - raw[2] * raw[4]);
    }
    Matrix3x3 Matrix3x3::Adjugate() const{
        return Matrix3x3(
            (raw[4] * raw[8] - raw[5] * raw[7]), -(raw[1] * raw[8] - raw[2] * raw[7]), (raw[1] * raw[5] - raw[2] * raw[4]),
            -(raw[3] * raw[8] - raw[5] * raw[6]), (raw[0] * raw[8] - raw[2] * raw[6]), -(raw[0] * raw[5] - raw[2] * raw[3]),
            (raw[3] * raw[7] - raw[4] * raw[6]), -(raw[0] * raw[7] - raw[1] * raw[6]), (raw[0] * raw[4] - raw[1] * raw[3])
        );
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