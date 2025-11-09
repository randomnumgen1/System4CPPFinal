#include <iostream>
#include <System/GameObject.hpp>
#include <System/Scene.hpp>
#include <System/AudioSource.hpp>
#include <System/AudioListener.hpp>
#include <System/AudioClip.hpp>
#include <unistd.h> // For sleep()

int main() {
    System::Scene::Initialize();

    std::cout << "Creating listener..." << std::endl;
    auto* listenerGo = new System::GameObject("Listener");
    auto* listener = listenerGo->AddComponent<System::AudioListener>();

    std::cout << "Creating audio source..." << std::endl;
    auto* audioGo = new System::GameObject("AudioSource");
    auto* audioSource = audioGo->AddComponent<System::AudioSource>();

    std::cout << "Audio components created successfully." << std::endl;

    // We can't play audio without a file, but we can update the engine
    // to make sure nothing crashes.
    std::cout << "Updating audio engine for 2 seconds..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        listener->Update();
        audioSource->Update();
        System::Scene::audioEngine->Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    System::Scene::Shutdown();
    std::cout << "Scene shut down." << std::endl;

    return 0;
}
