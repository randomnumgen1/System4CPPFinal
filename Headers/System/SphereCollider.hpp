#ifndef _SYSTEM_SPHERECOLLIDER_H
#define _SYSTEM_SPHERECOLLIDER_H

#include <System/Collider.hpp>
#include <System/Vector3.hpp>

namespace System {
    class SphereCollider : public System::Collider {
    public:
        Vector3 center;
        float radius;
    };
}
#endif