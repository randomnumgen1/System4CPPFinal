#include "System/GameObject.hpp"
#include <System/Scene.hpp>
#include <System/Transform.hpp>
#include <algorithm>
#include <System/Mesh.hpp>
#include <System/MeshFilter.hpp>
#include <System/MeshRenderer.hpp>

namespace System {
    std::vector<GameObject*> GameObject::allGameObjects;

    GameObject::GameObject(const std::string& name) : name(name), m_layer(0), m_activeSelf(true), m_activeInHierarchy(true) {
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
        Mesh* mesh = new Mesh();
        if (PrimitiveType::Cube == type) {
            std::vector<Vector3> vertices = {
                // Front face
                Vector3(-0.5f, -0.5f,  0.5f),
                Vector3(0.5f, -0.5f,  0.5f),
                Vector3(0.5f,  0.5f,  0.5f),
                Vector3(-0.5f,  0.5f,  0.5f),

                // Back face
                Vector3(-0.5f, -0.5f, -0.5f),
                Vector3(0.5f, -0.5f, -0.5f),
                Vector3(0.5f,  0.5f, -0.5f),
                Vector3(-0.5f,  0.5f, -0.5f),

                // Top face
                Vector3(-0.5f,  0.5f,  0.5f),
                Vector3(0.5f,  0.5f,  0.5f),
                Vector3(0.5f,  0.5f, -0.5f),
                Vector3(-0.5f,  0.5f, -0.5f),

                // Bottom face
                Vector3(-0.5f, -0.5f,  0.5f),
                Vector3(0.5f, -0.5f,  0.5f),
                Vector3(0.5f, -0.5f, -0.5f),
                Vector3(-0.5f, -0.5f, -0.5f),

                // Right face
                Vector3(0.5f, -0.5f,  0.5f),
                Vector3(0.5f, -0.5f, -0.5f),
                Vector3(0.5f,  0.5f, -0.5f),
                Vector3(0.5f,  0.5f,  0.5f),

                // Left face
                Vector3(-0.5f, -0.5f,  0.5f),
                Vector3(-0.5f, -0.5f, -0.5f),
                Vector3(-0.5f,  0.5f, -0.5f),
                Vector3(-0.5f,  0.5f,  0.5f)
            };

            std::vector<int> triangles = {
                // Front face (CW)
                0, 2, 1,
                2, 0, 3,

                // Back face (CW)
                4, 6, 5,
                6, 4, 7,

                // Top face (CW)
                8, 10, 9,
                10, 8, 11,

                // Bottom face (CW)
                12, 14, 13,
                14, 12, 15,

                // Right face (CW)
                16, 18, 17,
                18, 16, 19,

                // Left face (CW)
                20, 22, 21,
                22, 20, 23
            };

            mesh->SetVertices(vertices);
            mesh->SetTriangles(triangles, 0);

            std::vector<Vector3> normals = {
                // Front face (+Z)
                Vector3(0,0,1), Vector3(0,0,1), Vector3(0,0,1), Vector3(0,0,1),
                // Back face (–Z)
                Vector3(0,0,-1), Vector3(0,0,-1), Vector3(0,0,-1), Vector3(0,0,-1),
                // Top face (+Y)
                Vector3(0,1,0), Vector3(0,1,0), Vector3(0,1,0), Vector3(0,1,0),
                // Bottom face (–Y)
                Vector3(0,-1,0), Vector3(0,-1,0), Vector3(0,-1,0), Vector3(0,-1,0),
                // Right face (+X)
                Vector3(1,0,0), Vector3(1,0,0), Vector3(1,0,0), Vector3(1,0,0),
                // Left face (–X)
                Vector3(-1,0,0), Vector3(-1,0,0), Vector3(-1,0,0), Vector3(-1,0,0)
            };
            mesh->SetNormals(normals);
            mesh->RecalculateBounds();


            std::cout << "normal count" << mesh->normals.size() << std::endl;
            std::cout << "vertices count" << mesh->vertices.size() << std::endl;
            std::cout << "indices count" << mesh->indices.size() << std::endl;




            GameObject* go = new GameObject("Cube");
            MeshFilter* mf = go->AddComponent<MeshFilter>();
            mf->mesh = mesh;
            go->AddComponent<MeshRenderer>();
            return go;
        }
        else if(PrimitiveType::Sphere == type){
            const int stacks = 16;   // latitude subdivisions
            const int slices = 32;   // longitude subdivisions
            const float radius = 0.5f;

            std::vector<Vector3> vertices;
            std::vector<int> indices;

            // Generate vertices
            for (int i = 0; i <= stacks; ++i) {
                float phi = M_PI * i / stacks; // latitude angle 
                for (int j = 0; j <= slices; ++j) {
                    float theta = 2.0f * M_PI * j / slices; // longitude angle [0,  ]

                    float x = radius * sin(phi) * cos(theta);
                    float y = radius * cos(phi);
                    float z = radius * sin(phi) * sin(theta);

                    vertices.emplace_back(x, y, z);
                }
            }
            // Generate indices (two triangles per quad)
            for (int i = 0; i < stacks; ++i) {
                for (int j = 0; j < slices; ++j) {
                    int first = i * (slices + 1) + j;
                    int second = first + slices + 1;

                    indices.push_back(first);
                    indices.push_back(second);
                    indices.push_back(first + 1);

                    indices.push_back(second);
                    indices.push_back(second + 1);
                    indices.push_back(first + 1);
                }
            }


            mesh->SetVertices(vertices);
            mesh->SetTriangles(indices, 0);

            std::vector<Vector3> normals;
            for (auto& v : vertices) {
                normals.push_back(v.normalized());
            }
            mesh->SetNormals(normals);
            mesh->RecalculateBounds();

            GameObject* go = new GameObject("Sphere");
            MeshFilter* mf = go->AddComponent<MeshFilter>();
            mf->mesh = mesh;
            go->AddComponent<MeshRenderer>();
            return go;



        }
        // Not implemented
        return nullptr;
    }
    GameObject::GameObject(const GameObject& other) : name(other.name), m_layer(other.m_layer), m_activeSelf(other.m_activeSelf), m_activeInHierarchy(other.m_activeInHierarchy) {
        transform = new Transform(*other.transform);
        transform->m_gameObject = this;
        allGameObjects.push_back(this);

        for (const auto& pair : other.components) {
            for (void* comp : pair.second) {
                if (!comp) continue;
                if (dynamic_cast<Transform*>(static_cast<Component*>(comp))) continue;
                Component* newComp = static_cast<Component*>(comp)->Clone();
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
