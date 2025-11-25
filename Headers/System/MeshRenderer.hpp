#ifndef _SYSTEM_MESHRENDERER_HPP
#define _SYSTEM_MESHRENDERER_HPP

#include <System/Component.hpp>
#include <System/Material.hpp>
#include <System/MeshFilter.hpp>
#include <System/GameObject.hpp>
#include <System/Camera.hpp>
#include <System/Graphics/GraphicsHelpers.h>
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
            meshFilter->mesh->UploadMeshData(false);

            System::Graphics::GL::gl_glBindVertexArray(meshFilter->mesh->GetVAO());
            System::Graphics::GL::gl_glDrawElements(System::Graphics::DrawMode::TRIANGLES, meshFilter->mesh->indices.size(), System::Graphics::IndexType::UNSIGNED_INT, 0);//fix
            System::Graphics::GL::gl_glBindVertexArray(0); 
        }
        Component* Clone() const override {
            MeshRenderer* new_renderer = new MeshRenderer();
            new_renderer->material = material;
            return new_renderer;
        }
 
    };
}

#endif