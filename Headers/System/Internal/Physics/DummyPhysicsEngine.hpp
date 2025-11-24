#ifndef _SYSTEM_BULLET_PHYSICS_ENGINE_HPP
#define _SYSTEM_BULLET_PHYSICS_ENGINE_HPP

#include "PhysicsEngineInterface.hpp"
#include <memory>


namespace System {
    class DummyPhysicsEngine : public PhysicsEngineInterface {
    private:


    public:
        virtual ~DummyPhysicsEngine() = default;
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
