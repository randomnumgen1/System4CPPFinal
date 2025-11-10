#ifndef _SYSTEM_BULLET_PHYSICS_ENGINE_HPP
#define _SYSTEM_BULLET_PHYSICS_ENGINE_HPP

#include "PhysicsEngine.hpp"
#include <memory>

#include <bullet/btBulletDynamicsCommon.h>

namespace System {
    class BulletPhysicsEngine : public PhysicsEngine {
    private:
        std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
        std::unique_ptr<btCollisionDispatcher> dispatcher;
        std::unique_ptr<btBroadphaseInterface> broadphase;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
        std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

    public:
        void Initialize() override;
        void Shutdown() override;
        bool Raycast(Vector3 origin, Vector3 direction, float maxDistance, uint32_t layerMask) override;
        bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, uint32_t layerMask) override;
        void Simulate(float step) override;
        void Setgravity(const Vector3& gravity) override;
        void GenerateCollider(const System::Mesh& mesh) override;
    };
}

#endif
