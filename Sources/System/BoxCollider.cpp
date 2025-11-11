#include <System/Vector3.hpp>
#include "System/BoxCollider.hpp"

namespace System {
    Component* BoxCollider::Clone() const {
        BoxCollider* newCollider = new BoxCollider();
        newCollider->center = this->center;
        newCollider->size = this->size;
        return newCollider;
    }
    Vector3 BoxCollider::ClosestPoint(Vector3 position) {
        // TODO: Implement ClosestPoint
        return Vector3(0, 0, 0);
    }

    Vector3 BoxCollider::ClosestPointOnBounds(Vector3 position) {
        // TODO: Implement ClosestPointOnBounds
        return Vector3(0, 0, 0);
    } 
}