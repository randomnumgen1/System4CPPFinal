#ifndef _SYSTEM_PHYSICS_H
#define _SYSTEM_PHYSICS_H

#include <System/Vector3.hpp>
#include <cstdint>
#include <System/Mathf.hpp>
#include <memory>


#if defined(SYSTEM_PHYSICS_BULLET)
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
#endif

namespace System{
    struct Ray {
    public:
        Vector3 origin;
        Vector3 direction;
        Ray(const Vector3& origin, const Vector3& direction)
            : origin(origin), direction(direction) {}
    };
    struct RaycastHit {
    public:
        Vector3 point;
        Vector3 normal;
    };
    class Physics{
    private:
#if defined(SYSTEM_PHYSICS_BULLET)
        static std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
        static std::unique_ptr<btCollisionDispatcher> dispatcher;
        static std::unique_ptr<btBroadphaseInterface> broadphase;
        static std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
        static std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
#endif
    public:
        static void Initialize();
        static void Shutdown();
        static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance = System::Mathf::Infinity, uint32_t layerMask = -1);
        static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, uint32_t layerMask);
        static void Simulate(float step);









    };
}
#endif