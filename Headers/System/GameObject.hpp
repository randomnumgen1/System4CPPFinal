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
    private:
        uint32_t m_layer;

        
        bool m_activeSelf;
        //parent may be inactive
        bool m_activeInHierarchy;

    public:

        std::string name;
        Transform* transform;
        
        std::string tag;
        
        std::unordered_map<std::type_index, std::vector<void*>> components;

        static std::vector<GameObject*> allGameObjects;

    private:
        GameObject(const GameObject& other);
        static GameObject* InstantiateRecursive(GameObject* original);

    public:
        GameObject(const std::string& name);

        ~GameObject() {
            delete transform;
            for (auto& pair : components) {
                for (void* comp : pair.second) {
                    delete static_cast<Component*>(comp);
                }
            }
            allGameObjects.erase(std::remove(allGameObjects.begin(), allGameObjects.end(), this), allGameObjects.end());
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
            m_layer = newLayer;
        }
        uint32_t GetLayer() const{
            return m_layer;
        }
        void SetActive(bool value){
            m_activeSelf = value;
            if (value) {
                //loop through all children and update their m_activeInHierarchy to true;
                //remember to check each child's m_activeSelf before setting m_activeInHierarchy
            }else{
                //loop through all children and update their m_activeInHierarchy to false;
            }


            
        }

        static GameObject* Find(std::string name);
        static GameObject* CreatePrimitive(PrimitiveType type);
        static GameObject* Instantiate(GameObject* original);
        static GameObject* Instantiate(GameObject* original, Vector3 position, Quaternion rotation);











        
    





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