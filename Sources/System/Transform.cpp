#include "System/Transform.hpp"
#include <System/GameObject.h>
#include <System/Scene.hpp>
#include <algorithm>

void System::Transform::SetParent(Transform* p){
    // Default to worldPositionStays = true, which is the most common expectation.
    SetParent(p, true);
}

void System::Transform::SetParent(Transform* newParent, bool worldPositionStays) {
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
    }
    else {
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
