#include <System/Vector2.h>
#include <System/Mathf.h>

namespace System {

    Vector2::Vector2() {
        this->x = 0.0f; this->y = 0.0f;
    }
    Vector2::Vector2(float x, float y) {
        this->x = x; this->y = y;
    }
    /*
    ----------------------------------------------------------------------------------------
    Static Methods
    ----------------------------------------------------------------------------------------
    */
float Vector2::Angle(Vector2 from, Vector2 to){
    const float kEpsilonNormalSqrt = 1e-15f; // Small epsilon value
    float denominator = System::Mathf::Sqrt(from.SqrMagnitude() * to.SqrMagnitude());
    if (denominator < kEpsilonNormalSqrt) {
        return 0.0f;
    }
    float dot = System::Mathf::Clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
    return System::Mathf::Acos(dot) * System::Mathf::Rad2Deg;
}
float Vector2::Distance(const Vector2 lhs, const Vector2 rhs){
    return System::Mathf::Sqrt(
        System::Mathf::Pow(rhs.x - lhs.x, 2) +
        System::Mathf::Pow(rhs.y - lhs.y, 2)
    );
}
float Vector2::Dot(const Vector2 lhs, const Vector2 rhs){
    return (lhs.x * rhs.x + lhs.y * rhs.y);
}
Vector2 Vector2::Lerp(const Vector2 lhs, const Vector2 rhs, float t){
    t = System::Mathf::Clamp(t, 0.0f, 1.0f);
    return LerpUnclamped(lhs, rhs, t);
}
Vector2 Vector2::LerpUnclamped(const Vector2 lhs, const Vector2 rhs, const float t){
    return Vector2(
        (lhs.x + (t * (rhs.x - lhs.x))),
        (lhs.y + (t * (rhs.y - lhs.y)))
    );
}
Vector2 Vector2::Max(const Vector2 lhs, const Vector2 rhs){
    return  Vector2(
        System::Mathf::Max(lhs.x, rhs.x),
        System::Mathf::Max(lhs.y, rhs.y)
    );
}
Vector2 Vector2::Min(const Vector2 lhs, const Vector2 rhs){
    return  Vector2(
        System::Mathf::Min(lhs.x, rhs.x),
        System::Mathf::Min(lhs.y, rhs.y)
    );
}
Vector2 Vector2::Scale(const Vector2 a, const Vector2 b){
    return Vector2(a.x * b.x, a.y * b.y);
}
float Vector2::SignedAngle(Vector2 from, Vector2 to){
    float unsigned_angle = Angle(from, to);
    float sign = System::Mathf::Sign(from.x * to.y - from.y * to.x);
    return unsigned_angle * sign;
}
void Vector2::Set(float newX, float newY){
    x = newX;
    y = newY;
}






float Vector2::magnitude(){
    return System::Mathf::Sqrt(x * x + y * y);
}
float Vector2::SqrMagnitude(){
    return x * x + y * y;
}


}