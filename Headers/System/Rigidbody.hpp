#ifndef _SYSTEM_RIGIDBODY_HPP
#define _SYSTEM_RIGIDBODY_HPP

#include "Component.hpp"
#include "Vector3.hpp"

namespace System {
    class Rigidbody : public Component {
    public:
        float mass;
        Vector3 velocity;
        Vector3 force;

        Rigidbody() : mass(1.0f), velocity(0.0f, 0.0f, 0.0f), force(0.0f, 0.0f, 0.0f) {}

        void AddForce(const Vector3& force);
         

        Component* Clone() const override {
            Rigidbody* newRigidbody = new Rigidbody();
            newRigidbody->mass = this->mass;
            return newRigidbody;
        }
    };
}

#endif
