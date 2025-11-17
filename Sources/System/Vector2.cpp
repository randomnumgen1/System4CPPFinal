#include <System/Vector2.hpp>
#include <System/Mathf.hpp>
#include <System/Matrix3x3.hpp>
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
Vector2 Vector2::ClosestPointOnLineSegment(const Vector2& linestart, const Vector2& lineend, const Vector2& point){
    // Calculate the direction vector from origin to end
    Vector2 heading = lineend - linestart;
    // Compute the length of the heading vector
    float magnitudeMax =  heading.magnitude();
    // Normalize the heading vector
    heading = heading.normalized();
    // Calculate the vector from origin to the given point
    Vector2 lhs = point - linestart;
    // Project the lhs vector onto the heading vector
    float dotP = Dot(lhs, heading);
    // Clamp the dot product to the range [0, magnitudeMax]
    dotP = Mathf::Clamp(dotP, 0.0f, magnitudeMax);
    // Calculate and return the nearest point on the line segment
    return linestart + heading * dotP;
}
Vector2 Vector2::ClosestPointOnLineSegment2(const Vector2& linestart, const Vector2& lineend, const Vector2& point) {
    // Compute the vector representing the line segment
    Vector2 segment = lineend - linestart;
    // Compute the squared length of the segment (avoids costly sqrt)
    float segmentLengthSq = segment.SqrMagnitude();
    // Handle degenerate case: line segment is a single point
    if (segmentLengthSq == 0.0f) return linestart;
    // Compute vector from linestart to the external point
    Vector2 toPoint = point - linestart;
    // Project toPoint onto the segment vector and normalize by segment length
    float t = Dot(toPoint, segment) / segmentLengthSq;
    // Clamp the projection scalar to [0, 1] to stay within the segment bounds
    t = Mathf::Clamp(t, 0.0f, 1.0f);
    // Reconstruct the closest point using the clamped projection
    return linestart + segment * t;
}
Vector2 Vector2::ClosestPointOnAnySegment(const Vector2& point, const std::vector<Vector2>& segments) {
    Vector2 closestPoint;
    float minDistSq = std::numeric_limits<float>::max();

    // Ensure we have an even number of points (pairs)
    size_t count = segments.size();
    if (count < 2 || count % 2 != 0) return closestPoint; // Or throw/assert

    for (size_t i = 0; i < count; i += 2) {
        const Vector2& start = segments[i];
        const Vector2& end = segments[i + 1];

        Vector2 candidate =  ClosestPointOnLineSegment2(start, end, point);
        float distSq = (candidate - point).SqrMagnitude();

        if (distSq < minDistSq) {
            minDistSq = distSq;
            closestPoint = candidate;
        }
    }

    return closestPoint;
}
float Vector2::Distance(const Vector2 lhs, const Vector2 rhs){
    float dx = rhs.x - lhs.x;
    float dy = rhs.y - lhs.y;
    return System::Mathf::Sqrt(dx * dx + dy * dy);
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

Vector2 Vector2::normalized() const{
    float mag = magnitude();
    if (mag > 0) {
        return Vector2(x / mag, y / mag);
    }
    return Vector2(0, 0);
}




Vector2 operator*(const Matrix3x3& m, const Vector2& v) {
    float x = m.raw[0] * v.x + m.raw[3] * v.y + m.raw[6];
    float y = m.raw[1] * v.x + m.raw[4] * v.y + m.raw[7];
    return Vector2(x, y);
}





}