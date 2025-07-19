#include "System/Matrix3x3.hpp"
#include <System/Mathf.hpp>

namespace System {
    Matrix3x3::Matrix3x3() {
        raw[0] = 1.0f; raw[3] = 0.0f; raw[6] = 0.0f;
        raw[1] = 0.0f; raw[4] = 1.0f; raw[7] = 0.0f;
        raw[2] = 0.0f; raw[5] = 0.0f; raw[8] = 1.0f;
    }
    Matrix3x3::Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) {
        M00 = m00;
        M01 = m01;
        M02 = m02;
        M10 = m10;
        M11 = m11;
        M12 = m12;
        M20 = m20;
        M21 = m21;
        M22 = m22;
    }
    // Produces a transposed 3x3 matrix copy without modifying the original.
    // Optimized with explicit swaps-independent of memory layout assumptions.
    Matrix3x3 Matrix3x3::transpose() const {
        Matrix3x3 result;

        result.m[0][0] = m[0][0];
        result.m[0][1] = m[1][0];
        result.m[0][2] = m[2][0];

        result.m[1][0] = m[0][1];
        result.m[1][1] = m[1][1];
        result.m[1][2] = m[2][1];

        result.m[2][0] = m[0][2];
        result.m[2][1] = m[1][2];
        result.m[2][2] = m[2][2];

        return result;

    }
    Matrix3x3 Matrix3x3::Invert() const {
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
    float Matrix3x3::Determinant() const {
        return raw[0] * (raw[4] * raw[8] - raw[5] * raw[7]) -
            raw[1] * (raw[3] * raw[8] - raw[5] * raw[6]) +
            raw[2] * (raw[3] * raw[7] - raw[4] * raw[6]);
    }
    Matrix3x3 Matrix3x3::Adjugate() const {
        return Matrix3x3(
            (raw[4] * raw[8] - raw[5] * raw[7]), -(raw[3] * raw[8] - raw[5] * raw[6]), (raw[3] * raw[7] - raw[4] * raw[6]),
            -(raw[1] * raw[8] - raw[2] * raw[7]), (raw[0] * raw[8] - raw[2] * raw[6]), -(raw[0] * raw[7] - raw[1] * raw[6]),
            (raw[1] * raw[5] - raw[2] * raw[4]), -(raw[0] * raw[5] - raw[2] * raw[3]), (raw[0] * raw[4] - raw[1] * raw[3])
        ).transpose();
    }
    //compare matrixA against matrixB ignoring column/row major order
    bool Matrix3x3::CompareMajorityIndependant(Matrix3x3 matrixA, Matrix3x3 matrixB){
        const float epsilon = 1e-6f;

        bool matchRowMajor = true;
        bool matchColumnMajor = true;

        // Compare row-major directly
        for (int i = 0; i < 9; ++i) {
            if (std::fabs(matrixA.raw[i] - matrixB.raw[i]) > epsilon)
                matchRowMajor = false;
        }

        // Compare column-major (transpose simulation)
        int idx = 0;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                int flatIndexRowMajor = row * 3 + col;     // i * width + j
                int flatIndexColMajor = col * 3 + row;     // j * height + i
                if (System::Mathf::Abs(matrixA.raw[idx] - matrixB.raw[flatIndexColMajor]) > epsilon)
                    matchColumnMajor = false;
                idx++;
            }
        }

        return matchRowMajor || matchColumnMajor;
    }
    Matrix3x3 Matrix3x3::identity() {
        return Matrix3x3(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        );
    }








    Matrix3x3 Matrix3x3::Translate(float tx, float ty) {
        return Matrix3x3(
            1.0f, 0.0f, tx,
            0.0f, 1.0f, ty,
            0.0f, 0.0f, 1.0f
        );
    }
    Matrix3x3 Matrix3x3::Scale(float sx, float sy) {
        return Matrix3x3(
            sx, 0.f, 0.0f,
            0.0f, sy, 0.0f,
            0.0f, 0.0f, 1.0f
        );
    }
    Matrix3x3 Matrix3x3::Rotate(float radians) {
        float c = System::Mathf::Cos(radians);
        float s = System::Mathf::Sin(radians);
        return Matrix3x3(
            c, s, 0.0f,
            -s, c, 0.0f,
            0.0f, 0.0f, 1.0f
        );
    }
    Matrix3x3 Matrix3x3::TRSOptimised(Vector2 translation, float rotation, Vector2 scale){
        const float radians = rotation * System::Mathf::Deg2Rad;
        const float c = System::Mathf::Cos(radians);
        const float s = System::Mathf::Sin(radians);
        Matrix3x3 m;

        m.M00 = scale.x * c;
        m.M01 = scale.y * -s;
        m.M02 = translation.x;

        m.M10 = scale.x * s;
        m.M11 = scale.y * c;
        m.M12 = translation.y;

        m.M20 = 0.0f;
        m.M21 = 0.0f;
        m.M22 = 1.0f;

        return m;
    }
    Matrix3x3 Matrix3x3::TRS(Vector2 translation, float rotation, Vector2 scale) {
        Matrix3x3 T = Translate(translation.x, translation.y);
        Matrix3x3 R = Rotate(rotation);
        Matrix3x3 S = Scale(scale.x, scale.y);
        return T * S * R;
    }
}