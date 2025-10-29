#include "System/GameObject.hpp"
#include <System/Scene.hpp>
#include <System/Transform.hpp>


System::GameObject::GameObject(const std::string& name) : name(name) {
    transform = new Transform();
    transform->m_gameObject = this;
    //automatically parent new objects to the scene root.
    if (Scene::root && this != Scene::root) {
        transform->SetParent(Scene::root->transform);
    }
}
