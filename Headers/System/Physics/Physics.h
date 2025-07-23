#ifndef _SYSTEM_PHYSICS_H
#define _SYSTEM_PHYSICS_H

#include <System/Vector3.hpp>
#include <cstdint>
#include <System/Mathf.hpp>
#include <memory>
#include <System/Mesh.h>


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
        float distance;
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
        static void Setgravity(const Vector3& gravity);


        static void GenerateCollider(System::Mesh mesh){
#if defined(SYSTEM_PHYSICS_BULLET)
            btConvexHullComputer ch;
            ch.compute(&vertices[0].x(), sizeof(btVector3), vertices.size(), 0.0f, 0.0f);

            // Access hull points:
            for (int i = 0; i < ch.vertices.size(); ++i) {
                btVector3 pt(ch.vertices[i]);
                // Use pt to build your new mesh
            }
#endif
        }





    };
}
#endif