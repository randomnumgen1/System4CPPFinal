#ifndef _SYSTEM_BOXCOLLIDER_H
#define _SYSTEM_BOXCOLLIDER_H

#include <System/Collider.hpp>
#include <System/Vector3.hpp>

namespace System {
    class BoxCollider : public System::Collider {
    public:
        Vector3 center;
        Vector3 size;
        Bounds bounds() override {
            return Bounds();
           // return Bounds(transform()->GetPosition() + center, size);
        }
        Component* Clone() const override;
        Vector3 ClosestPoint(Vector3 position) override;
        Vector3 ClosestPointOnBounds(Vector3 position) override;
    };
}
#endif