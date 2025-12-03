#ifndef _SYSTEM_QUATERNION_H
#define _SYSTEM_QUATERNION_H

#include <System/Vector3.hpp> 
namespace System {
    struct Quaternion {
    public:
        union {
            float v[4];
            struct {
                float x;
                float y;
                float z;
                float w;
            };
        };
        /*
         ----------------------------------------------------------------------------------------
         Constructors
         ----------------------------------------------------------------------------------------
        */ 
        Quaternion() : x(0), y(0), z(0), w(1) {}
        Quaternion(float _x, float _y, float _z, float _w);

        static constexpr float  kEpsilon = 0.00001F;


        static Quaternion Identity() {
            return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }
        /*
        ----------------------------------------------------------------------------------------
        Static Methods
        ----------------------------------------------------------------------------------------
        */
        static float Angle(Quaternion a, Quaternion b);
        static Quaternion AngleAxis(float angle, System::Vector3 axis);
        static float Dot(Quaternion a, Quaternion b);
        static Quaternion Euler(float x, float y, float z);
        static Quaternion FromToRotation(System::Vector3 fromDirection, System::Vector3 toDirection);
        static Quaternion Inverse(Quaternion rotation);
        static Quaternion Lerp(Quaternion q1, Quaternion q2, float t);
        static Quaternion LerpUnclamped(Quaternion q1, Quaternion q2, float t);
        static Quaternion LookRotation(System::Vector3 forward, System::Vector3 upwards);
        static Quaternion Normalize(Quaternion q);
        static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta);
        static Quaternion Slerp(Quaternion q1, Quaternion q2, float t);
        static Quaternion SlerpUnclamped(Quaternion q1, Quaternion  q2, float t);
        
        /*
        ----------------------------------------------------------------------------------------
        Normal Methods
        ----------------------------------------------------------------------------------------
        */

        void Normalize();
        System::Vector3 eulerAngles() const;
        float magnitude() const;
        float sqrMagnitude() const;
        void Set(float newX, float newY, float newZ, float newW);
        System::Quaternion conjugate() const {
            return System::Quaternion(-x, -y, -z, w);
        }
        /*
        ----------------------------------------------------------------------------------------
        Operators
        ----------------------------------------------------------------------------------------
        */
        Quaternion operator*(const Quaternion& q) const {
            return Quaternion(
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z
            );
        } 
        System::Vector3 operator*(const System::Vector3& v) const {
            Vector3 u(x, y, z);
            float s = w;
            return u * 2.0f * Vector3::Dot(u, v) +
                v * (s * s - Vector3::Dot(u, u)) +
                Vector3::Cross(u, v) * 2.0f * s;
        }

        inline float operator[](int i) const { return (&x)[i]; }
        inline float& operator[](int i) { return (&x)[i]; }




    };
}
#endif