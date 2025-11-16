#include <System/Vector3.hpp>
#include "System/BoxCollider.hpp"
#include <algorithm>


namespace System {
    Component* BoxCollider::Clone() const {
        BoxCollider* newCollider = new BoxCollider();
        newCollider->center = this->center;
        newCollider->size = this->size;
        return newCollider;
    }
    Vector3 BoxCollider::ClosestPoint(Vector3 position) {
        Vector3 closestPoint = position;
        Vector3 boxMin = bounds().min();
        Vector3 boxMax = bounds().max();

        closestPoint.x = std::max(boxMin.x, std::min(closestPoint.x, boxMax.x));
        closestPoint.y = std::max(boxMin.y, std::min(closestPoint.y, boxMax.y));
        closestPoint.z = std::max(boxMin.z, std::min(closestPoint.z, boxMax.z));

        return closestPoint;
    }

    Vector3 BoxCollider::ClosestPointOnBounds(Vector3 position) {
        Vector3 closestPoint = position;
        Vector3 boxMin = bounds().min();
        Vector3 boxMax = bounds().max();

        float dx = std::max({ boxMin.x - closestPoint.x, 0.0f, closestPoint.x - boxMax.x });
        float dy = std::max({ boxMin.y - closestPoint.y, 0.0f, closestPoint.y - boxMax.y });
        float dz = std::max({ boxMin.z - closestPoint.z, 0.0f, closestPoint.z - boxMax.z });

        if (dx < dy && dx < dz) {
            closestPoint.x = (closestPoint.x < (boxMin.x + boxMax.x) / 2.0f) ? boxMin.x : boxMax.x;
        }
        else if (dy < dz) {
            closestPoint.y = (closestPoint.y < (boxMin.y + boxMax.y) / 2.0f) ? boxMin.y : boxMax.y;
        }
        else {
            closestPoint.z = (closestPoint.z < (boxMin.z + boxMax.z) / 2.0f) ? boxMin.z : boxMax.z;
        }

        return closestPoint;
    } 
}