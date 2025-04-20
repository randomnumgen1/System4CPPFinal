#ifndef _SYSTEM_VECTOR3_H
#define _SYSTEM_VECTOR3_H
#include <algorithm>
#include <corecrt_math_defines.h>
//#include <System/Vector2.h>

namespace System {
//class Vector2;
    struct Vector3 {
    public:
        union {
            float v[3];
            struct {
                float x;
                float y;
                float z;
            };
        };
        static constexpr float Rad2Deg = 180.0f / static_cast<float>(M_PI);

        static constexpr float  kEpsilon = 0.00001F;
        static constexpr float  kEpsilonNormalSqrt = 1e-15F;
        /*
         ----------------------------------------------------------------------------------------
        Constructors
         ----------------------------------------------------------------------------------------
        */
      
        inline Vector3() {
            this->x = this->y = this->z = 0.0f;
        }
        inline Vector3(float x, float y, float z) {
            this->x = x; this->y = y; this->z = z;
        }

        /*
        *  inline explicit Vector3(System::Vector2 lol) {
            this->x = lol.x;
            this->y = lol.y;
            this->z = 0.0f;
        }
        * 
        * 
        ----------------------------------------------------------------------------------------
        Static Methods
        ----------------------------------------------------------------------------------------
        */
        inline static float Angle(const Vector3 from, const  Vector3 to) {
            float denominator = (float)std::sqrt(from.sqrMagnitude() * to.sqrMagnitude());
            if (denominator < kEpsilonNormalSqrt) {
                return 0.0f;
            }
            float dot = std::clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
            return std::acos(dot) * Rad2Deg;
        }
        inline  static Vector3 ClampMagnitude(const Vector3 vector, const  float maxLength) {

        }
        /*Cross*/
        inline static Vector3 Cross(const Vector3 lhs, const  Vector3 rhs) {
            return Vector3(
                (lhs.y * rhs.z) - (lhs.z * rhs.y),
                (lhs.z * rhs.x) - (lhs.x * rhs.z),
                (lhs.x * rhs.y) - (lhs.y * rhs.x));
        }
        /*Distance*/
        inline  static float Distance(const Vector3 lhs, const  Vector3 rhs) {
            return std::sqrt(
                std::powf(rhs.x - lhs.x, 2) +
                std::powf(rhs.y - lhs.y, 2) +
                std::powf(rhs.z - lhs.z, 2)
            );
        }
        /*Dot*/
        inline static float Dot(const Vector3 lhs, const  Vector3 rhs){
            return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
        }
        /*Lerp*/
        inline  static Vector3 Lerp(const Vector3 lhs, const  Vector3 rhs,  float t) {
            t = std::clamp(t, 0.0f, 1.0f);
            return Vector3(
                (lhs.x + (t * (rhs.x - lhs.x))),
                (lhs.y + (t * (rhs.y - lhs.y))),
                (lhs.z + (t * (rhs.z - lhs.z)))
            );
        }
        /*LerpUnclamped*/
        inline static Vector3 LerpUnclamped(const Vector3 lhs, const  Vector3 rhs, const  float t) {
            return Vector3(
                (lhs.x + (t * (rhs.x - lhs.x))),
                (lhs.y + (t * (rhs.y - lhs.y))),
                (lhs.z + (t * (rhs.z - lhs.z)))
            );
        }
        /*Max*/
        inline  static Vector3 Max(const Vector3 lhs, const  Vector3 rhs) {
            return  Vector3(
                std::max(lhs.x, rhs.x),
                std::max(lhs.y, rhs.y),
                std::max(lhs.z, rhs.z)
            );
        }
        /*Min*/
        inline  static Vector3 Min(const Vector3 lhs, const  Vector3 rhs) {
            return  Vector3(
                std::min(lhs.x, rhs.x),
                std::min(lhs.y, rhs.y),
                std::min(lhs.z, rhs.z)
            );
        }
        inline static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta) {
            throw std::runtime_error("Not Implemented Error.");
        }
        inline  static Vector3 Normalize(Vector3 value) {
            throw std::runtime_error("Not Implemented Error.");
        }
        inline  static void OrthoNormalize(Vector3& normal, Vector3& tangent) {
            throw std::runtime_error("Not Implemented Error.");
        }
        /*Project*/
        inline  static Vector3 Project(const Vector3 vector, const  Vector3 onNormal) {
            return (onNormal * (Dot(vector, onNormal) / Dot(onNormal, onNormal)));
        }
        /*ProjectOnPlane*/
        inline static Vector3 ProjectOnPlane(const Vector3 vector, const  Vector3 planeNormal) {
            Vector3 projection = Project(vector, planeNormal);
            return vector - projection;

        }
        /*Reflect*/
        inline static Vector3 Reflect(const Vector3 inDirection, const  Vector3 inNormal) {
            float factor = -2.0f * Dot(inNormal, inDirection);
            return Vector3{
                factor * inNormal.x + inDirection.x,
                factor * inNormal.y + inDirection.y,
                factor * inNormal.z + inDirection.z
            };
        }
        inline static Vector3 RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta) {
            throw std::runtime_error("Not Implemented Error.");
        }
        /*Scale*/
        inline static Vector3 Scale(const Vector3 a, const  Vector3 b) {
            return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
        }
        inline  static float SignedAngle(Vector3 from, Vector3 to, Vector3 axis) {
            throw std::runtime_error("Not Implemented Error.");
        }
        inline  static Vector3 Slerp(Vector3 a, Vector3 b, float t) {
            throw std::runtime_error("Not Implemented Error.");
        }
        inline static Vector3 SlerpUnclamped(Vector3 lhs, Vector3 rhs, float t) {
            throw std::runtime_error("Not Implemented Error.");
        }
        inline static Vector3 SmoothDamp() {
            throw std::runtime_error("Not Implemented Error.");
        }
        //
        inline float magnitude() const {
            return std::sqrt(x * x + y * y + z * z);
        }
        inline float sqrMagnitude() const {
            return x * x + y * y + z * z;
        }



        Vector3 operator*(float scalar) const {
            return Vector3{
                x * scalar,
                y * scalar,
                z * scalar
            };
        }
        Vector3 operator-(const Vector3& other) const {
            return Vector3{
                x - other.x,
                y - other.y,
                z - other.z
            };
        }
        bool operator==(const Vector3& rhs) const { 
            float diff_x = x - rhs.x;
            float diff_y = y - rhs.y;
            float diff_z = z - rhs.z;
            float sqrmag = diff_x * diff_x + diff_y * diff_y + diff_z * diff_z;
            return sqrmag < kEpsilon * kEpsilon;
        }
        bool operator!=(const Vector3& rhs) const {
            return !(*this == rhs);
        }





        inline float operator[](int i) const {Assert((i >= 0) && (i < 3));return (&x)[i];}
        inline float& operator[](int i) { Assert((i >= 0) && (i < 3));return (&x)[i]; }





    };

}
#endif