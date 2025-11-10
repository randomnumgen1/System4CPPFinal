#ifndef _SYSTEM_AUDIO_OPENALENGINE_HPP_
#define _SYSTEM_AUDIO_OPENALENGINE_HPP_

#include <System/Internal/Audio/AudioEngineInterface.hpp>
#include <vector>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <al.h>
#include <alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

namespace System {
    class OpenALEngine : public AudioEngineInterface {
    public:
        OpenALEngine();
        ~OpenALEngine();
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

        ALCdevice* m_device;
        ALCcontext* m_context;
        std::vector<unsigned int> m_temporarySources;

    };
}

#endif
