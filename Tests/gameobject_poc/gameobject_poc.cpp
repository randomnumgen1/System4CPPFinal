#include <iostream>
#include <System/GameObject.hpp>
#include <System/Transform.hpp>
#include <System/Vector3.hpp>
#include <System/Scene.hpp>
#include "MonsterAI.hpp"

void PrintTransformDetails(System::GameObject* go) {
    if (!go) {
        std::cout << "GameObject is null." << std::endl;
        return;
    }
    std::cout << "--- " << go->name << " ---" << std::endl;
    if(go->transform->parent) {
        std::cout << "Parent: " << go->transform->parent->gameObject()->name << std::endl;
    } else {
        std::cout << "Parent: nullptr" << std::endl;
    }
    go->transform->PrintPosition();
}

void PrintSceneHierarchy(System::Transform* startNode, int indent = 0) {
    if (!startNode) return;
    for (int i = 0; i < indent; ++i) std::cout << "  ";
    std::cout << "- " << startNode->gameObject()->name << std::endl;
    for (System::Transform* child : startNode->children) {
        PrintSceneHierarchy(child, indent + 1);
    }
}

void PrintAllGameObjects() {
    std::cout << "--- All GameObjects in Scene ---" << std::endl;
    for (System::GameObject* go : System::GameObject::allGameObjects) {
        std::cout << "- " << go->name;
        if (go->transform->parent) {
            std::cout << " (Parent: " << go->transform->parent->gameObject()->name << ")";
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;
}

int main() {
    System::Scene::Initialize();

    std::cout << "--- Test: Custom Hierarchy Model ---" << std::endl;
    System::GameObject* minionPrefab = new System::GameObject("MinionPrefab");
    minionPrefab->AddComponent<System::MonsterAI>();
    System::GameObject* minionChild = new System::GameObject("MinionChild");
    minionChild->transform->SetParent(minionPrefab->transform);
    minionPrefab->transform->SetParent(nullptr); // Unparent from scene root

    System::GameObject* monster = new System::GameObject("Monster");
    monster->AddComponent<System::MonsterAI>();
    monster->AddComponent<System::MonsterAI>();

    // Simulate Start and Update calls
    for (auto& ai : monster->GetComponents<System::MonsterAI>()) {
        ai->Start();
    }
    for (auto& ai : monster->GetComponents<System::MonsterAI>()) {
        ai->Update();
    }
    monster->transform->SetPosition(10, 0, 0);
    System::GameObject* arm = new System::GameObject("Arm");
    arm->transform->SetParent(monster->transform, false);
    arm->transform->SetPosition(5, 0, 0);
    System::GameObject* hand = new System::GameObject("Hand");
    hand->transform->SetParent(arm->transform, false);
    hand->transform->SetPosition(2, 0, 0);

    std::cout << "Initial Scene Hierarchy:" << std::endl;
    PrintSceneHierarchy(System::Scene::root->transform);
    
    std::cout << "\n--- Unparenting 'Hand' ---" << std::endl;
    hand->transform->SetParent(nullptr, true); // Unparent, keeping world position

    std::cout << "\nScene Hierarchy After Unparenting:" << std::endl;
    PrintSceneHierarchy(System::Scene::root->transform);

    std::cout << "\n--- Reparenting 'Hand' to 'Arm' ---" << std::endl;
    hand->transform->SetParent(arm->transform, true); // Reparent, keeping world position

    std::cout << "\nFinal Scene Hierarchy:" << std::endl;
    PrintSceneHierarchy(System::Scene::root->transform);
    std::cout << "\n'Hand' Details After Reparenting:" << std::endl;
    PrintTransformDetails(hand);

    PrintAllGameObjects();

    System::Scene::Shutdown();

    return 0;
}
