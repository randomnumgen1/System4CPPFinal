#include "System/GameObject.hpp"
#include <System/Scene.hpp>
#include <System/Transform.hpp>
#include <algorithm>

namespace System {
    std::vector<GameObject*> GameObject::allGameObjects;

    GameObject::GameObject(const std::string& name) : name(name), m_layer(0) {
        transform = new Transform();
        transform->m_gameObject = this;
        //automatically parent new objects to the scene root.
        if (Scene::root && this != Scene::root) {
            transform->SetParent(Scene::root->transform);
        }
        allGameObjects.push_back(this);
    }

    void GameObject::SetLayer(uint32_t newLayer){
        if (newLayer != 0 && (newLayer & (newLayer - 1)) != 0) {
            throw std::invalid_argument("Layer must be 0 (default) or have exactly one bit set (i.e., be a power of two).");
        }
        m_layer = newLayer;
    }
    GameObject* GameObject::Find(std::string name) {
        auto it = std::find_if(allGameObjects.begin(), allGameObjects.end(),
                               [&](GameObject* go) { return go->name == name; });
        return (it != allGameObjects.end()) ? *it : nullptr;
    }
    GameObject* GameObject::CreatePrimitive(PrimitiveType type) {
        // Not implemented
        return nullptr;
    }
    GameObject::GameObject(const GameObject& other) : name(other.name), m_layer(other.m_layer) {
        transform = new Transform(*other.transform);
        transform->m_gameObject = this;
        allGameObjects.push_back(this);

        for (const auto& pair : other.components) {
            for (const void* comp : pair.second) {
                if (dynamic_cast<const Transform*>(static_cast<const Component*>(comp))) continue;
                Component* newComp = static_cast<const Component*>(comp)->Clone();
                newComp->m_gameObject = this;
                newComp->m_transform = this->transform;
                components[pair.first].push_back(newComp);
                newComp->Awake();
            }
        }
    }

    GameObject* GameObject::InstantiateRecursive(GameObject* original) {
        if (!original) return nullptr;
        GameObject* newGO = new GameObject(*original);
        for (const Transform* childTransform : original->transform->children) {
            GameObject* newChild = InstantiateRecursive(childTransform->gameObject());
            newChild->transform->SetParent(newGO->transform);
        }
        return newGO;
    }

    GameObject* GameObject::Instantiate(GameObject* original) {
        if (!original) return nullptr;
        GameObject* newGO = InstantiateRecursive(original);
        newGO->name = original->name + "(Clone)";
        if (Scene::root) {
            newGO->transform->SetParent(Scene::root->transform);
        }
        return newGO;
    }

    GameObject* GameObject::Instantiate(GameObject* original, Vector3 position, Quaternion rotation) {
        GameObject* newGO = Instantiate(original);
        if (newGO) {
            newGO->transform->SetPosition(position);
            newGO->transform->SetRotation(rotation);
        }
        return newGO;
    }
}
