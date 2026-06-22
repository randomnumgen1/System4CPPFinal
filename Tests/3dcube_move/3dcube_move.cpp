#include <iostream>
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



float horizontalSpeed = 2.0f;

float yaw = 0.0f;
float pitch = 0.0f;
float speedH = 2.0f;
float speedV = 2.0f;


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

    System::GameObject* cube = System::GameObject::CreatePrimitive(System::PrimitiveType::Cube);
    System::MeshRenderer* renderer = cube->GetComponent<System::MeshRenderer>();
    
	
	
	#if defined(__arm__) || defined(__aarch64__)
    System::Shader* shader = new System::Shader("shaderbasicflat_es.vert", "shaderbasicflat_es.frag");
#else	
    System::Shader* shader = new System::Shader("shaderadvanced.vert", "shaderadvanced.frag");
#endif	
	
	
	
	 
    System::Material* material = new System::Material(shader);
    material->color = System::Color(1.0f, 0.0f, 0.0f, 1.0f);
    renderer->material = material;

    System::Scene* scene = new System::Scene();
    
    int frameCount = 0;
	
	System::Devices::GamePad pad;
	
	
    while (true) {  
        System::Time::startframe();
		pad.update();
		
		
        //System::Input::UpdateInputState();
        
        // Rotate cube
        cube->transform->Rotate(System::Vector3(0.0f, 45.0f * System::Time::deltaTime, 0.0f), Space::Self);
        
        scene->Run(800, 600);
		
		
		 
		
		// looking around [start]
		
		float mouseX = pad.getAxis(0, System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_RIGHT_X);
		float mouseY = pad.getAxis(0, System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_RIGHT_Y);

		
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
        
		
		
		//transform.position += moveDirection * 1.0f * Time.deltaTime;
		
		if (moveDirection != System::Vector3::zero) {
			moveDirection.Normalize();
			camera->transform()->SetPosition(camera->transform()->GetPosition() + moveDirection * 5.0f * System::Time::deltaTime);
		}
		
		// moving around [end]
		
        System::Windows::Application::SwapBuffers();
        if (frameCount % 100 == 0) {
		std::cout << "Frame " << frameCount << " completed. deltaTime: " << System::Time::deltaTime << std::endl;
		}
        frameCount++;
    }

    delete scene;
    std::cout << "Test completed successfully." << std::endl;
    return 0;
}
