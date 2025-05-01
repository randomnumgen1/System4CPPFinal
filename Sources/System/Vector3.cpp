#include <System/Vector3.h> 
#include <stdexcept>
#include <System/Mathf.h>
namespace System {
 
   const Vector3 Vector3::back(0.0f, 0.0f, -1.0f);
   const Vector3 Vector3::down(0.0f, -1.0f, 0.0f);
   const Vector3 Vector3::forward(0.0f, 0.0f, 1.0f);
   const Vector3 Vector3::left(-1.0f, 0.0f, 0.0f);
   const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);
   const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
   const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
   const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);






    Vector3::Vector3() {
        this->x = 0.0f; this->y = 0.0f; this->z = 0.0f;
    }
    Vector3::Vector3(float x, float y, float z) {
        this->x = x; this->y = y; this->z = z;
    }

    /*
    ----------------------------------------------------------------------------------------
    Static Methods
    ----------------------------------------------------------------------------------------
    */

    float Vector3::Angle(const Vector3 from, const  Vector3 to) {
        float denominator = (float)System::Mathf::Sqrt(from.sqrMagnitude() * to.sqrMagnitude());
        if (denominator < kEpsilonNormalSqrt) {
            return 0.0f;
        }
        float dot = System::Mathf::Clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
        return System::Mathf::Acos(dot) * System::Mathf::Rad2Deg;
    }
    Vector3 Vector3::ClampMagnitude(const Vector3 vector, const  float maxLength) {
        throw std::runtime_error("Not Implemented Error.");
    }
    Vector3 Vector3::Cross(const Vector3 lhs, const  Vector3 rhs) {
        return Vector3(
            (lhs.y * rhs.z) - (lhs.z * rhs.y),
            (lhs.z * rhs.x) - (lhs.x * rhs.z),
            (lhs.x * rhs.y) - (lhs.y * rhs.x));
    }
    float Vector3::Distance(const Vector3 lhs, const  Vector3 rhs) {
        return System::Mathf::Sqrt(
            System::Mathf::Pow(rhs.x - lhs.x, 2) +
            System::Mathf::Pow(rhs.y - lhs.y, 2) +
            System::Mathf::Pow(rhs.z - lhs.z, 2)
        );
    }
    float Vector3::Dot(const Vector3 lhs, const  Vector3 rhs) {
        return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
    }
    Vector3 Vector3::Lerp(const Vector3 lhs, const  Vector3 rhs,const float t) {
       float t1 = System::Mathf::Clamp(t, 0.0f, 1.0f);
        return LerpUnclamped(lhs,rhs,t1);
    }
    Vector3 Vector3::LerpUnclamped(const Vector3 lhs, const  Vector3 rhs, const  float t) {
        return Vector3(
            (lhs.x + (t * (rhs.x - lhs.x))),
            (lhs.y + (t * (rhs.y - lhs.y))),
            (lhs.z + (t * (rhs.z - lhs.z)))
        );
    }
    Vector3 Vector3::Max(const Vector3 lhs, const  Vector3 rhs) {
        return  Vector3(
            std::max(lhs.x, rhs.x),
            std::max(lhs.y, rhs.y),
            std::max(lhs.z, rhs.z)
        );
    }
    Vector3 Vector3::Min(const Vector3 lhs, const  Vector3 rhs) {
        return  Vector3(
            std::min(lhs.x, rhs.x),
            std::min(lhs.y, rhs.y),
            std::min(lhs.z, rhs.z)
        );
    }
    Vector3 Vector3::MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta) {
        Vector3 toVector = target - current;
        float sqdist = toVector.sqrMagnitude();
        if (sqdist == 0.0f || sqdist <= maxDistanceDelta * maxDistanceDelta) {
            return target;
        }
        float scaleFactor = maxDistanceDelta / System::Mathf::Sqrt(sqdist);
        return  (current + toVector * scaleFactor);
    }
    Vector3 Vector3::Normalize(const Vector3 value) {
        double magnitude = value.magnitude();
        if (magnitude == 0.0) {
            return value;
        }
        return Vector3(value.x / magnitude, value.y / magnitude, value.z / magnitude);
    }
    void Vector3::OrthoNormalize(Vector3& normal, Vector3& tangent) {
        throw std::runtime_error("Not Implemented Error.");
    }
    Vector3 Vector3::Project(const Vector3 vector, const  Vector3 onNormal) {
        return (onNormal * (Dot(vector, onNormal) / Dot(onNormal, onNormal)));
    }
    Vector3 Vector3::ProjectOnPlane(const Vector3 vector, const  Vector3 planeNormal) {
        Vector3 projection = Project(vector, planeNormal);
        return vector - projection;

    }
    Vector3 Vector3::Reflect(const Vector3 inDirection, const  Vector3 inNormal) {
        float factor = -2.0f * Dot(inNormal, inDirection);
        return Vector3{
            factor * inNormal.x + inDirection.x,
            factor * inNormal.y + inDirection.y,
            factor * inNormal.z + inDirection.z
        };
    }
    Vector3 Vector3::RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta) {
        throw std::runtime_error("Not Implemented Error.");
    }
    Vector3 Vector3::Scale(const Vector3 a, const  Vector3 b) {
        return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
    }
    float Vector3::SignedAngle(Vector3 from, Vector3 to, Vector3 axis) {
        throw std::runtime_error("Not Implemented Error.");
    }
    Vector3 Vector3::Slerp(Vector3 a, Vector3 b, float t) {
        throw std::runtime_error("Not Implemented Error.");
    }
    Vector3 Vector3::SlerpUnclamped(Vector3 lhs, Vector3 rhs, float t) {
        throw std::runtime_error("Not Implemented Error.");
    }
    Vector3 Vector3::SmoothDamp() {
        throw std::runtime_error("Not Implemented Error.");
    }
    /*
    ----------------------------------------------------------------------------------------
    Normal Methods
    ----------------------------------------------------------------------------------------
    */
    float Vector3::magnitude() const {
        return System::Mathf::Sqrt(x * x + y * y + z * z);
    }
    float Vector3::sqrMagnitude() const {
        return x * x + y * y + z * z;
    }
    void Vector3::Set(float new_x, float new_y, float new_z){
        x = new_x;
        y = new_y;
        z = new_z;
    }
    void Vector3::Normalize(){
        float magnitude1 = magnitude();
        if (magnitude1 == 0.0) return; // Prevent division by zero
        x /= magnitude1;
        y /= magnitude1;
        z /= magnitude1;
    }
    Vector3 Vector3::normalized(){
        float magnitude1 = magnitude();
        if (magnitude1 == 0.0) return *this; // Return original if magnitude is zero
        return Vector3(x / magnitude1, y / magnitude1, z / magnitude1);

    }










}