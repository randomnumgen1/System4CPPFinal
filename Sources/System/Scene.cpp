#include <System/Scene.hpp>
#include <System/GameObject.hpp>
#include <System/Internal/OpenALEngine.hpp>
namespace System {
    GameObject* Scene::root = nullptr;

    void Scene::Initialize() {
        if (!root) {
            //dummy gameobject acts as a "meta-parent" or a "scene organizer.".

            root = new GameObject("__SceneRoot__");
        }
        if (!audioEngine) {
            audioEngine = new OpenALEngine();
            audioEngine->Initialize();
        }
    }

    void Scene::Shutdown() {
        delete root;
        root = nullptr;
    }
}
