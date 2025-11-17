#ifndef _SYSTEM_BOXCOLLIDER_H
#define _SYSTEM_BOXCOLLIDER_H

#include <System/Collider.hpp>
#include <System/Vector3.hpp>

namespace System {
    class BoxCollider : public System::Collider {
    public:
        Vector3 center;
        Vector3 size;
        Component* Clone() const override;
        Bounds bounds() override;
        Vector3 ClosestPoint(Vector3 position) override;
        Vector3 ClosestPointOnBounds(Vector3 position) override;
    };
}
#endif