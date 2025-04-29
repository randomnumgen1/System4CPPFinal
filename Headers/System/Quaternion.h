#ifndef _SYSTEM_QUATERNION_H
#define _SYSTEM_QUATERNION_H
#include <stdexcept>
#include <algorithm>
#include <corecrt_math_defines.h>
#include <System/Vector3.h>
#include <cmath>
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
        Quaternion() {
            *this = Identity();
        }
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
        inline static Quaternion Euler(float x, float y, float z) {
            // Convert Euler angles from degrees to radians
            x = x * 0.0174532925f; // x in radians (pitch)
            y = y * 0.0174532925f; // y in radians (yaw)
            z = z * 0.0174532925f; // z in radians (roll)

            // Calculate half angles
            float halfX = x * 0.5f;
            float halfY = y * 0.5f;
            float halfZ = z * 0.5f;

            // Calculate trigonometric values
            float cosX = std::cos(halfX);
            float sinX = std::sin(halfX);
            float cosY = std::cos(halfY);
            float sinY = std::sin(halfY);
            float cosZ = std::cos(halfZ);
            float sinZ = std::sin(halfZ);

            // Combine rotations using quaternion math
            return Quaternion(
                sinX * cosY * cosZ - cosX * sinY * sinZ, // x
                cosX * sinY * cosZ + sinX * cosY * sinZ, // y
                cosX * cosY * sinZ - sinX * sinY * cosZ, // z
                cosX * cosY * cosZ + sinX * sinY * sinZ  // w
            );
        }

        inline static Quaternion FromToRotation(System::Vector3 fromDirection, System::Vector3 toDirection) {
            throw std::runtime_error("Not Implemented Error.");
        }
        inline static Quaternion Inverse(Quaternion rotation) {
            Quaternion ans;
            float ls = rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z + rotation.w * rotation.w;
            float invNorm = 1.0f / ls;
            ans.x = -rotation.x * invNorm;
            ans.y = -rotation.y * invNorm;
            ans.z = -rotation.z * invNorm;
            ans.w = rotation.w * invNorm;
            return ans;
        }
        inline static Quaternion Lerp(Quaternion q1, Quaternion q2, float t) {
            return LerpUnclamped(q1, q2, std::clamp(t, 0.0f, 1.0f));
        }
        inline static Quaternion LerpUnclamped(Quaternion q1, Quaternion q2, float t) {
            Quaternion r{
                    q1.x + t * (q2.x - q1.x),
                    q1.y + t * (q2.y - q1.y),
                    q1.z + t * (q2.z - q1.z),
                    q1.w + t * (q2.w - q1.w)
            };
            r.Normalize();
            return r;
        }
        inline static Quaternion LookRotation(System::Vector3 forward, System::Vector3 upwards) {
            throw std::runtime_error("Not Implemented Error.");
        }
        inline static Quaternion Normalize(Quaternion q) {
            float mag = std::sqrt(Dot(q, q));
            if (mag < kEpsilon) {
                return Quaternion::Identity();
            }
            return Quaternion(q.x / mag, q.y / mag, q.z / mag, q.w / mag);
        }
        inline static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta) {
            float angle = Quaternion::Angle(from, to);
            if (angle == 0.0f) return to;
            return SlerpUnclamped(from, to, std::min(1.0f, maxDegreesDelta / angle));
        }
        inline static Quaternion Slerp(Quaternion q1, Quaternion q2, float t) {
            return SlerpUnclamped(q1, q2, std::clamp(t, 0.0f, 1.0f));
        }
        inline static Quaternion SlerpUnclamped(Quaternion q1, Quaternion  q2, float t) {
            q1.Normalize();
            q2.Normalize();
            double dot = (double)q1.x * q2.x + (double)q1.y * q2.y + (double)q1.z * q2.z + (double)q1.w * q2.w;
            if (dot < 0.0f) {
                dot *= -1.0f;
                q2.x = -q2.x;
                q2.y = -q2.y;
                q2.z = -q2.z;
                q2.w = -q2.w;
            }
            if (std::abs(dot) < 1e-10) {
                dot = 1e-10;
            }
            double angle = std::acos(dot);
            double sina = std::sin(angle);
            float st0 = (float)(std::sin(angle * (1.0f - t)) / sina);
            float st1 = (float)(std::sin(angle * t) / sina);

            Quaternion r = Quaternion
            {
                 q1.x * st0 + q2.x * st1,
                 q1.y * st0 + q2.y * st1,
                 q1.z * st0 + q2.z * st1,
                 q1.w * st0 + q2.w * st1
            };
            return r;
        }



        inline    void Normalize() {
            *this = Normalize(*this);
        }

        System::Vector3 eulerAngles() const;











        float magnitude() const;
        float sqrMagnitude() const;








        void Set(float newX, float newY, float newZ, float newW);

        inline float operator[](int i) const { return (&x)[i]; }
        inline float& operator[](int i) { return (&x)[i]; }




    };
}
#endif