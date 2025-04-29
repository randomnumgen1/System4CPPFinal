#include "System/Quaternion.h"

namespace System {



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








    System::Vector3 System::Quaternion::eulerAngles() const {
        System::Vector3 euler;

        // Calculate pitch (x-axis rotation)
        float sinPitch = 2.0f * (w * x + y * z);
        float cosPitch = 1.0f - 2.0f * (x * x + y * y);
        euler.x = std::atan2(sinPitch, cosPitch); // Pitch

        // Calculate yaw (y-axis rotation)
        float sinYaw = 2.0f * (w * y - z * x);
        if (std::abs(sinYaw) >= 1.0f) {
            euler.y = std::copysign(3.14159265358979323846f / 2.0f, sinYaw); // Use 90 degrees if out of bounds
        }
        else {
            euler.y = std::asin(sinYaw); // Yaw
        }

        // Calculate roll (z-axis rotation)
        float sinRoll = 2.0f * (w * z + x * y);
        float cosRoll = 1.0f - 2.0f * (y * y + z * z);
        euler.z = std::atan2(sinRoll, cosRoll); // Roll

        // Convert radians to degrees (if required)
        euler.x *= 57.2957795f; // Rad to Deg
        euler.y *= 57.2957795f; // Rad to Deg
        euler.z *= 57.2957795f; // Rad to Deg

        return euler;
    }



    inline float System::Quaternion::magnitude() const {
        return std::sqrt(sqrMagnitude());
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