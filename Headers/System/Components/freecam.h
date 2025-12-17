#ifndef _SYSTEM_FREECAM_H
#define _SYSTEM_FREECAM_H



#include <iostream>
#include <System/Vector3.hpp>
#include <System/Quaternion.hpp>
#include <System/Matrix4x4.hpp>
#include <System/Component.hpp>
#include <System/Color32.hpp>
#include <vector>
#include <System/Input.hpp>
#include <System/Camera.hpp>
#include <System/Time.hpp>
#include <System/GameObject.hpp>
#include <System/Transform.hpp>

namespace System {
    class FreeCam : public System::Component {
    private:

    public:
        System::Camera* camera = NULL;
        float horizontalSpeed = 2.0f;

        float yaw = 0.0f;
        float pitch = 0.0f;
        float speedH = 2.0f;
        float speedV = 2.0f;
    
        FreeCam()
        {
           

        }
        void Update() override {
            bool fastMode = System::Input::GetKey(System::KeyCode::LeftShift);
            float movementSpeed = fastMode ? 100.0f : 10.0f;
        
            if (System::Input::GetKey(System::KeyCode::W) || Input::GetKey(System::KeyCode::UpArrow)) {
                System::Vector3 forward = camera->transform()->forward();
                camera->transform()->SetPosition(camera->transform()->GetPosition() + (camera->transform()->forward() * movementSpeed * System::Time::deltaTime));
            }
            if (System::Input::GetKey(System::KeyCode::S) || Input::GetKey(System::KeyCode::DownArrow))  {
                System::Vector3 forward = camera->transform()->forward();
                camera->transform()->SetPosition(camera->transform()->GetPosition() + (-camera->transform()->forward() * movementSpeed * System::Time::deltaTime));
            }
            if (System::Input::GetKey(System::KeyCode::A) || Input::GetKey(System::KeyCode::LeftArrow)) {
                System::Vector3 right = camera->transform()->right();
                camera->transform()->SetPosition(camera->transform()->GetPosition() + (-camera->transform()->right() * movementSpeed * System::Time::deltaTime));
            }
            if (System::Input::GetKey(System::KeyCode::D) || Input::GetKey(System::KeyCode::RightArrow)) {
                System::Vector3 right = camera->transform()->right();
                camera->transform()->SetPosition(camera->transform()->GetPosition() + (camera->transform()->right() * movementSpeed * System::Time::deltaTime));
            }

          
            yaw += speedH * System::Input::GetAxis("MouseX");
            pitch += speedV * System::Input::GetAxis("MouseY");
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
             

            
            camera->transform()->seteulerAngles(System::Vector3(pitch, yaw, 0.0f)); 
             



        
        }



        Component* Clone() const override {
            FreeCam* freecam = new FreeCam();
            return freecam;
        }

    };
}
#endif