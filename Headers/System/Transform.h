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
            // Default to worldPositionStays = true, which is the most common expectation.
            SetParent(p, true);
        }
        void SetParent(Transform* newParent, bool worldPositionStays) {
            // 1. If we are to maintain the world position, store the current world position and rotation.
            System::Vector3 oldWorldPosition;
            System::Quaternion oldWorldRotation;
            if (worldPositionStays) {
                oldWorldPosition = GetPosition();
                oldWorldRotation = GetRotation();
            }

            // 2. Remove this transform from the old parent's children list.
            if (parent) {
                auto& siblings = parent->children;
                siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
            }else{
                auto& siblings = Scene::root->transform->children;
                siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
            }

            // 3. Set the new parent.
            parent = newParent;
            Transform* targetParent = newParent ? newParent : Scene::root->transform;
            targetParent->children.push_back(this);


            // 4. If we are maintaining world position, calculate the new local position and rotation.
            if (worldPositionStays) {
                if (parent) {
                    // The new local position is the old world position transformed into the new parent's local space.
                    localPosition = parent->worldToLocalMatrix().MultiplyPoint3x4(oldWorldPosition);
                    // The new local rotation is derived from the old world rotation and the new parent's world rotation.
                    localRotation = System::Quaternion::Inverse(parent->GetRotation()) * oldWorldRotation;
                }
                else {
                    // If there's no new parent, the new local position/rotation is just the old world position/rotation.
                    localPosition = oldWorldPosition;
                    localRotation = oldWorldRotation;
                }
                hasChanged = true;
            }
            // If worldPositionStays is false, the localPosition and localRotation remain unchanged,
            // so the object's position and rotation will now be relative to the new parent.
        }
        void SetParent(Transform* newParent, bool worldPositionStays) {
           // 1. If we are to maintain the world position, store the current world position and rotation.
            System::Vector3 oldWorldPosition;
            System::Quaternion oldWorldRotation;
            if (worldPositionStays) {
                oldWorldPosition = GetPosition();
                oldWorldRotation = GetRotation();
            }

            // 2. Remove this transform from the old parent's children list.
            if (parent && newParent == nullptr) {
                auto& siblings = parent->children;
                siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
            }

            if (newParent == nullptr) {
                Scene::root->transform->children.push_back(this);
            }else{
                newParent->children.push_back(this);
            }
            parent = newParent;


            // 4. If we are maintaining world position, calculate the new local position and rotation.
            if (worldPositionStays) {
                if (parent) {
                    // The new local position is the old world position transformed into the new parent's local space.
                    localPosition = parent->worldToLocalMatrix().MultiplyPoint3x4(oldWorldPosition);
                    // The new local rotation is derived from the old world rotation and the new parent's world rotation.
                    localRotation = System::Quaternion::Inverse(parent->GetRotation()) * oldWorldRotation;
                }
                else {
                    // If there's no new parent, the new local position/rotation is just the old world position/rotation.
                    localPosition = oldWorldPosition;
                    localRotation = oldWorldRotation;
                }
                hasChanged = true;
            }
            // If worldPositionStays is false, the localPosition and localRotation remain unchanged,
            // so the object's position and rotation will now be relative to the new parent.
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
            std::cout << "Position (World): (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
            std::cout << "Position (local): (" << localPosition.x << ", " << localPosition.y << ", " << localPosition.z << ")" << std::endl;


        }

    };
}
#endif