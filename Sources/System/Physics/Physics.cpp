#include <System/Physics/Physics.h>

#if defined(SYSTEM_PHYSICS_BULLET)
#include <bullet/btBulletDynamicsCommon.h>
#endif


namespace System {
    void Physics::Initialize(){
#if defined(SYSTEM_PHYSICS_BULLET)
        collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
        dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
        broadphase = std::make_unique<btDbvtBroadphase>();
        solver = std::make_unique<btSequentialImpulseConstraintSolver>();
        dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get());
        dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
#endif
    }
    void Physics::Shutdown() {
#if defined(SYSTEM_PHYSICS_BULLET)
        dynamicsWorld.reset();
        solver.reset();
        broadphase.reset();
        dispatcher.reset();
        collisionConfiguration.reset();
#endif
    }







    bool Physics::Raycast(Vector3 origin, Vector3 direction, float maxDistance, uint32_t layerMask){
#if defined(SYSTEM_PHYSICS_BULLET)
        if (!dynamicsWorld) return false;

        btVector3 btOrigin(origin.x, origin.y, origin.z);
        btVector3 btDirection(direction.x, direction.y, direction.z);
        btVector3 btEnd = btOrigin + btDirection.normalized() * maxDistance;

        btCollisionWorld::ClosestRayResultCallback rayCallback(btOrigin, btEnd);
        dynamicsWorld->rayTest(btOrigin, btEnd, rayCallback);
        return rayCallback.hasHit();
#else
        return false;
#endif
    }



















    bool Physics::Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, uint32_t layerMask){
#if defined(SYSTEM_PHYSICS_BULLET)
        if (!dynamicsWorld) return false;

        btVector3 btOrigin(origin.x, origin.y, origin.z);
        btVector3 btDirection(direction.x, direction.y, direction.z);
        btVector3 btEnd = btOrigin + btDirection.normalized() * maxDistance;

        btCollisionWorld::ClosestRayResultCallback rayCallback(btOrigin, btEnd);
        dynamicsWorld->rayTest(btOrigin, btEnd, rayCallback);
        if (rayCallback.hasHit()) {
            hitInfo.point = Vector3(rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z());
            hitInfo.normal = Vector3(rayCallback.m_hitNormalWorld.x(), rayCallback.m_hitNormalWorld.y(), rayCallback.m_hitNormalWorld.z());
            hitInfo.distance = (hitInfo.point - origin).magnitude();
            return true;
        }
#endif
        return false;
    }










    void Physics::Simulate(float step){
#if defined(SYSTEM_PHYSICS_BULLET)
    dynamicsWorld->stepSimulation(step);
#endif
}

    void Physics::Setgravity(const Vector3& gravity){
#if defined(SYSTEM_PHYSICS_BULLET)

#endif
    }


}