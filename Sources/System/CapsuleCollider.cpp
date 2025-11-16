#include <System/CapsuleCollider.hpp>
#include <System/GameObject.hpp>
#include <System/Vector3.hpp>
#include <System/Mathf.hpp>
#include <System/Bounds.hpp>

namespace System {
    Component* CapsuleCollider::Clone() const {
        CapsuleCollider* collider = new CapsuleCollider();
        collider->center = center;
        collider->radius = radius;
        collider->height = height;
        collider->direction = direction;
        return collider;
    }

    Vector3 CapsuleCollider::ClosestPoint(Vector3 position) {
        Vector3 p1 = transform()->GetPosition() + center + Vector3::up * (height / 2 - radius);
        Vector3 p2 = transform()->GetPosition() + center - Vector3::up * (height / 2 - radius);
        Vector3 pointOnLine = p1 + (p2 - p1).normalized() * Mathf::Clamp(Vector3::Dot(position - p1, (p2 - p1).normalized()), 0, (p2 - p1).magnitude());
        return pointOnLine + (position - pointOnLine).normalized() * radius;
    }

    Vector3 CapsuleCollider::ClosestPointOnBounds(Vector3 position) {
        Vector3 size = Vector3(radius * 2, height, radius * 2);
        if (direction == 0) { // X-axis
            size = Vector3(height, radius * 2, radius * 2);
        }
        else if (direction == 2) { // Z-axis
            size = Vector3(radius * 2, radius * 2, height);
        }
        Bounds bounds(transform()->GetPosition() + center, size);
        return bounds.ClosestPoint(position);
    }
}
