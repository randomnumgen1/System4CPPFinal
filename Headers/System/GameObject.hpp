#ifndef _SYSTEM_GAMEOBJECT_H
#define _SYSTEM_GAMEOBJECT_H
#include <unordered_map>
#include <typeindex>
#include <string>
#include <cstdint>
#include "Transform.hpp"
#include "Scene.hpp"
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
        uint32_t layer;
        std::string tag;
        std::unordered_map<std::type_index, void*> components;





        GameObject(const std::string& name);

        ~GameObject() {
            delete transform;
        }
        /// <summary>
        /// GameObject can only be assigned to one layer at a time.
        /// default is layer 0 (default layer).
        /// </summary>
        /// <param name="newLayer"></param>
        void SetLayer(uint32_t newLayer) {
            if (newLayer != 0 && (newLayer & (newLayer - 1)) != 0) {
                throw std::invalid_argument("Layer must be 0 (default) or have exactly one bit set (i.e., be a power of two).");
            }
            layer = newLayer;
        }


        static GameObject* Find(std::string name) {
            // Not implemented
            return nullptr;
        }
        static GameObject* CreatePrimitive(PrimitiveType type) {
            // Not implemented
            return nullptr;

        }
        static GameObject* Instantiate(GameObject original, Vector3 position, Quaternion rotation) {
            // Not implemented
            return nullptr;
        }











        
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