#ifndef _SYSTEM_PHYSICS_H
#define _SYSTEM_PHYSICS_H

#include <System/Vector3.hpp>
#include <cstdint>
#include <System/Mathf.hpp>
#include <memory>

#if defined(SYSTEM_PHYSICS_BULLET)
#include <bullet/btBulletDynamicsCommon.h>
#endif

namespace System{
    struct Ray {
    public:
        Vector3 origin;
        Vector3 direction;
        Ray(Vector3 origin, Vector3 direction){
            this->origin = origin;
            this->direction = direction;
        }
    };
    class Physics{
    private:
#if defined(SYSTEM_PHYSICS_BULLET)
        std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
        std::unique_ptr<btCollisionDispatcher> dispatcher;
        std::unique_ptr<btBroadphaseInterface> broadphase;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
        std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
#endif
    public:
        Physics() {
#if defined(SYSTEM_PHYSICS_BULLET)
            collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
            dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
            broadphase = std::make_unique<btDbvtBroadphase>();
            solver = std::make_unique<btSequentialImpulseConstraintSolver>();
            dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get());
            dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
#endif
        }
        static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance = System::Mathf::Infinity, uint32_t layerMask = -1) {
#if defined(SYSTEM_PHYSICS_BULLET)
            if (!dynamicsWorld) return false;

            btVector3 btOrigin(origin.x, origin.y, origin.z);
            btVector3 btDirection(direction.x, direction.y, direction.z);
            btVector3 btEnd = btOrigin + btDirection.normalized() * maxDistance;

            btCollisionWorld::ClosestRayResultCallback rayCallback(btOrigin, btEnd);
            dynamicsWorld->rayTest(btOrigin, btEnd, rayCallback);

            if (rayCallback.hasHit()) {
                return true;
            }
#endif
            return false;
        }
        static void Simulate(float step){
#if defined(SYSTEM_PHYSICS_BULLET)
            dynamicsWorld->stepSimulation(deltaTime);
#endif
        }









    };
}
#endif