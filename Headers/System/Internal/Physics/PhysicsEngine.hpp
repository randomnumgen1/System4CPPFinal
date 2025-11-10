#ifndef _SYSTEM_PHYSICS_ENGINE_HPP
#define _SYSTEM_PHYSICS_ENGINE_HPP

#include <System/Vector3.hpp>
#include <System/Mesh.hpp>
#include <cstdint>

namespace System {
    struct Ray;
    struct RaycastHit;

    class PhysicsEngine {
    public:
        virtual ~PhysicsEngine() = default;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
        virtual bool Raycast(Vector3 origin, Vector3 direction, float maxDistance, uint32_t layerMask) = 0;
        virtual bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, uint32_t layerMask) = 0;
        virtual void Simulate(float step) = 0;
        virtual void Setgravity(const Vector3& gravity) = 0;
        virtual void GenerateCollider(const System::Mesh& mesh) = 0;
    };
}

#endif
