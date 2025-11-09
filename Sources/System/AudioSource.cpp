#include <System/AudioSource.hpp>
#include <System/Scene.hpp>
#include <System/Internal/AudioEngine.hpp>
#include <System/Transform.hpp>

namespace System {
    AudioSource::AudioSource() : m_clip(nullptr), m_volume(1.0f), m_loop(false) {
        Scene::audioEngine->GenerateSource(this);
    }

    AudioSource::~AudioSource() {
        Scene::audioEngine->DeleteSource(this);
    }

    void AudioSource::SetClip(System::AudioClip* clip) {
        m_clip = clip;
        Scene::audioEngine->SetSourceClip(this, clip->bufferId);
    }

    void AudioSource::SetVolume(float volume) {
        m_volume = volume;
        Scene::audioEngine->SetSourceVolume(this, volume);
    }

    void AudioSource::SetLoop(bool loop) {
        m_loop = loop;
        Scene::audioEngine->SetSourceLoop(this, loop);
    }

    void AudioSource::PlayOneShot(AudioClip* clip) {
        PlayOneShot(clip, 1.0f);
    }

    void AudioSource::PlayOneShot(AudioClip* clip, float volumeScale) {
        Scene::audioEngine->PlaySourceOneShot(this, clip->bufferId, volumeScale);
    }

    void AudioSource::Play() {
        Scene::audioEngine->SetSourcePosition(this, transform()->GetPosition());
        Scene::audioEngine->PlaySource(this);
    }

    void AudioSource::Pause() {
        Scene::audioEngine->PauseSource(this);
    }

    void AudioSource::Stop() {
        Scene::audioEngine->StopSource(this);
    }

    void AudioSource::PlayClipAtPoint(AudioClip* clip, System::Vector3 position) {
        PlayClipAtPoint(clip, position, 1.0f);
    }

    void AudioSource::PlayClipAtPoint(AudioClip* clip, System::Vector3 position, float volume) {
        Scene::audioEngine->PlayClipAtPoint(clip, position, volume);
    }
}
