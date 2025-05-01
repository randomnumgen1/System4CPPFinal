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
Vector2 Vector2::Lerp(Vector2 lhs, Vector2 rhs, Vector2 t){
    return Vector2(
        (lhs.x + (t.x * (rhs.x - lhs.x))), 
        (lhs.y + (t.y * (rhs.y - lhs.y)))
    );
}
Vector2 Vector2::LerpUnclamped(const Vector2 lhs, const Vector2 rhs, const float t){
    return Vector2(
        (lhs.x + (t * (rhs.x - lhs.x))),
        (lhs.y + (t * (rhs.y - lhs.y)))
    );
}
Vector2 Vector2::InverseLerp(Vector2 a, Vector2 b, Vector2 v){
    return (v - a) / (b - a);
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
bool Vector2::PointInsideTriangle(Vector2 point, Vector2 a, Vector2 b, Vector2 c, float aMargin, float bMargin, float cMargin) {
        Vector2 v0 = c - a;
        Vector2 v1 = b - a;
        Vector2 v2 = point - a;
        float dot00 = Vector2::Dot(v0,v0);
        float dot01 = Vector2::Dot(v0,v1);
        float dot02 = Vector2::Dot(v0,v2);
        float dot11 = Vector2::Dot(v1,v1);
        float dot12 = Vector2::Dot(v1,v2);
        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
        return (u >= -aMargin && v >= -bMargin && (u + v) <= (1.0f + cMargin));
}
Vector2 Vector2::Remap(Vector2 value, Vector2 oldMin, Vector2 oldMax, Vector2 newMin, Vector2 newMax){
    return Lerp(newMin, newMax, InverseLerp(oldMin, oldMax, value));
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






float Vector2::magnitude() const{
    return System::Mathf::Sqrt(x * x + y * y);
}
float Vector2::SqrMagnitude() const{
    return x * x + y * y;
}
float Vector2::MinScalar() const{
    return x < y ? x : y;
}
float Vector2::MaxScalar() const{
    return x > y ? x : y;
}


}