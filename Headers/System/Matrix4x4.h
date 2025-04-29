#ifndef _SYSTEM_MATRIX4X4_H
#define _SYSTEM_MATRIX4X4_H

#include <System/Vector4.h>
#include "Vector3.h"
#include "Quaternion.h"
namespace System {
    struct Matrix4x4 {

        static const Matrix4x4 identity;
        static const Matrix4x4 zero;




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
        /*
         ----------------------------------------------------------------------------------------
         Constructors
         ----------------------------------------------------------------------------------------
        */
        Matrix4x4();
        Matrix4x4(System::Vector4 column0, System::Vector4 column1, System::Vector4 column2, System::Vector4 column3);
        /*
        ----------------------------------------------------------------------------------------
        Static Methods
        ----------------------------------------------------------------------------------------
        */
        
        static Matrix4x4 Frustum(float left, float right, float bottom, float top, float zNear, float zFar);
        static Matrix4x4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
        static Matrix4x4 Perspective(float fov, float aspect, float zNear, float zFar);
        static Matrix4x4 Rotation(System::Quaternion q);
        static Matrix4x4 Scaling(const Vector3& scale);
        static Matrix4x4 Translation(const Vector3& position);
        static Matrix4x4 TRS(Vector3 pos, Quaternion q, Vector3 s);
    





            /*
            ----------------------------------------------------------------------------------------
            Operators
            ----------------------------------------------------------------------------------------
            */
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






         Matrix4x4 transpose() const;


    };



}
#endif