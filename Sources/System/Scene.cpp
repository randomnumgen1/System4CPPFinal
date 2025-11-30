#include <System/Scene.hpp>
#include <System/GameObject.hpp>
#include <System/Camera.hpp>
#include <System/MeshRenderer.hpp>

//Audio
#if defined(SYSTEM_AUDIO_OPENAL)
#include <System/Internal/Audio/OpenALEngine.hpp>
#elif defined(SYSTEM_AUDIO_FMOD)
#include <System/Internal/Audio/FMODEngine.hpp>
#else
#include <System/Internal/Audio/DummyAudioEngine.hpp>
#endif
//Physics
#if defined(SYSTEM_PHYSICS_BULLET)
#include <System/Internal/Physics/BulletPhysicsEngine.hpp>
#elif defined(SYSTEM_PHYSICS_PHYSX)
#include <System/Internal/Physics/PhysxPhysicsEngine.hpp>
#elif defined(SYSTEM_PHYSICS_JOLT)
#include <System/Internal/Physics/JoltPhysicsEngine.hpp>
#else
#include <System/Internal/Physics/DummyPhysicsEngine.hpp>
#endif

namespace System {
    GameObject* Scene::root = nullptr;
    AudioEngineInterface* Scene::audioEngine = nullptr;
    PhysicsEngineInterface* Scene::physicsEngine = nullptr;

    void Scene::Initialize() {
        if (!root) {
            //dummy gameobject acts as a "meta-parent" or a "scene organizer.".

            root = new GameObject("__SceneRoot__");
        }
        if (!audioEngine) {
#if defined(SYSTEM_AUDIO_OPENAL)
            audioEngine = new OpenALEngine();
#elif defined(SYSTEM_AUDIO_FMOD)
            audioEngine = new FMODEngine();
#else
            audioEngine = new DummyAudioEngine();
#endif
            audioEngine->Initialize();
        }
        if (!physicsEngine) {
#if defined(SYSTEM_PHYSICS_BULLET)
            physicsEngine = new BulletPhysicsEngine();
#elif defined(SYSTEM_PHYSICS_PHYSX)
    physicsEngine = new PhysxPhysicsEngine();
#elif defined(SYSTEM_PHYSICS_JOLT)
physicsEngine = new JoltPhysicsEngine();
#else
            physicsEngine = new DummyPhysicsEngine();
#endif
            physicsEngine->Initialize();
        }


    }

    
    void Scene::Run() {
        // Update All GameObjects

        for (GameObject* go : GameObject::allGameObjects) {
            for (auto const& [type, components] : go->components) {
                for (void* comp : components) {
                    static_cast<Component*>(comp)->Update();
                }
            }
        }

        // Render Scene
        System::Camera* cam = Camera::Getmain();
        if (cam) {
            cam->RenderStart();
        }
        for (int i = 0; i < GameObject::allGameObjects.size(); i++) {
            GameObject* go = GameObject::allGameObjects[i];
            System::MeshRenderer* renderer = go->GetComponent<System::MeshRenderer>();
            if (renderer) {
                renderer->Render(cam);
            }






        }

    }
    void Scene::Shutdown() {
        delete root;
        root = nullptr;

        if (audioEngine) {
            audioEngine->Shutdown();
            delete audioEngine;
            audioEngine = nullptr;
        }

        if (physicsEngine) {
            physicsEngine->Shutdown();
            delete physicsEngine;
            physicsEngine = nullptr;
        }
    }
}
