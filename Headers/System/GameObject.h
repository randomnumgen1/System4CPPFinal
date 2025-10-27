#ifndef _SYSTEM_GAMEOBJECT_H
#define _SYSTEM_GAMEOBJECT_H
#include <unordered_map>
#include <typeindex>
#include <string>
#include "Transform.h"
namespace System {
    enum class PrimitiveType {
        Sphere,
        Capsule,
        Cylinder,
        Cube,
        Plane,
        Quad
    };
    struct GameObject {
        std::string name;

        Transform* transform;

        GameObject(const std::string& name) : name(name) {
            transform = new Transform();
            transform->gameObject = this;
        }

        ~GameObject() {
            delete transform;
        }



        static GameObject CreatePrimitive(PrimitiveType type) {


        }
        static GameObject Instantiate(GameObject original, Vector3 position, Quaternion rotation) {

        }











        std::unordered_map<std::type_index, void*> components;
        template<typename T>
        void AddComponent(T* component) {
            components[std::type_index(typeid(T))] = component;
        }
        template<typename T>
        T* GetComponent() {
            auto it = components.find(std::type_index(typeid(T)));
            if (it != components.end()) {
                return static_cast<T*>(it->second);
            }
            return nullptr;
        }

        template<typename T>
        bool TryGetComponent(T*& component) {
            component = GetComponent<T>();
            return component != nullptr;
        }
    };
}
#endif