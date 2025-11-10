#include <System/Scene.hpp>
#include <System/GameObject.hpp>
#if defined(SYSTEM_AUDIO_OPENAL)
#include <System/Internal/Audio/OpenALEngine.hpp>
#elseif defined(SYSTEM_AUDIO_FMOD)
#include <System/Internal/Audio/FMODEngine.hpp>
#else
#include <System/Internal/Audio/DummyAudioEngine.hpp>
#endif
namespace System {
    GameObject* Scene::root = nullptr;
    AudioEngine* Scene::audioEngine = nullptr;

    void Scene::Initialize() {
        if (!root) {
            //dummy gameobject acts as a "meta-parent" or a "scene organizer.".

            root = new GameObject("__SceneRoot__");
        }
        if (!audioEngine) {
#if defined(SYSTEM_AUDIO_OPENAL)
            audioEngine = new OpenALEngine();
#elseif defined(SYSTEM_AUDIO_FMOD)
            audioEngine = new FMODEngine();
#else
            audioEngine = new DummyAudioEngine();
#endif


            audioEngine->Initialize();
        }
    }

    void Scene::Shutdown() {
        delete root;
        root = nullptr;
    }
}
