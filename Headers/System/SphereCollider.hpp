#ifndef _SYSTEM_SPHERECOLLIDER_H
#define _SYSTEM_SPHERECOLLIDER_H

#include <System/Collider.hpp>
#include <System/Vector3.hpp>

namespace System {
    class SphereCollider : public System::Collider {
    public:
        Vector3 center;
        float radius;
        Bounds bounds() override {
            return Bounds();
           // Vector3 worldCenter = transform()->GetPosition() + center;
           // Vector3 size(radius * 2, radius * 2, radius * 2);
           // return Bounds(worldCenter, size);
        }

        Component* Clone() const override;
        Vector3 ClosestPoint(Vector3 position) override;
        Vector3 ClosestPointOnBounds(Vector3 position) override;
    };
}
#endif