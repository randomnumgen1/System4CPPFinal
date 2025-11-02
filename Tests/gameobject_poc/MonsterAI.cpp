#include "MonsterAI.hpp"
#include <System/GameObject.hpp>
#include <System/Vector3.hpp>
#include <System/Quaternion.hpp>
#include <iostream>

namespace System {
    void MonsterAI::Awake() {
        std::cout << "MonsterAI Awake!" << std::endl;
    }

    void MonsterAI::Start() {
        std::cout << "MonsterAI Start!" << std::endl;
        GameObject* prefab = GameObject::Find("MinionPrefab");
        if (prefab) {
            GameObject* newMinion = GameObject::Instantiate(prefab, Vector3(5, 5, 5), Quaternion::Euler(0, 90, 0));
            if (newMinion) {
                newMinion->transform->PrintPosition();
            }
        }
    }

    void MonsterAI::Update() {
        std::cout << "MonsterAI Update!" << std::endl;
    }

    Component* MonsterAI::Clone() const {
        return new MonsterAI(*this);
    }
}
