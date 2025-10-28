#include <System/Scene.hpp>
#include <System/GameObject.h>

namespace System {
    GameObject* Scene::root = nullptr;

    void Scene::Initialize() {
        if (!root) {
            //dummy gameobject acts as a "meta-parent" or a "scene organizer.".

            root = new GameObject("__SceneRoot__");
        }
    }

    void Scene::Shutdown() {
        delete root;
        root = nullptr;
    }
}
