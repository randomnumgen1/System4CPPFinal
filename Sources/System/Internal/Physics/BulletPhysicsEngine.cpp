#include <System/Internal/Physics/BulletPhysicsEngine.hpp>

namespace System {

    void BulletPhysicsEngine::Initialize() { 
    }

    void BulletPhysicsEngine::Shutdown() {
        // TODO: Implement Shutdown
    }
    bool BulletPhysicsEngine::Raycast(Vector3 origin, Vector3 direction, float maxDistance, uint32_t layerMask) {
        // TODO: Implement Raycast
        return false;
    }
    bool BulletPhysicsEngine::Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, uint32_t layerMask) {
        // TODO: Implement Raycast
        return false;
    }
    void BulletPhysicsEngine::Simulate(float step) { 
    }
    void BulletPhysicsEngine::Setgravity(const Vector3& gravity) {

    }
    void BulletPhysicsEngine::GenerateCollider(const System::Mesh& mesh) {

    }

}