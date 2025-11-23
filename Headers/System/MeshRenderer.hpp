#ifndef _SYSTEM_MESHRENDERER_HPP
#define _SYSTEM_MESHRENDERER_HPP

#include <System/Component.hpp>
#include <System/Material.hpp>
#include <System/MeshFilter.hpp>
#include <System/GameObject.hpp>
#include <System/Camera.hpp>

namespace System {
    class MeshRenderer : public Component {
    public:
        Material* material;

        MeshRenderer() : material(nullptr) {}

        void Render(Camera* camera) {
            if (!material || !material->shader) {
                return;
            }

            MeshFilter* meshFilter = gameObject()->GetComponent<MeshFilter>();
            if (!meshFilter || !meshFilter->mesh) {
                return;
            }

            material->shader->use();
            material->shader->setMat4("model", transform()->GetLocalToWorldMatrix());
            material->shader->setMat4("view", camera->viewMatrix);
            material->shader->setMat4("projection", camera->projectionMatrix);
            material->shader->setColor("color", material->color);

             
        }
        Component* Clone() const override {
            MeshRenderer* new_renderer = new MeshRenderer();
            new_renderer->material = material;
            return new_renderer;
        }
 
    };
}

#endif