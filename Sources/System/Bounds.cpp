#include <System/Bounds.hpp>
#include <algorithm>
namespace System{
    bool Bounds::Contains(Vector3 point) const{
        return (point.x >= m_min.x && point.x <= m_max.x) && (point.y >= m_min.y && point.y <= m_max.y) && (point.z >= m_min.z && point.z <= m_max.z);
    }
    float Bounds::SqrDistance(const Vector3 point) const{
        float dx = std::max<float>(m_min.x - point.x, 0.0f) + std::max<float>(point.x - m_max.x, 0.0f);
        float dy = std::max<float>(m_min.y - point.y, 0.0f) + std::max<float>(point.y - m_max.y, 0.0f);
        float dz = std::max<float>(m_min.z - point.z, 0.0f) + std::max<float>(point.z - m_max.z, 0.0f);
        return dx * dx + dy * dy + dz * dz;
    }
    Vector3 Bounds::ClosestPoint(const Vector3 point) const{
        float x = std::max<float>(m_min.x, std::min<float>(point.x, m_max.x));
        float y = std::max<float>(m_min.y, std::min<float>(point.y, m_max.y));
        float z = std::max<float>(m_min.z, std::min<float>(point.z, m_max.z));
        return Vector3(x, y, z);
    }
}