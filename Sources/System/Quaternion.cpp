#include <System/Quaternion.hpp>
#include <System/Mathf.hpp>
#include <stdexcept>
#include <cmath>
namespace System {


    System::Quaternion::Quaternion() {
        *this = Identity();
    }
    System::Quaternion::Quaternion(float _x, float _y, float _z, float _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    /*
    ----------------------------------------------------------------------------------------
    Static Methods
    ----------------------------------------------------------------------------------------
    */
    float System::Quaternion::Angle(Quaternion a, Quaternion b) {
        throw std::runtime_error("Not Implemented Error.");
    }
    Quaternion System::Quaternion::AngleAxis(float angle, System::Vector3 axis) {
        throw std::runtime_error("Not Implemented Error.");
    }
    float System::Quaternion::Dot(Quaternion a, Quaternion b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }
    Quaternion System::Quaternion::Euler(float x, float y, float z) {
        // Convert Euler angles from degrees to radians
        x = x * 0.0174532925f; // x in radians (pitch)
        y = y * 0.0174532925f; // y in radians (yaw)
        z = z * 0.0174532925f; // z in radians (roll)

        // Calculate half angles
        float halfX = x * 0.5f;
        float halfY = y * 0.5f;
        float halfZ = z * 0.5f;

        // Calculate trigonometric values
        float cosX = Mathf::Cos(halfX);
        float sinX = Mathf::Sin(halfX);
        float cosY = Mathf::Cos(halfY);
        float sinY = Mathf::Sin(halfY);
        float cosZ = Mathf::Cos(halfZ);
        float sinZ = Mathf::Sin(halfZ);

        // Combine rotations using quaternion math
        return Quaternion(
            sinX * cosY * cosZ - cosX * sinY * sinZ, // x
            cosX * sinY * cosZ + sinX * cosY * sinZ, // y
            cosX * cosY * sinZ - sinX * sinY * cosZ, // z
            cosX * cosY * cosZ + sinX * sinY * sinZ  // w
        );
    }
    Quaternion System::Quaternion::FromToRotation(System::Vector3 fromDirection, System::Vector3 toDirection) {
        throw std::runtime_error("Not Implemented Error.");
    }
    Quaternion System::Quaternion::Inverse(Quaternion rotation) {
        Quaternion ans;
        float ls = rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z + rotation.w * rotation.w;
        float invNorm = 1.0f / ls;
        ans.x = -rotation.x * invNorm;
        ans.y = -rotation.y * invNorm;
        ans.z = -rotation.z * invNorm;
        ans.w = rotation.w * invNorm;
        return ans;
    }
    Quaternion System::Quaternion::Lerp(Quaternion q1, Quaternion q2, float t) {
        return LerpUnclamped(q1, q2, Mathf::Clamp(t, 0.0f, 1.0f));
    }
    Quaternion System::Quaternion::LerpUnclamped(Quaternion q1, Quaternion q2, float t) {
        Quaternion r{
                q1.x + t * (q2.x - q1.x),
                q1.y + t * (q2.y - q1.y),
                q1.z + t * (q2.z - q1.z),
                q1.w + t * (q2.w - q1.w)
        };
        r.Normalize();
        return r;
    }
    Quaternion System::Quaternion::LookRotation(System::Vector3 forward, System::Vector3 upwards) {
        throw std::runtime_error("Not Implemented Error.");
    }
    Quaternion System::Quaternion::Normalize(Quaternion q) {
        float mag = Mathf::Sqrt(Dot(q, q));
        if (mag < kEpsilon) {
            return Quaternion::Identity();
        }
        return Quaternion(q.x / mag, q.y / mag, q.z / mag, q.w / mag);
    }
    Quaternion System::Quaternion::RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta) {
        float angle = Quaternion::Angle(from, to);
        if (angle == 0.0f) return to;
        return SlerpUnclamped(from, to, Mathf::Min(1.0f, maxDegreesDelta / angle));
    }
    Quaternion System::Quaternion::Slerp(Quaternion q1, Quaternion q2, float t) {
        return SlerpUnclamped(q1, q2, Mathf::Clamp(t, 0.0f, 1.0f));
    }
    Quaternion System::Quaternion::SlerpUnclamped(Quaternion q1, Quaternion  q2, float t) {
        q1.Normalize();
        q2.Normalize();
        float dot = (float)q1.x * q2.x + (float)q1.y * q2.y + (float)q1.z * q2.z + (float)q1.w * q2.w;
        if (dot < 0.0f) {
            dot *= -1.0f;
            q2.x = -q2.x;
            q2.y = -q2.y;
            q2.z = -q2.z;
            q2.w = -q2.w;
        }
        if (std::abs(dot) < 1e-10f) {
            dot = 1e-10f;
        }
        float angle = Mathf::Acos(dot);
        float sina = Mathf::Sin(angle);
        float st0 = (float)(Mathf::Sin(angle * (1.0f - t)) / sina);
        float st1 = (float)(Mathf::Sin(angle * t) / sina);

        Quaternion r = Quaternion
        {
             q1.x * st0 + q2.x * st1,
             q1.y * st0 + q2.y * st1,
             q1.z * st0 + q2.z * st1,
             q1.w * st0 + q2.w * st1
        };
        return r;
    }








    void System::Quaternion::Normalize() {
        *this = Normalize(*this);
    }
    System::Vector3 System::Quaternion::eulerAngles() const {
        System::Vector3 euler;

        // Calculate pitch (x-axis rotation)
        float sinPitch = 2.0f * (w * x + y * z);
        float cosPitch = 1.0f - 2.0f * (x * x + y * y);
        euler.x = Mathf::Atan2(sinPitch, cosPitch); // Pitch

        // Calculate yaw (y-axis rotation)
        float sinYaw = 2.0f * (w * y - z * x);
        if (std::abs(sinYaw) >= 1.0f) {
            euler.y = std::copysign(3.14159265358979323846f / 2.0f, sinYaw); // Use 90 degrees if out of bounds
        }
        else {
            euler.y = Mathf::Asin(sinYaw); // Yaw
        }

        // Calculate roll (z-axis rotation)
        float sinRoll = 2.0f * (w * z + x * y);
        float cosRoll = 1.0f - 2.0f * (y * y + z * z);
        euler.z = Mathf::Atan2(sinRoll, cosRoll); // Roll

        // Convert radians to degrees (if required)
        euler.x *= 57.2957795f; // Rad to Deg
        euler.y *= 57.2957795f; // Rad to Deg
        euler.z *= 57.2957795f; // Rad to Deg

        return euler;
    }



    inline float System::Quaternion::magnitude() const {
        return Mathf::Sqrt(sqrMagnitude());
    }
    inline float System::Quaternion::sqrMagnitude() const {
        return x * x + y * y + z * z + w * w;
    }

    void System::Quaternion::Set(float newX, float newY, float newZ, float newW) {
        x = newX;
        y = newY;
        z = newZ;
        w = newW;
    }

}