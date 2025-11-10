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
    };
}
#endif