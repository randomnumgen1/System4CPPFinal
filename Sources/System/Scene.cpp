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
#include <System/Time.hpp>
#include <thread>

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

    //this is a individual scene run, called every frame from the main loop.
    void Scene::Run(int windowWidth, int windowHeight) {
        static bool firstrun = 0;
        if (!firstrun) {
            // --- Update phase ---
            std::cout << "Scene: calling start on all gameobjects" << std::endl;
            for (GameObject* go : GameObject::allGameObjects) {
                std::cout << go->name << std::endl;
                for (auto const& [type, components] : go->components) {
                    for (void* comp : components) { 
                        static_cast<Component*>(comp)->Start();
                    }
                }
            }
            firstrun = true;
        }



            //System::Time::startframe();

            // --- Update phase ---
        std::cout << "Scene: calling update on all gameobjects" << std::endl;
            for (GameObject* go : GameObject::allGameObjects) {
                for (auto const& [type, components] : go->components) {
                    for (void* comp : components) {
                        static_cast<Component*>(comp)->Update();
                    }
                }
            }

            // --- Render phase ---
            System::Camera* cam = Camera::Getmain();
            cam->RenderStart(windowWidth, windowHeight);

            for (GameObject* go : GameObject::allGameObjects) {
                System::MeshRenderer* renderer = go->GetComponent<System::MeshRenderer>();
                if (renderer) {
                    renderer->Render(cam);
                }
            }
          // Sleep(1);
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
            // --- End frame ---
         //  System::Time::endframe();


        
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
