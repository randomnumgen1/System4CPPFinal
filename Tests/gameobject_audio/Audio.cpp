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
    listenerGo->AddComponent<System::AudioListener>();

    std::cout << "Creating audio source..." << std::endl;
    auto* audioGo = new System::GameObject("AudioSource");
    auto* audioSource = audioGo->AddComponent<System::AudioSource>();

    std::cout << "Loading audio clip..." << std::endl;
    System::AudioClip audioClip;
    // NOTE: This file does not exist. The test will fail to load it,
    // but the program should not crash.
    if (audioClip.LoadMonoWaveFile("test.wav")) {
        std::cout << "Audio clip loaded successfully." << std::endl;
        audioSource->SetClip(&audioClip);
        audioSource->SetLoop(true);
        audioSource->Play();

        std::cout << "Playing for 5 seconds..." << std::endl;
        for (int i = 0; i < 5; ++i) {
            System::Scene::audioEngine->Update();
            sleep(1);
        }

        audioSource->Stop();
        std::cout << "Playback stopped." << std::endl;
    } else {
        std::cout << "Failed to load audio clip. This is expected as the file doesn't exist." << std::endl;
    }

    System::Scene::Shutdown();
    std::cout << "Scene shut down." << std::endl;
    return 0;
}
