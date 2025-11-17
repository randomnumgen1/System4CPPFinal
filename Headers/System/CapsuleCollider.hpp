#ifndef _SYSTEM_CAPSULECOLLIDER_H
#define _SYSTEM_CAPSULECOLLIDER_H

#include <System/Collider.hpp>
#include <System/Vector3.hpp>

namespace System {
    class CapsuleCollider : public System::Collider {
    public:
        Vector3 center;
        float radius;
        float height;
        int direction;

        Component* Clone() const override;
        Bounds bounds() override;
        Vector3 ClosestPoint(Vector3 position) override;
        Vector3 ClosestPointOnBounds(Vector3 position) override;
    };
}
#endif