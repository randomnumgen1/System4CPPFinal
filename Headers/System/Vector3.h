#ifndef _SYSTEM_VECTOR3_H
#define _SYSTEM_VECTOR3_H
#include <corecrt_math_defines.h>
#include <assert.h>
//#include <System/Vector2.h>

namespace System {
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


        static Vector3 back;
        static Vector3 down;
        static Vector3 forward;
        static Vector3 left;
        static Vector3 one;
        static Vector3 right;
        static Vector3 up;
        static Vector3 zero;


        /*
         ----------------------------------------------------------------------------------------
        Constructors
         ----------------------------------------------------------------------------------------
        */
      
        Vector3();
        Vector3(float x, float y, float z);

        /*inline explicit Vector3( System::Vector2 lol) {
            this->x = lol.x;
            this->y = lol.y;
            this->z = 0.0f;
        }
        *  
        * 
        * 
        ----------------------------------------------------------------------------------------
        Static Methods
        ----------------------------------------------------------------------------------------
        */
        static float Angle(const Vector3 from, const  Vector3 to);
		static Vector3 ClampMagnitude(const Vector3 vector, const  float maxLength);
        static Vector3 Cross(const Vector3 lhs, const  Vector3 rhs);
        static float Distance(const Vector3 lhs, const  Vector3 rhs);
        static float Dot(const Vector3 lhs, const  Vector3 rhs);
        static Vector3 Lerp(const Vector3 lhs, const  Vector3 rhs, float t);
        static Vector3 LerpUnclamped(const Vector3 lhs, const  Vector3 rhs, const  float t);
        static Vector3 Max(const Vector3 lhs, const  Vector3 rhs);
        static Vector3 Min(const Vector3 lhs, const  Vector3 rhs);
        static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta);
        static Vector3 Normalize(Vector3 value);
        static void OrthoNormalize(Vector3& normal, Vector3& tangent);
        static Vector3 Project(const Vector3 vector, const  Vector3 onNormal);
        static Vector3 ProjectOnPlane(const Vector3 vector, const  Vector3 planeNormal);
        static Vector3 Reflect(const Vector3 inDirection, const  Vector3 inNormal);
        static Vector3 RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta);
        static Vector3 Scale(const Vector3 a, const  Vector3 b);
        static float SignedAngle(Vector3 from, Vector3 to, Vector3 axis);
        static Vector3 Slerp(Vector3 a, Vector3 b, float t);
        static Vector3 SlerpUnclamped(Vector3 lhs, Vector3 rhs, float t);
        static Vector3 SmoothDamp();
        /*
        ----------------------------------------------------------------------------------------
        Normal Methods
        ----------------------------------------------------------------------------------------
        */
        float magnitude() const;
        float sqrMagnitude() const;
        void Set(float new_x, float new_y, float new_z);
        /*
        ----------------------------------------------------------------------------------------
        Operators
        ----------------------------------------------------------------------------------------
        */

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
        Vector3 operator+(const Vector3& other) const {
            return Vector3{
                x + other.x,
                y + other.y,
                z + other.z
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





        inline float operator[](int i) const {assert((i >= 0) && (i < 3));return (&x)[i];}
        inline float& operator[](int i) { assert((i >= 0) && (i < 3));return (&x)[i]; }





    };

}
#endif