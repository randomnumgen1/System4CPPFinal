#include <System/Bounds.hpp>
#include <algorithm>
namespace System{
    bool Bounds::Contains(Vector3 point) const{
        return (point.x >= min.x && point.x <= max.x) && (point.y >= min.y && point.y <= max.y) && (point.z >= min.z && point.z <= max.z);
    }
    float Bounds::SqrDistance(const Vector3 point) const{
        float dx = std::max(min.x - point.x, 0.0f) + std::max(point.x - max.x, 0.0f);
        float dy = std::max(min.y - point.y, 0.0f) + std::max(point.y - max.y, 0.0f);
        float dz = std::max(min.z - point.z, 0.0f) + std::max(point.z - max.z, 0.0f);
        return dx * dx + dy * dy + dz * dz;
    }
    Vector3 Bounds::ClosestPoint(const Vector3 point) const{
        float x = std::max(min.x, std::min(point.x, max.x));
        float y = std::max(min.y, std::min(point.y, max.y));
        float z = std::max(min.z, std::min(point.z, max.z));
        return Vector3(x, y, z);
    }
}