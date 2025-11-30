#include <System/Scene.hpp>
#include <System/GameObject.hpp>
#include <System/Camera.hpp>
#include <System/MeshRenderer.hpp>
#include <System/MeshFilter.hpp>
#include <System/Material.hpp>
#include <System/Shader.hpp>
#include <System/Texture2d.hpp>
#include <System/Graphics/GraphicsHelpers.h>
#include <GLFW/glfw3.h>

using namespace System::Graphics;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    System::Scene::Initialize();
	OpenGLVersion version;
    OpenGLInit(&version);
    System::GameObject* cameraGO = new System::GameObject("Main Camera");
    System::Camera* camera = cameraGO->AddComponent<System::Camera>();
    camera->isMain = true;

    System::GameObject* cube = System::GameObject::CreatePrimitive(System::PrimitiveType::Cube);
    System::MeshRenderer* renderer = cube->GetComponent<System::MeshRenderer>();

    System::Shader* shader = new System::Shader("Tests/scene_render/simple.vert", "Tests/scene_render/simple.frag");
    System::Material* material = new System::Material(shader);
    material->color = System::Color(1, 0, 0, 1);

    renderer->material = material;

    System::Scene scene;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    scene.Run(width, height);

    delete material;
    delete shader;
    delete cube;
    delete cameraGO;

    glfwTerminate();
    return 0;
}