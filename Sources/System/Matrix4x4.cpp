#include <System/Matrix4x4.hpp>
#include <stdexcept>
#include <System/Mathf.hpp>
namespace System {

    const Matrix4x4 Matrix4x4::identity(System::Vector4(1.0f, 0.0f, 0.0f, 0.0f), System::Vector4(0.0f, 1.0f, 0.0f, 0.0f), System::Vector4(0.0f, 0.0f, 1.0f, 0.0f), System::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    const Matrix4x4 Matrix4x4::zero(System::Vector4(0.0f, 0.0f, 0.0f, 0.0f), System::Vector4(0.0f, 0.0f, 0.0f, 0.0f), System::Vector4(0.0f, 0.0f, 0.0f, 0.0f), System::Vector4(0.0f, 0.0f, 0.0f, 0.0f));


 







    Matrix4x4::Matrix4x4()
        : m00(1), m10(0), m20(0), m30(0),
        m01(0), m11(1), m21(0), m31(0),
        m02(0), m12(0), m22(1), m32(0),
        m03(0), m13(0), m23(0), m33(1) {
    }
    Matrix4x4::Matrix4x4(System::Vector4 column0, System::Vector4 column1, System::Vector4 column2, System::Vector4 column3) {
        this->m00 = column0.x;  this->m01 = column1.x;  this->m02 = column2.x;  this->m03 = column3.x;
        this->m10 = column0.y;  this->m11 = column1.y;  this->m12 = column2.y;  this->m13 = column3.y;
        this->m20 = column0.z;  this->m21 = column1.z;  this->m22 = column2.z;  this->m23 = column3.z;
        this->m30 = column0.w;  this->m31 = column1.w;  this->m32 = column2.w;  this->m33 = column3.w;


    }
    Matrix4x4 Matrix4x4::Frustum(float left, float right, float bottom, float top, float zNear, float zFar) {
        float invWidth = 1.0f / (right - left);
        float invHeight = 1.0f / (top - bottom);
        float invDepth = 1.0f / (zFar - zNear);
        return Matrix4x4({
            Vector4(2.0f * zNear * invWidth, 0.0f, (right + left) * invWidth, 0.0f),
            Vector4(0.0f, 2.0f * zNear * invHeight, (top + bottom) * invHeight, 0.0f),
            Vector4(0.0f, 0.0f, (zFar + zNear) * invDepth, 2.0f * zFar * zNear * invDepth),
            Vector4(0.0f, 0.0f, 1.0f, 0.0f) 
                         });
    }
    Matrix4x4 Matrix4x4::LookAt2(Vector3 from, Vector3 to, Vector3 up) {
        Vector3 forward = (to - from).normalized();
        Vector3 right = Vector3::Cross(up,forward).normalized();
        Vector3 trueUp = Vector3::Cross(forward,right);

        return {
            Vector4(right.x, trueUp.x, -forward.x, 0.0f),
            Vector4(right.y, trueUp.y, -forward.y, 0.0f),
           Vector4(right.z, trueUp.z, -forward.z, 0.0f),
          Vector4(-Vector3::Dot(right,from), -Vector3::Dot(trueUp,from), Vector3::Dot(forward,from), 1.0f) 
        };
    }
    Matrix4x4 Matrix4x4::LookAt(Vector3 from, Vector3 to, Vector3 up) {
        Matrix4x4 m;

        Vector3 z = (to - from).normalized();
        Vector3 x = Vector3::Cross(up, z).normalized();
        Vector3 y = Vector3::Cross(z, x);

        m.m00 = x.x;
        m.m01 = y.x;
        m.m02 = z.x;
        m.m03 = 0;
        m.m10 = x.y;
        m.m11 = y.y;
        m.m12 = z.y;
        m.m13 = 0;
        m.m20 = x.z;
        m.m21 = y.z;
        m.m22 = z.z;
        m.m23 = 0;
        m.m30 = -Vector3::Dot(x, from);
        m.m31 = -Vector3::Dot(y, from);
        m.m32 = -Vector3::Dot(z, from);
        m.m33 = 1;

        return m.transpose();
    }
    Matrix4x4 Matrix4x4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
        System::Matrix4x4 orthoMatrix = {
            Vector4(2.0f / (right - left), 0.0f, 0.0f, 0.0f),
            Vector4(0.0f, 2.0f / (top - bottom), 0.0f, 0.0f),
            Vector4(0.0f, 0.0f, -2.0f / (zFar - zNear), 0.0f),
            Vector4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(zFar + zNear) / (zFar - zNear), 1.0f)
        };
        return orthoMatrix;
    }
    Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float zNear, float zFar) {
        float tanHalfFovy = tan(fov / 2.0f);

        Matrix4x4 m = Matrix4x4::zero;
        m.m00 = 1.0f / (aspect * tanHalfFovy);
        m.m11 = 1.0f / tanHalfFovy;
        m.m22 = (zFar + zNear) / (zFar - zNear);
        m.m23 = - (2.0f * zFar * zNear) / (zFar - zNear);
        m.m32 = 1.0f;

        return m;
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

    Vector3 Matrix4x4::MultiplyPoint(const Vector3 point) const{
        Vector3 result;
        float w = 0.0f;
        result.x = m00 * point.x +  m01 * point.y +  m02 * point.z +  m03;
        result.y =  m10 * point.x +  m11 * point.y +  m12 * point.z +  m13;
        result.z =  m20 * point.x +  m21 * point.y +  m22 * point.z +  m23;
        w =  m30 * point.x +  m31 * point.y +  m32 * point.z +  m33;

        w = 1.0f / w;
        result.x *= w;
        result.y *= w;
        result.z *= w;
        return result;
    }

    Vector3 Matrix4x4::MultiplyPoint3x4(const Vector3 point) const{
        Vector3 result;
        result.x = m00 * point.x + m01 * point.y + m02 * point.z + m03;
        result.y = m10 * point.x + m11 * point.y + m12 * point.z + m13;
        result.z = m20 * point.x + m21 * point.y + m22 * point.z + m23;
        return result;
    }

    Vector3 Matrix4x4::MultiplyVector(const Vector3 vector) const{
        Vector3 result;
        result.x = m00 * vector.x + m01 * vector.y + m02 * vector.z;
        result.y = m10 * vector.x + m11 * vector.y + m12 * vector.z;
        result.z = m20 * vector.x + m21 * vector.y + m22 * vector.z;
        return result;
    }

    float Matrix4x4::determinant() const{
        return m00 * (m11 * m22 - m12 * m21) - m10 * (m01 * m22 - m02 * m21) + m20 * (m01 * m12 - m02 * m11);
    }
    Vector4 Matrix4x4::GetColumn(const int index){
        if (index < 0 || index > 3) {
            throw std::out_of_range("Index out of range");
        }
        switch (index) {
            case 0: return Vector4(m00, m10, m20, m30);
            case 1: return Vector4(m01, m11, m21, m31);
            case 2: return Vector4(m02, m12, m22, m32);
            case 3: return Vector4(m03, m13, m23, m33);
            default:
                throw std::out_of_range("Index out of range");
        }
    }
    Vector4 Matrix4x4::GetRow(const int index){
        if (index < 0 || index > 3) {
            throw std::out_of_range("Index out of range");
        }
        switch (index) {
            case 0: return Vector4(m00, m01, m02, m03);
            case 1: return Vector4(m10, m11, m12, m13);
            case 2: return Vector4(m20, m21, m22, m23);
            case 3: return Vector4(m30, m31, m32, m33);
            default:
                throw std::out_of_range("Index out of range");
        }
    }

    Matrix4x4 Matrix4x4::inverse() const{
        Matrix4x4 result;
        float det = determinant();
        if (det == 0) {
            return Matrix4x4::identity;
        }
        float invDet = 1.0f / det;
        result.m00 = (m11 * m22 - m12 * m21) * invDet;
        result.m01 = (m02 * m21 - m01 * m22) * invDet;
        result.m02 = (m01 * m12 - m02 * m11) * invDet;
        result.m10 = (m12 * m20 - m10 * m22) * invDet;
        result.m11 = (m00 * m22 - m02 * m20) * invDet;
        result.m12 = (m02 * m10 - m00 * m12) * invDet;
        result.m20 = (m10 * m21 - m11 * m20) * invDet;
        result.m21 = (m01 * m20 - m00 * m21) * invDet;
        result.m22 = (m00 * m11 - m01 * m10) * invDet;
        result.m03 = -(m01 * (m12 * m23 - m13 * m22) - m02 * (m11 * m23 - m13 * m21) + m03 * (m11 * m22 - m12 * m21));
        result.m13 = (m00 * (m12 * m23 - m13 * m22) - m02 * (m10 * m23 - m13 * m20) + m03 * (m10 * m22 - m12 * m20));
        result.m23 = -(m00 * (m11 * m23 - m13 * m21) - m01 * (m10 * m23 - m13 * m20) + m03 * (m10 * m21 - m11 * m20));
        result.m33 = (m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20));
        result.m03 *= invDet;
        result.m13 *= invDet;
        result.m23 *= invDet;
        result.m33 *= invDet;
        return result;
    }

    bool Matrix4x4::isIdentity() const {
        return m00 == 1.0f && m10 == 0.0f && m20 == 0.0f && m30 == 0.0f &&
               m01 == 0.0f && m11 == 1.0f && m21 == 0.0f && m31 == 0.0f &&
               m02 == 0.0f && m12 == 0.0f && m22 == 1.0f && m32 == 0.0f &&
               m03 == 0.0f && m13 == 0.0f && m23 == 0.0f && m33 == 1.0f;
    }

 








}