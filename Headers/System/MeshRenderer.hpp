#ifndef _SYSTEM_MESHRENDERER_HPP
#define _SYSTEM_MESHRENDERER_HPP

#include <System/Component.hpp>
#include <System/Material.hpp>
#include <System/MeshFilter.hpp>
#include <System/GameObject.hpp>
#include <System/Camera.hpp>
#include <System/Graphics/GraphicsHelpers.h>
#include <System/Light.hpp>
#include <System/Mesh.hpp>
#include <vector>
namespace System {
    class MeshRenderer : public Component {
    public:
       
        std::vector<Material*> m_Materials;






        MeshRenderer() {}
        // Sets the primary material at index 0
        void SetMaterial(Material* newmat) {
            SetMaterial(newmat, 0);
        }

        // Sets a material at a specific index safely
        void SetMaterial(Material* newmat, int idx) {
            // Prevent crashes from negative indices
            if (idx < 0) return;

            // Safely resize vector if the index is out of bounds
            if (static_cast<size_t>(idx) >= m_Materials.size()) {
                m_Materials.resize(idx + 1, nullptr);
            }

            m_Materials[idx] = newmat;
        }

        //this is for meshes with multiple submeshes, we will implement this later.
        /*
        void RenderWIP(Camera* camera) {
            MeshFilter* meshFilter = gameObject()->GetComponent<MeshFilter>();
            if (!meshFilter || !meshFilter->mesh) return;

            const std::vector<System::Mesh::SubMeshDescriptor>& submeshes = meshFilter->mesh->submeshes;

          
            System::Graphics::GL::gl_glBindVertexArray(meshFilter->mesh->GetVAO());

            for (size_t i = 0; i < submeshes.size(); ++i) {
                const System::Mesh::SubMeshDescriptor& submesh = submeshes[i];

                
                Material* material = nullptr;
                if (i < m_Materials.size()) {
                    material = m_Materials[i];
                }

                
                if (!material) {
                    //we will implement a default material later, for now just error out.
                    //material = Material::GetDefaultMaterial();
                }

                

                material->shader->use();
                material->shader->setMat4("model", transform()->GetLocalToWorldMatrix());
                material->shader->setMat4("view", camera->GetworldToCameraMatrix());
                material->shader->setMat4("projection", System::Graphics::GL::GetGPUProjectionMatrix(camera->projectionMatrix));




                material->shader->setColor("color", material->color);
                material->shader->setVec3("viewPos", camera->transform()->GetPosition());







                material->BindProperties();

               
                void* indexOffsetBytes = reinterpret_cast<void*>(submesh.indexStart * sizeof(uint32_t));

               
                System::Graphics::GL::gl_glDrawElementsBaseVertex(
                    GL_TRIANGLES,
                    submesh.indexCount,
                    GL_UNSIGNED_INT,
                    indexOffsetBytes,
                    submesh.baseVertex
                );

                
            }

            // 10. Clean up global vertex bindings
            System::Graphics::GL::gl_glBindVertexArray(0);
        } 
        */
        void Render(Camera* camera) {
            if (!m_Materials[0] || !m_Materials[0]->shader) {
                return;
            }

            MeshFilter* meshFilter = gameObject()->GetComponent<MeshFilter>();
            if (!meshFilter || !meshFilter->mesh) {
                return;
            }

            m_Materials[0]->shader->use();
            m_Materials[0]->shader->setMat4("model", transform()->GetLocalToWorldMatrix()  );
            m_Materials[0]->shader->setMat4("view", camera->GetworldToCameraMatrix());
            m_Materials[0]->shader->setMat4("projection", System::Graphics::GL::GetGPUProjectionMatrix(camera->projectionMatrix) );




            m_Materials[0]->shader->setColor("color", m_Materials[0]->color);
            m_Materials[0]->shader->setVec3("viewPos", camera->transform()->GetPosition() );




            m_Materials[0]->BindProperties();
           










            //point light
            if(System::Light::allLights.size() > 0){
                if (System::Light::allLights[0]->type == System::LightType::Point) {
                    m_Materials[0]->shader->setVec3("lightPos", System::Light::allLights[0]->transform()->GetPosition());
                    m_Materials[0]->shader->setVec3("lightColor", System::Vector3(1.0f, 1.0f, 1.0f));
                    m_Materials[0]->shader->setFloat("LightIntensity", System::Light::allLights[0]->intensity);
                    m_Materials[0]->shader->setFloat("LightRange", System::Light::allLights[0]->range);
                }else if (System::Light::allLights[0]->type == System::LightType::Directional) {

                }
                else if (System::Light::allLights[0]->type == System::LightType::Spot) {

                }


            }





            meshFilter->mesh->UploadMeshData(false);
            assert(meshFilter->mesh->GetVAO() != 0 && "Attempting to draw with uninitialized VAO");
            assert(!meshFilter->mesh->indices.empty() && "No indices to draw");
            System::Graphics::GL::gl_glBindVertexArray(meshFilter->mesh->GetVAO());
            System::Graphics::GL::gl_glDrawElements(System::Graphics::DrawMode::TRIANGLES, meshFilter->mesh->indices.size(), System::Graphics::IndexType::UNSIGNED_INT, 0);//fix
            System::Graphics::GL::gl_glBindVertexArray(0); 
        }
        Component* Clone() const override {
            MeshRenderer* new_renderer = new MeshRenderer();
            new_renderer->m_Materials  = m_Materials;
            return new_renderer;
        }
        /*
         void SetMaterial(size_t submeshIndex, Material* material) {
        if (submeshIndex >= m_Materials.size()) {
            m_Materials.resize(submeshIndex + 1);
        }
        m_Materials[submeshIndex] = material;
    }

    Material* GetMaterial(size_t submeshIndex) const {
        if (submeshIndex < m_Materials.size()) {
            return m_Materials[submeshIndex];
        }
        return nullptr;
    }
        
        
        
        
        
        
        */
 
    };
}

#endif