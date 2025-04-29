#include <System/Matrix4x4.h>
namespace System {

    const Matrix4x4 Matrix4x4::identity(Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), Vector4(0, 0, 1, 0), Vector4(0, 0, 0, 1));
    const Matrix4x4 Matrix4x4::zero(Vector4(0, 0, 0, 0),Vector4(0, 0, 0, 0),Vector4(0, 0, 0, 0),Vector4(0, 0, 0, 0));


 







    Matrix4x4::Matrix4x4()
        : m00(1), m10(0), m20(0), m30(0),
        m01(0), m11(1), m21(0), m31(0),
        m02(0), m12(0), m22(1), m32(0),
        m03(0), m13(0), m23(0), m33(1) {
    }
    Matrix4x4::Matrix4x4(System::Vector4 column0, System::Vector4 column1, System::Vector4 column2, System::Vector4 column3) {
        m00 = column0.x;  m01 = column1.x;  m02 = column2.x;  m03 = column3.x;
        m10 = column0.y;  m11 = column1.y;  m12 = column2.y;  m13 = column3.y;
        m20 = column0.z;  m21 = column1.z;  m22 = column2.z;  m23 = column3.z;
        m30 = column0.w;  m31 = column1.w;  m32 = column2.w;  m33 = column3.w;
    }

    Matrix4x4 Matrix4x4::Frustum(float left, float right, float bottom, float top, float zNear, float zFar){
    
    }
    Matrix4x4 Matrix4x4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar){
        
    }
    Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float zNear, float zFar){
        
    }
    Matrix4x4 Matrix4x4::Rotation(System::Quaternion q){
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
    Matrix4x4 Matrix4x4::Scaling(const Vector3& scale) {
        return Matrix4x4(
            Vector4(scale.x, 0, 0, 0),
            Vector4(0, scale.y, 0, 0),
            Vector4(0, 0, scale.z, 0),
            Vector4(0, 0, 0, 1)
        );
    }
    Matrix4x4 Matrix4x4::Translation(const Vector3& position) {
        return Matrix4x4(
            Vector4(1, 0, 0, 0),
            Vector4(0, 1, 0, 0),
            Vector4(0, 0, 1, 0),
            Vector4(position.x, position.y, position.z, 1)
        );
    }
    Matrix4x4 Matrix4x4::TRS(Vector3 pos, Quaternion q, Vector3 s) {
        Matrix4x4 translation = Translation(pos);
        Matrix4x4 rotation = Rotation(q);
        Matrix4x4 scale = Scaling(s);
        return translation * rotation * scale;
    }
    Matrix4x4 Matrix4x4::transpose() const {
        Matrix4x4 result;
        result.m00 = m00; 
        result.m01 = m10; 
        result.m02 = m20; 
        result.m03 = m30;
        result.m10 = m01; 
        result.m11 = m11; 
        result.m12 = m21; 
        result.m13 = m31;
        result.m20 = m02; 
        result.m21 = m12; 
        result.m22 = m22; 
        result.m23 = m32;
        result.m30 = m03; 
        result.m31 = m13; 
        result.m32 = m23; 
        result.m33 = m33;
        return result;
    }

 








}