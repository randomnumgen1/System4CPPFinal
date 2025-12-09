#ifndef _SYSTEM_MESHRENDERER_HPP
#define _SYSTEM_MESHRENDERER_HPP

#include <System/Component.hpp>
#include <System/Material.hpp>
#include <System/MeshFilter.hpp>
#include <System/GameObject.hpp>
#include <System/Camera.hpp>
#include <System/Graphics/GraphicsHelpers.h>
#include <System/Light.hpp>

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
           std::cout << "Model Matrix: " << transform()->GetLocalToWorldMatrix().ToString() << std::endl;
            material->shader->setMat4("model", transform()->GetLocalToWorldMatrix().transpose() );


            std::cout << "View: " << camera->GetworldToCameraMatrix().ToString() << std::endl;
            //camera->GetworldToCameraMatrix() or camera->viewMatrix
            material->shader->setMat4("view", camera->GetworldToCameraMatrix());



            std::cout << "Projection: " << camera->projectionMatrix.ToString() << std::endl;
            std::cout << "GPU Projection: " << System::Graphics::GL::GetGPUProjectionMatrix(camera->projectionMatrix).ToString() << std::endl;
            material->shader->setMat4("projection", System::Graphics::GL::GetGPUProjectionMatrix(camera->projectionMatrix) );




            material->shader->setColor("color", material->color);
            std::cout <<"MVP is" << ( System::Graphics::GL::GetGPUProjectionMatrix(camera->projectionMatrix)* camera->GetworldToCameraMatrix() * transform()->GetLocalToWorldMatrix()).ToString() << std::endl;
            material->shader->setVec3("viewPos", camera->transform()->GetPosition() );
            //point light
            if(System::Light::allLights.size() > 0){
            material->shader->setVec3("lightPos", System::Light::allLights[0]->transform()->GetPosition());
            Vector3 oglLightPos = Vector3(System::Light::allLights[0]->transform()->GetPosition().x, System::Light::allLights[0]->transform()->GetPosition().y,  System::Light::allLights[0]->transform()->GetPosition().z);

            std::cout << "lightPos: " << System::Light::allLights[0]->transform()->GetPosition().ToString() << std::endl;
            material->shader->setVec3("lightColor", System::Vector3(1, 1, 1)); 
            }

        //    System::Light::allLights[0].

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