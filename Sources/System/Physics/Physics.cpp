#include <System/Physics/Physics.h>
#include <System/Scene.hpp>
#include <System/Internal/Physics/PhysicsEngineInterface.hpp>

namespace System {
    bool Physics::Raycast(Vector3 origin, Vector3 direction, float maxDistance, uint32_t layerMask) {
        return Scene::physicsEngine->Raycast(origin, direction, maxDistance, layerMask);
    }

    bool Physics::Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, uint32_t layerMask) {
        return Scene::physicsEngine->Raycast(origin, direction, hitInfo, maxDistance, layerMask);
    }

    void Physics::Simulate(float step) {
        Scene::physicsEngine->Simulate(step);
    }

    void Physics::Setgravity(const Vector3& gravity) {
        Scene::physicsEngine->Setgravity(gravity);
    }

    void Physics::GenerateCollider(const System::Mesh& mesh) {
        Scene::physicsEngine->GenerateCollider(mesh);
    }
}