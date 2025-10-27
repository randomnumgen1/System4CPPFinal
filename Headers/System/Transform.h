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

        // Getters for world-space properties
        System::Vector3 GetPosition() const {
            if (parent) {
                return parent->GetLocalToWorldMatrix().MultiplyPoint3x4(localPosition);
            }
            return localPosition;
        }

        System::Quaternion GetRotation() const {
            if (parent) {
                return parent->GetRotation() * localRotation;
            }
            return localRotation;
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

        void SetPosition(float nx, float ny, float nz) {
            SetPosition(System::Vector3(nx, ny, nz));
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

        void SetParent(Transform* p) {
            // A proper implementation would also remove this from the old parent's children list
            if (parent) {
                auto& siblings = parent->children;
                siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
            }
            parent = p;
            if (p) {
                p->children.push_back(this);
            }
        }
        void SetParent(Transform* p, bool worldPositionStays) {
            if (worldPositionStays) {
                System::Vector3 worldPos = GetPosition();
                System::Quaternion worldRot = GetRotation();
                SetParent(p);
                SetPosition(worldPos);
                localRotation = System::Quaternion::Inverse(GetRotation()) * worldRot;

            }
            else {
                SetParent(p);
            }
        }

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
            std::cout << "Position: (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
        }

    };
}
#endif