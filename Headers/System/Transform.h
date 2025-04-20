#pragma once
#include <iostream>
#include <System/Vector3.h>
#include <System/Quaternion.h>
#include "System/Matrix4x4.h"
enum Space { Self, World };
class Transform {
public:
    System::Vector3 position;
    System::Quaternion rotation;
    System::Vector3 scale;

    Transform(){
        position = System::Vector3(0.0f, 0.0f, 0.0f);
        rotation = System::Quaternion::Euler(0.0f, 0.0f, 0.0f);
        scale = System::Vector3(1.0f, 1.0f, 1.0f);
    }
    void Rotate(System::Vector3 eulers, Space relativeTo) {
    
    }
    void Rotate(float xAngle, float yAngle, float zAngle, Space relativeTo) {
    
    }
    System::Matrix4x4  GetLocalToWorldMatrix() const {
        return System::Matrix4x4::Translation(position) *  System::Matrix4x4::Rotation(rotation) *  System::Matrix4x4::Scaling(scale);


        System::Matrix4x4 localMatrix = System::Matrix4x4::Translation(position) * System::Matrix4x4::Rotation(rotation) * System::Matrix4x4::Scaling(scale);
      //  if (parent) {
       //     return parent->GetLocalToWorldMatrix() * localMatrix;
      //  }
        return localMatrix;
    }
    void SetPosition(float nx, float ny, float nz) {
        std::cout << "Setting position to: (" << nx << ", " << ny << ", " << nz << ")" << std::endl;
        position.x = nx;
        position.y = ny;
        position.z = nz;
    }
    void PrintPosition() const {
        std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    }
};
