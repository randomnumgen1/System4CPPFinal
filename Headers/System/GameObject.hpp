#ifndef _SYSTEM_GAMEOBJECT_H
#define _SYSTEM_GAMEOBJECT_H
#include <unordered_map>
#include <typeindex>
#include <string>
#include <cstdint>
#include <vector>
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
        std::unordered_map<std::type_index, std::vector<void*>> components;





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
        T* AddComponent() {
            T* component = new T();
            component->m_gameObject = this;
            component->m_transform = this->transform;
            components[std::type_index(typeid(T))].emplace_back(component);
            component->Awake();
            return component;
        }
        template<typename T>
        T* GetComponent() {
            auto it = components.find(std::type_index(typeid(T)));
            if (it != components.end() && !it->second.empty()) {
                return static_cast<T*>(it->second[0]);
            }
            return nullptr;
        }

        template<typename T>
        std::vector<T*> GetComponents() {
            auto it = components.find(std::type_index(typeid(T)));
            if (it != components.end()) {
                std::vector<T*> result;
                result.reserve(it->second.size());
                for (void* comp : it->second) {
                    result.push_back(static_cast<T*>(comp));
                }
                return result;
            }
            return std::vector<T*>();
        }

        template<typename T>
        bool TryGetComponent(T*& component) {
            component = GetComponent<T>();
            return component != nullptr;
        }







    };
}
#endif