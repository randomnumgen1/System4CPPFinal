#ifndef _SYSTEM_COLLIDER_H
#define _SYSTEM_COLLIDER_H

#include <System/Component.hpp>
#include <System/Bounds.hpp>

namespace System {
    class Rigidbody;

    class Collider : public System::Component {
    private:
        bool m_isTrigger = false;
    public:

        Rigidbody* attachedRigidbody;
       
        virtual Bounds bounds() = 0;

        bool GetisTrigger() {
            return m_isTrigger;
        }
        void SetisTrigger(bool value) {
            m_isTrigger = value;
        }
        //
        virtual Vector3 ClosestPoint(Vector3 position) = 0;
        virtual Vector3 ClosestPointOnBounds(Vector3 position) = 0;
    };
}
#endif