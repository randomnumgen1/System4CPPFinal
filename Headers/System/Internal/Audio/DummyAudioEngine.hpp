#ifndef _SYSTEM_AUDIO_DUMMYAUDIOENGINE_HPP_
#define _SYSTEM_AUDIO_DUMMYAUDIOENGINE_HPP_

#include <System/Internal/Audio/AudioEngine.hpp>
#include <vector>


 

namespace System {
    class DummyAudioEngine : public AudioEngine {
    public:
        DummyAudioEngine();
        ~DummyAudioEngine();
        void Initialize() override;
        void Shutdown() override;
        void Update() override;
        void GenerateSource(AudioSource* source) override;
        void DeleteSource(AudioSource* source) override;
        void SetSourceClip(AudioSource* source, unsigned int bufferId) override;
        void SetSourceVolume(AudioSource* source, float volume) override;
        void SetSourceLoop(AudioSource* source, bool loop) override;
        void PlaySourceOneShot(AudioSource* source, unsigned int bufferId, float volumeScale) override;
        void PlaySource(AudioSource* source) override;
        void SetSourcePosition(AudioSource* source, const Vector3& position) override;
        void PauseSource(AudioSource* source) override;
        void StopSource(AudioSource* source) override;
        void PlayClipAtPoint(AudioClip* clip, const Vector3& position, float volume) override;
        void SetListenerPosition(const Vector3& position) override;
        void SetListenerOrientation(const Vector3& forward, const Vector3& up) override;
    private:

        

    };
}

#endif
