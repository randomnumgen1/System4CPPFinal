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

#include <vector>
#include <algorithm>

namespace System {
    class GameObject;
    class Scene;
}

enum Space { Self, World };
namespace System {
    class Transform : public System::Component {
    private:
        // Local space properties - these are the source of truth
        System::Vector3 localPosition;
        System::Quaternion localRotation;
        System::Vector3 localScale;
    public:


        // Hierarchy
        Transform* parent;
        std::vector<Transform*> children;
        bool hasChanged;

        Transform() {
            localPosition = System::Vector3(0.0f, 0.0f, 0.0f);
            localRotation = System::Quaternion::Euler(0.0f, 0.0f, 0.0f);
            localScale = System::Vector3(1.0f, 1.0f, 1.0f);
            parent = nullptr;
            hasChanged = false;
        }
        Transform(const Transform& other) {
            localPosition = other.localPosition;
            localRotation = other.localRotation;
            localScale = other.localScale;
            parent = nullptr;
            hasChanged = true;
        }

        Component* Clone() const override {
            return new Transform(*this);
        }
        // Getters for world-space properties
        System::Vector3 GetPosition() const {
            if (parent) {
                return parent->GetLocalToWorldMatrix().MultiplyPoint3x4(localPosition);
            }
            return localPosition;
        }
        System::Vector3 GetLocalPosition() const {
            return localPosition;
        }
        System::Quaternion GetRotation() const {
            if (parent) {
                return parent->GetRotation() * localRotation;
            }
            return localRotation;
        }
        System::Quaternion GetLocalRotation() const {
            return localRotation;
        }
        System::Vector3 GetScale() const {
            if (parent) {
                return parent->GetScale() * localScale;
            }
            return localScale;
        }
        System::Vector3 GetLocalScale() const {
            return localScale;
        }

        // Setters for world-space properties
        void SetPosition(const System::Vector3& newPosition) {
            if (parent) {
                localPosition = parent->worldToLocalMatrix().MultiplyPoint3x4(newPosition);
            }else{
                localPosition = newPosition;
            }
            hasChanged = true;
        }
        void SetLocalPosition(const System::Vector3& newPosition) {
            localPosition = newPosition;
            hasChanged = true;
        }
        void SetPosition(float nx, float ny, float nz) {
            SetPosition(System::Vector3(nx, ny, nz));
        }
        void SetRotation(const System::Quaternion& newRotation) {
            if (parent) {
                localRotation = Quaternion::Inverse(parent->GetRotation()) * newRotation;
            }
            else {
                localRotation = newRotation;
            }
            hasChanged = true;
        }

        void SetScale(const System::Vector3& newScale) {
            if (parent) {
                localScale = newScale / parent->GetScale();
            }
            else {
                localScale = newScale;
            }
            hasChanged = true;
        }
        // Matrix calculations
        System::Matrix4x4 GetLocalToWorldMatrix() const {
            System::Matrix4x4 localMatrix = System::Matrix4x4::Translation(localPosition) * System::Matrix4x4::Rotation(localRotation) * System::Matrix4x4::Scaling(localScale);
            if (parent) {
                return parent->GetLocalToWorldMatrix() * localMatrix;
            }
            return localMatrix;
        }

        System::Matrix4x4 worldToLocalMatrix() const {
            return GetLocalToWorldMatrix().inverse();
        }

        void SetParent(Transform* p);
        void SetParent(Transform* newParent, bool worldPositionStays);
        System::Vector3 right() const {
            return GetRotation() * Vector3::right;
        }
        /// <summary>
        /// Y axis of the transform in world space.
        /// </summary>
        /// <returns></returns>
        System::Vector3 up() const {
            return GetRotation() * Vector3::up;
        }
        System::Vector3 forward() const {
            return GetRotation() * Vector3::forward;
        }

        void Rotate(float xAngle, float yAngle, float zAngle, Space relativeTo) {
            Rotate(System::Vector3(xAngle, yAngle, zAngle), relativeTo);
        }

        void Rotate(System::Vector3 eulers, Space relativeTo) {
            System::Quaternion rotationDelta = System::Quaternion::Euler(eulers.x, eulers.y, eulers.z);
            if (relativeTo == Self) {
                localRotation = localRotation * rotationDelta; // Local rotation
            }
            else {
                // World rotation
                System::Quaternion currentWorldRotation = GetRotation();
                System::Quaternion newWorldRotation = rotationDelta * currentWorldRotation;
                if (parent) {
                    localRotation = System::Quaternion::Inverse(parent->GetRotation()) * newWorldRotation;
                }
                else {
                    localRotation = newWorldRotation;
                }
            }
            hasChanged = true;
        }

        // Debugging
        void PrintPosition() const {
            System::Vector3 worldPos = GetPosition();
            std::cout << "Position (World): (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
            std::cout << "Position (local): (" << localPosition.x << ", " << localPosition.y << ", " << localPosition.z << ")" << std::endl;


        }

    };
}
#endif