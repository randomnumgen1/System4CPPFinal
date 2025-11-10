#include <System/Internal/Audio/DummyAudioEngine.hpp>
#include <System/AudioSource.hpp>
#include <iostream>

 

namespace System {









    DummyAudioEngine::DummyAudioEngine()  {}

    DummyAudioEngine::~DummyAudioEngine() {}

    void DummyAudioEngine::Initialize() {
 
        std::cerr << "init success DummyAudioEngine device" << std::endl;
    }

    void DummyAudioEngine::Shutdown() {
 
    }

    void DummyAudioEngine::GenerateSource(AudioSource* source) {
 
    }

    void DummyAudioEngine::DeleteSource(AudioSource* source) {
       
    }

    void DummyAudioEngine::SetSourceClip(AudioSource* source, unsigned int bufferId) {
        
    }

    void DummyAudioEngine::SetSourceVolume(AudioSource* source, float volume) {
        
    }

    void DummyAudioEngine::SetSourceLoop(AudioSource* source, bool loop) {
        
    }

    void DummyAudioEngine::PlaySourceOneShot(AudioSource* source, unsigned int bufferId, float volumeScale) {
       
    }


    void DummyAudioEngine::PlaySource(AudioSource* source) {
        std::cout << "DummyAudioEngine::PlaySource called for source ID: " << source->sourceId << std::endl; 
    }
    void DummyAudioEngine::SetSourcePosition(AudioSource* source, const Vector3& position) {
      
    }

    void DummyAudioEngine::PauseSource(AudioSource* source) {
        
    }

    void DummyAudioEngine::StopSource(AudioSource* source) {
        
    }

    void DummyAudioEngine::Update() {
 
    }

    void DummyAudioEngine::PlayClipAtPoint(AudioClip* clip, const Vector3& position, float volume) {
        
    }

    void DummyAudioEngine::SetListenerPosition(const Vector3& position) {
        
    }

    void DummyAudioEngine::SetListenerOrientation(const Vector3& forward, const Vector3& up) {
        
    }
}
