#ifndef _SYSTEM_PHYSICS_H
#define _SYSTEM_PHYSICS_H

#include <System/Vector3.hpp>
#include <cstdint>
#include <System/Mathf.hpp>
#include <System/Mesh.hpp>

namespace System {
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

    class Physics {
    public:
        static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance = System::Mathf::Infinity, uint32_t layerMask = -1);
        static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, uint32_t layerMask);
        static void Simulate(float step);
        static void Setgravity(const Vector3& gravity);
        static void GenerateCollider(const System::Mesh& mesh);
    };
}
#endif