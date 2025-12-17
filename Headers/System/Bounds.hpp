#ifndef _SYSTEM_BOUNDS_H
#define _SYSTEM_BOUNDS_H

#include "System/Vector3.hpp"

namespace System {
    struct Bounds {
    public:
        Vector3 center;
        Vector3 extents;
        Vector3 m_max;
        Vector3 m_min;
        Vector3 size;
        Bounds() : center(0.0f, 0.0f, 0.0f), extents(0.0f, 0.0f, 0.0f), m_max(0.0f, 0.0f, 0.0f), m_min(0.0f, 0.0f, 0.0f), size(0.0f, 0.0f, 0.0f) {}

        Vector3 GetMin() const {
            return m_min;
        }
        Vector3 GetMax() const {
            return m_max;
        }


        Bounds(Vector3 new_center, Vector3 new_size){
            center = new_center;
            size = new_size;
            extents = new_size * 0.5f;
            m_min = center - extents;
            m_max = center + extents;

        }
        bool Contains(Vector3 point) const;
        float SqrDistance(const Vector3 point) const;
        Vector3 ClosestPoint(const Vector3 point) const; 
    };
}


#endif