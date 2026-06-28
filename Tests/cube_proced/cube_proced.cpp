#include <iostream>
#include <vector>
#include <System/Windows/Application.h>
#include <System/GameObject.hpp>
#include <System/Scene.hpp>
#include <System/Camera.hpp>
#include <System/MeshRenderer.hpp>
#include <System/Transform.hpp>
#include <System/Light.hpp>
#include <System/Time.hpp>
#include <System/Input.hpp>
#include <System/Shader.hpp>
#include <System/Material.hpp>
#include <System/MeshFilter.hpp>
#include <System/Devices/Gamepad.hpp>
#include <System/Mesh.hpp>

float horizontalSpeed = 2.0f;
float yaw = 0.0f;
float pitch = 0.0f;
float speedH = 2.0f;
float speedV = 2.0f;

const char* vertexShaderSource = R"(#version 310 es
precision mediump float;
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(#version 310 es
precision mediump float;
out vec4 FragColor;
uniform vec4 color;
void main()
{
    FragColor = color;
}
)";

int main() {
    System::Windows::Application::initWindow();

    System::Scene::Initialize();
    
    System::GameObject* cameraGO = new System::GameObject("Main Camera");
    System::Camera* camera = cameraGO->AddComponent<System::Camera>();
    camera->isMain = true;
    camera->transform()->SetPosition(System::Vector3(0.0f, 0.0f, -5.0f));
    camera->transform()->SetRotation(System::Quaternion::Euler(0, 0, 0));

    System::GameObject* lightGO = new System::GameObject("Light");
    System::Light* light = lightGO->AddComponent<System::Light>();
    light->type = System::LightType::Point;
    light->color = System::Color32(255, 255, 255, 255);
    light->intensity = 6.0f;
    light->range = 20.0f;
    light->transform()->SetPosition(System::Vector3(0.0f, 2.0f, -2.0f));

    System::Mesh* m_mesh = new System::Mesh();
    System::GameObject* proc_cube = new System::GameObject("Proc Cube");
    System::MeshFilter* mf = proc_cube->AddComponent<System::MeshFilter>();
    mf->mesh = m_mesh;

    System::MeshRenderer* mr = proc_cube->AddComponent<System::MeshRenderer>();
	 
    std::vector<System::Vector3> vertices = {
        System::Vector3(0.0f, 0.0f, 0.0f),
        System::Vector3(0.5f, 0.0f, 0.0f),
        System::Vector3(0.5f, 0.5f, 0.0f),
        System::Vector3(0.0f, 0.5f, 0.0f),
        System::Vector3(0.0f, 0.5f, 0.5f),
        System::Vector3(0.5f, 0.5f, 0.5f),
        System::Vector3(0.5f, 0.0f, 0.5f),
        System::Vector3(0.0f, 0.0f, 0.5f),
    };

    std::vector<int> triangles = {
        0, 2, 1, //face front
        0, 3, 2,
        2, 3, 4, //face top
        2, 4, 5,
        1, 2, 5, //face right
        1, 5, 6,
        0, 7, 4, //face left
        0, 4, 3,
        5, 4, 7, //face back
        5, 7, 6,
        0, 6, 7, //face bottom
        0, 1, 6
    };

    m_mesh->SetVertices(vertices);
    m_mesh->SetTriangles(triangles, 0);
    m_mesh->RecalculateNormals();
    m_mesh->UploadMeshData(false);
		
    System::Shader* shader = System::Shader::CreateFromSource(vertexShaderSource, fragmentShaderSource);
	 
    System::Material* materialGreen = new System::Material(shader);
    materialGreen->color = System::Color(0.0f, 1.0f, 0.0f, 1.0f);
    mr->material = materialGreen;

    System::Scene* scene = new System::Scene();
    
    int frameCount = 0;
	
    System::Devices::GamePad pad;
	
    while (frameCount < 1000) {  
        System::Time::startframe();
        pad.update();
		
        // Rotate proc_cube removed
        // proc_cube->transform->Rotate(System::Vector3(45.0f * System::Time::deltaTime, 45.0f * System::Time::deltaTime, 0.0f), Space::Self);
        
        scene->Run(800, 600);
		
        // looking around [start]
        float mouseX = pad.getAxis(0, System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_RIGHT_X);
        float mouseY = pad.getAxis(0, System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_RIGHT_Y) * -1.0f;

        yaw += 100.0f * mouseX * System::Time::deltaTime;
        pitch += 100.0f * mouseY * System::Time::deltaTime;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
		
        camera->transform()->seteulerAngles(System::Vector3(pitch, yaw, 0.0f));
        // looking around [end]
		
        // moving around [start]
        float horizontal = pad.getAxis(0, System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_LEFT_X);
        float vertical = pad.getAxis(0, System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_LEFT_Y);
		
        System::Vector3 forwardDirection = camera->transform()->forward() * vertical;
        System::Vector3 rightDirection = camera->transform()->right() * horizontal;

        System::Vector3 moveDirection = forwardDirection + rightDirection;
        moveDirection.y = 0.0f;
        
        if (moveDirection != System::Vector3::zero) {
            moveDirection.Normalize();
            camera->transform()->SetPosition(camera->transform()->GetPosition() + moveDirection * 5.0f * System::Time::deltaTime);
        }
        // moving around [end]
		
        System::Windows::Application::SwapBuffers();
        if (frameCount % 100 == 0) {
            std::cout << "Frame " << frameCount << " completed. deltaTime: " << System::Time::deltaTime << std::endl;
		    System::Vector3 camPos = camera->transform()->GetPosition();
            System::Vector3 camRot = camera->transform()->GetRotation().eulerAngles();
            System::Vector3 cubePos = proc_cube->transform->GetPosition();
            System::Vector3 cubeRot = proc_cube->transform->GetRotation().eulerAngles();
            std::cout << "Camera Pos: (" << camPos.x << ", " << camPos.y << ", " << camPos.z << ") Rot: (" << camRot.x << ", " << camRot.y << ", " << camRot.z << ")" << std::endl;
            std::cout << "Cube Pos: (" << cubePos.x << ", " << cubePos.y << ", " << cubePos.z << ") Rot: (" << cubeRot.x << ", " << cubeRot.y << ", " << cubeRot.z << ")" << std::endl;
        }
			 
        frameCount++;

        if (frameCount == 500) {
            camera->TakeScreenshot("cube_proced_screenshot.bmp");
        }
    }

    delete scene;
    std::cout << "Test completed successfully." << std::endl;
    return 0;
}
