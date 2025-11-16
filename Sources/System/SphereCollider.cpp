#include <System/SphereCollider.hpp>
#include <System/GameObject.hpp>
#include <System/Vector3.hpp>
#include <System/Mathf.hpp>
#include <System/Bounds.hpp>

namespace System {
    Component* SphereCollider::Clone() const {
        SphereCollider* collider = new SphereCollider();
        collider->center = center;
        collider->radius = radius;
        return collider;
    }

    Vector3 SphereCollider::ClosestPoint(Vector3 position) {
        Vector3 p = transform()->GetPosition() + center;
        return p + (position - p).normalized() * radius;
    }

    Vector3 SphereCollider::ClosestPointOnBounds(Vector3 position) {
        Bounds bounds(transform()->GetPosition() + center, Vector3(radius * 2, radius * 2, radius * 2));
        return bounds.ClosestPoint(position);
    }
}
