#ifndef _SYSTEM_BOUNDS_H
#define _SYSTEM_BOUNDS_H

#include "System/Vector3.hpp"

namespace System {
    struct Bounds {
    public:
        Vector3 center;
        Vector3 extents;
        Vector3 max;
        Vector3 min;
        Vector3 size;
        Bounds() : center(0, 0, 0), extents(0, 0, 0), max(0, 0, 0), min(0, 0, 0), size(0, 0, 0) {}

        Bounds(Vector3 new_center, Vector3 new_size){
            center = new_center;
            size = new_size;
            extents = new_size * 0.5f;
            min = center - extents;
            max = center + extents;

        }
        bool Contains(Vector3 point) const {
            return (point.x >= min.x && point.x <= max.x) && (point.y >= min.y && point.y <= max.y) && (point.z >= min.z && point.z <= max.z);
        }
        float SqrDistance(const Vector3 point) const {
            float dx = std::max(min.x - point.x, 0.0f) + std::max(point.x - max.x, 0.0f);
            float dy = std::max(min.y - point.y, 0.0f) + std::max(point.y - max.y, 0.0f);
            float dz = std::max(min.z - point.z, 0.0f) + std::max(point.z - max.z, 0.0f);
            return dx * dx + dy * dy + dz * dz;
        }
    };
}


#endif