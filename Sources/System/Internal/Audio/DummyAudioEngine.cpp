#include <System/Internal/Audio/DummyAudioEngine.hpp>
#include <System/AudioSource.hpp>
#include <iostream>

 

namespace System {









    DummyAudioEngine::DummyAudioEngine()  {}

    DummyAudioEngine::~DummyAudioEngine() {}

    void DummyAudioEngine::Initialize() {
 
        std::cerr << "init DummyAudioEngine device" << std::endl;
    }

    void DummyAudioEngine::Shutdown() {
        std::cerr << "shutdown DummyAudioEngine device" << std::endl;
    }

    void DummyAudioEngine::GenerateSource(AudioSource* source) {
        std::cout << "DummyAudioEngine::GenerateSource called for source ID: " << source->sourceId << std::endl;
    }

    void DummyAudioEngine::DeleteSource(AudioSource* source) {
        std::cout << "DummyAudioEngine::DeleteSource called for source ID: " << source->sourceId << std::endl;
    }

    void DummyAudioEngine::SetSourceClip(AudioSource* source, unsigned int bufferId) {
        std::cout << "DummyAudioEngine::SetSourceClip called for source ID: " << source->sourceId << std::endl;
    }

    void DummyAudioEngine::SetSourceVolume(AudioSource* source, float volume) {
        std::cout << "DummyAudioEngine::SetSourceVolume called for source ID: " << source->sourceId << std::endl;
    }

    void DummyAudioEngine::SetSourceLoop(AudioSource* source, bool loop) {
        std::cout << "DummyAudioEngine::SetSourceLoop called for source ID: " << source->sourceId << std::endl;
    }

    void DummyAudioEngine::PlaySourceOneShot(AudioSource* source, unsigned int bufferId, float volumeScale) {
        std::cout << "DummyAudioEngine::PlaySourceOneShot called for source ID: " << source->sourceId << std::endl;
    }
    void DummyAudioEngine::PlaySource(AudioSource* source) {
        std::cout << "DummyAudioEngine::PlaySource called for source ID: " << source->sourceId << std::endl; 
    }
    void DummyAudioEngine::SetSourcePosition(AudioSource* source, const Vector3& position) {
        std::cout << "DummyAudioEngine::SetSourcePosition called for source ID: " << source->sourceId << std::endl;
    }

    void DummyAudioEngine::PauseSource(AudioSource* source) {
        std::cout << "DummyAudioEngine::PauseSource called for source ID: " << source->sourceId << std::endl;
    }

    void DummyAudioEngine::StopSource(AudioSource* source) {
        std::cout << "DummyAudioEngine::StopSource called for source ID: " << source->sourceId << std::endl;
    }

    void DummyAudioEngine::Update() {
        std::cout << "DummyAudioEngine::Update called" << std::endl;
    }

    void DummyAudioEngine::PlayClipAtPoint(AudioClip* clip, const Vector3& position, float volume) {
        std::cout << "DummyAudioEngine::PlayClipAtPoint called with audio ID: " << clip->m_audiobufferId << std::endl;
    }

    void DummyAudioEngine::SetListenerPosition(const Vector3& position) {
        std::cout << "DummyAudioEngine::SetListenerPosition called with position: " << std::endl;
    }

    void DummyAudioEngine::SetListenerOrientation(const Vector3& forward, const Vector3& up) {
        std::cout << "DummyAudioEngine::SetListenerOrientation called with forward and up: " << std::endl;
    }
}
