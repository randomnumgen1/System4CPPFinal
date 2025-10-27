/*
SetParent works by modifying the Transform hierarchy, not the GameObject itself. 
Since every GameObject has a Transform component, parenting is handled at the Transform level rather than the GameObject level.

*/
#ifndef _SYSTEM_TRANSFORM_H
#define _SYSTEM_TRANSFORM_H


#include <iostream>
#include <System/Vector3.hpp>
#include <System/Quaternion.hpp>
#include <System/Matrix4x4.hpp>
#include <System/Component.h>
enum Space { Self, World };
namespace System{
class Transform : System::Component {
private:

public:
    /// <summary>
    /// The position of the transform in world space.
    /// </summary>
    System::Vector3 position;
    /// <summary>
    /// The rotation of the transform in world space.
    /// </summary>
    System::Quaternion rotation;
    /// <summary>
    /// The scale of the transform in world space.
    /// </summary>
    System::Vector3 scale;


    System::Vector3 localPosition;
    System::Quaternion localRotation;
    System::Vector3 localScale;



    Transform *parent;
    bool hasChanged;
    Transform(){
        position = System::Vector3(0.0f, 0.0f, 0.0f);
        rotation = System::Quaternion::Euler(0.0f, 0.0f, 0.0f);
        scale = System::Vector3(1.0f, 1.0f, 1.0f);

        localPosition = position;
        localRotation = rotation;
        localScale = scale;
        parent = nullptr;
        hasChanged = false;
    }
    System::Vector3 right() const {
        return rotation * Vector3::right;
    }
    System::Vector3 up() const {
        return rotation * Vector3::up;
    }
    System::Vector3 forward() const {
        return rotation * Vector3::forward;
    }
    void SetParent(Transform* p) {

    }
    void SetParent(Transform* p, bool worldPositionStays) {
    
    }


    void Rotate(System::Vector3 eulers, Space relativeTo) {
    
    }
    void Rotate(float xAngle, float yAngle, float zAngle, Space relativeTo) {
    
    }
    void Rotate(System::Vector3 eulers, Space relativeTo) {
        System::Quaternion rotationDelta = System::Quaternion::Euler(eulers.x, eulers.y, eulers.z);
        if (relativeTo == Self) {
            rotation = rotation * rotationDelta; // Local rotation
        }
        else {
            rotation = rotationDelta * rotation; // World rotation
        }
    }
    System::Matrix4x4 GetLocalToWorldMatrix() const {
        System::Matrix4x4 localMatrix = System::Matrix4x4::Translation(localPosition) * System::Matrix4x4::Rotation(localRotation) * System::Matrix4x4::Scaling(localScale);
        if (parent) {
            return parent->GetLocalToWorldMatrix() * localMatrix;
        }
        return System::Matrix4x4::Translation(position) * System::Matrix4x4::Rotation(rotation) * System::Matrix4x4::Scaling(scale);
    }
    System::Matrix4x4 worldToLocalMatrix() const {
    
    }

    void SetPosition(float nx, float ny, float nz) {
        std::cout << "Setting position to: (" << nx << ", " << ny << ", " << nz << ")" << std::endl;
        position.x = nx;
        position.y = ny;
        position.z = nz;
        if (parent) {
            Matrix4x4 parentWorldToLocal = parent->GetLocalToWorldMatrix().inverse();
            localPosition = parentWorldToLocal.MultiplyPoint3x4(position);
        }
        else {
            localPosition = position;
        }
        hasChanged = true;
    }
    void PrintPosition() const {
        std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    }

};
}
#endif