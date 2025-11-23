#include <System/Internal/Physics/DummyPhysicsEngine.hpp>



void System::DummyPhysicsEngine::Initialize(){
    std::cout << "DummyPhysicsEngine::Initialize" << std::endl;
}

void System::DummyPhysicsEngine::Shutdown(){
    std::cout << "DummyPhysicsEngine::Shutdown" << std::endl;
}

bool System::DummyPhysicsEngine::Raycast(Vector3 origin, Vector3 direction, float maxDistance, uint32_t layerMask){
    std::cout << "DummyPhysicsEngine::Raycast" << std::endl;
    return false;
}

bool System::DummyPhysicsEngine::Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, uint32_t layerMask){
    std::cout << "DummyPhysicsEngine::Raycast" << std::endl;
    return false;
}

void System::DummyPhysicsEngine::Simulate(float step){
    std::cout << "DummyPhysicsEngine::Simulate" << std::endl;
}

void System::DummyPhysicsEngine::Setgravity(const Vector3& gravity){
    std::cout << "DummyPhysicsEngine::Setgravity" << std::endl;
}

void System::DummyPhysicsEngine::GenerateCollider(const System::Mesh& mesh){
    std::cout << "DummyPhysicsEngine::GenerateCollider" << std::endl;
}
