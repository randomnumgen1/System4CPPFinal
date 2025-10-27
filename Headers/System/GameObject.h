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

        std::shared_ptr<Transform> transform; 

        GameObject(const std::string& name, System::Transform* parent = nullptr)
            : name(name), transform(std::make_shared<Transform>(parent)) {
        
            if (parent) {
                transform->SetParent(parent);
            }
        }



        static GameObject CreatePrimitive(PrimitiveType type){
        
        
        }
        static GameObject Instantiate(GameObject original, Vector3 position, Quaternion rotation){
        
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