#include <System/Scene.hpp>
#include <System/GameObject.hpp>
#include <System/Camera.hpp>
#include <System/MeshRenderer.hpp>
#include <System/MeshFilter.hpp>
#include <System/Material.hpp>
#include <System/Shader.hpp>
#include <System/Texture2d.hpp>
#include <System/Graphics/GraphicsHelpers.h>

using namespace System::Graphics;

int main() {
    System::Scene::Initialize();
  OpenGLVersion version;
    OpenGLInit(&version);
    System::GameObject* cameraGO = new System::GameObject("Main Camera");
    System::Camera* camera = cameraGO->AddComponent<System::Camera>();
    camera->isMain = true;

    System::GameObject* cube = System::GameObject::CreatePrimitive(System::PrimitiveType::Cube);
    System::MeshRenderer* renderer = cube->GetComponent<System::MeshRenderer>();

    System::Shader* shader = new System::Shader("Tests/gameobject_poc/simple.vert", "Tests/gameobject_poc/simple.frag");
    System::Material* material = new System::Material(shader);
    material->color = System::Color(1, 0, 0, 1);

    renderer->material = material;

    System::Scene scene;
    scene.Run();



    delete material;
    delete shader;
    delete cube;
    delete cameraGO;

    return 0;
}