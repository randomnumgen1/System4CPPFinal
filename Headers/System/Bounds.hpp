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
        bool Contains(Vector3 point) const;
        float SqrDistance(const Vector3 point) const;
        Vector3 ClosestPoint(const Vector3 point) const; 
    };
}


#endif