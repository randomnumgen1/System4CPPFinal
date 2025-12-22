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
    
        FreeCam(){
            camera = nullptr;

        }

        void Update() override {
            if (camera == nullptr) {
                return;
            }
            bool fastMode = System::Input::GetKey(System::KeyCode::LeftShift);
            float movementSpeed = fastMode ? 100.0f : 10.0f;


            
            //rotation


            yaw += speedH * System::Input::GetAxis("MouseX");
            pitch += speedV * System::Input::GetAxis("MouseY");
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
             

            
            camera->transform()->seteulerAngles(System::Vector3(pitch, yaw, 0.0f)); 


            //movement


            System::Vector3 move = System::Vector3::zero;

            // local movement


            // Move the camera forward based on its current facing direction.
            if (System::Input::GetKey(System::KeyCode::W)) {
                move += camera->transform()->forward();
            }
            // Move the camera backward opposite to its current facing direction.
            if (System::Input::GetKey(System::KeyCode::S)) {
                move -= camera->transform()->forward();
            }
            // Move the camera left relative to its current rotation.
            if (System::Input::GetKey(System::KeyCode::A)) {
                move -= camera->transform()->right();
            }
            // Move the camera right relative to its current rotation.
            if (System::Input::GetKey(System::KeyCode::D)) {
                move += camera->transform()->right();
            }


            // world movement

            // Move the camera forward in world space (ignores camera rotation)
            if (System::Input::GetKey(System::KeyCode::I)) {
                move += Vector3::forward;
            }
            // Move the camera backward in world space (ignores camera rotation)
            if (System::Input::GetKey(System::KeyCode::K)) {
                move += Vector3::back;
            }
            // Move the camera left in world space (ignores camera rotation)
            if (System::Input::GetKey(System::KeyCode::J)) {
                move += Vector3::left;
            }
            // Move the camera right in world space (ignores camera rotation)
            if (System::Input::GetKey(System::KeyCode::L)) {
                move += Vector3::right;
            }


            // Move the camera up in world space (ignores camera rotation)
            if (System::Input::GetKey(System::KeyCode::N)) {
                move += Vector3::up;
            }
            // Move the camera down in world space (ignores camera rotation)
            if (System::Input::GetKey(System::KeyCode::M)) {
                move += Vector3::down;
            }

            if (move != Vector3::zero) {
                camera->transform()->SetPosition(camera->transform()->GetPosition() + move.normalized() * movementSpeed * System::Time::deltaTime);
            }




             


          

             



        
        }



        Component* Clone() const override {
            FreeCam* freecam = new FreeCam();
            return freecam;
        }

    };
}
#endif