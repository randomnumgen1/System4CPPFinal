#include "System/Rigidbody.hpp"
#include "System/Scene.hpp"
#include "System/Internal/Physics/BulletPhysicsEngine.hpp"
#include "System/GameObject.hpp"

namespace System {
    void Rigidbody::AddForce(const Vector3& force) {
        this->force += force;
    }

 
}
