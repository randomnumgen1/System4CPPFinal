#ifndef _SYSTEM_MATRIX4X4_H
#define _SYSTEM_MATRIX4X4_H

#include <System/Vector4.h>
#include "Vector3.h"
#include "Quaternion.h"
namespace System {
    struct Matrix4x4 {
        // memory layout:
        //
        //             row no (=vertical)
        //               |  0   1   2   3
        //            ---+----------------
        //            0  | m00 m10 m20 m30
        // column no  1  | m01 m11 m21 m31
        // (=horiz)   2  | m02 m12 m22 m32
        //            3  | m03 m13 m23 m33
        float m00, m10, m20, m30;
        float m01, m11, m21, m31;
        float m02, m12, m22, m32;
        float m03, m13, m23, m33;
        // Default constructor (identity matrix)
        Matrix4x4()
            : m00(1), m10(0), m20(0), m30(0),
            m01(0), m11(1), m21(0), m31(0),
            m02(0), m12(0), m22(1), m32(0),
            m03(0), m13(0), m23(0), m33(1) {
        }
        Matrix4x4(System::Vector4 column0, System::Vector4 column1, System::Vector4 column2, System::Vector4 column3) {
             m00 = column0.x;  m01 = column1.x;  m02 = column2.x;  m03 = column3.x;
             m10 = column0.y;  m11 = column1.y;  m12 = column2.y;  m13 = column3.y;
             m20 = column0.z;  m21 = column1.z;  m22 = column2.z;  m23 = column3.z;
             m30 = column0.w;  m31 = column1.w;  m32 = column2.w;  m33 = column3.w;
        }
        static Matrix4x4 Translation(const Vector3& position) {
            return Matrix4x4(
                Vector4(1, 0, 0, 0),
                Vector4(0, 1, 0, 0),
                Vector4(0, 0, 1, 0),
                Vector4(position.x, position.y, position.z, 1)
            );
        }
        static Matrix4x4 Scaling(const Vector3& scale) {
            return Matrix4x4(
                Vector4(scale.x, 0, 0, 0),
                Vector4(0, scale.y, 0, 0),
                Vector4(0, 0, scale.z, 0),
                Vector4(0, 0, 0, 1)
            );
        }
    
         static Matrix4x4 Rotation(System::Quaternion q)
        {
            // Precalculate coordinate products
            float x = q.x * 2.0F;
            float y = q.y * 2.0F;
            float z = q.z * 2.0F;
            float xx = q.x * x;
            float yy = q.y * y;
            float zz = q.z * z;
            float xy = q.x * y;
            float xz = q.x * z;
            float yz = q.y * z;
            float wx = q.w * x;
            float wy = q.w * y;
            float wz = q.w * z;

            // Calculate 3x3 matrix from orthonormal basis
            Matrix4x4 m;
            m.m00 = 1.0f - (yy + zz); m.m10 = xy + wz; m.m20 = xz - wy; m.m30 = 0.0F;
            m.m01 = xy - wz; m.m11 = 1.0f - (xx + zz); m.m21 = yz + wx; m.m31 = 0.0F;
            m.m02 = xz + wy; m.m12 = yz - wx; m.m22 = 1.0f - (xx + yy); m.m32 = 0.0F;
            m.m03 = 0.0F; m.m13 = 0.0F; m.m23 = 0.0F; m.m33 = 1.0F;
            return m;
        }



         Matrix4x4 operator*(const Matrix4x4& other) const {
             Matrix4x4 result;
             // Implement multiplication here
             result.m00 = m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30;
             result.m10 = m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30;
             result.m20 = m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30;
             result.m30 = m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30;

             result.m01 = m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31;
             result.m11 = m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31;
             result.m21 = m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31;
             result.m31 = m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31;

             result.m02 = m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32;
             result.m12 = m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32;
             result.m22 = m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32;
             result.m32 = m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32;

             result.m03 = m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33;
             result.m13 = m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33;
             result.m23 = m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33;
             result.m33 = m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33;

             return result;
         }









    };



}
#endif