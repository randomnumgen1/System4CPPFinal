
#ifndef _SYSTEM_AUDIOENGINE_HPP_
#define _SYSTEM_AUDIOENGINE_HPP_

#include "../../AudioSource.hpp"
#include "../../Vector3.hpp"
#include "../../AudioClip.hpp"
#include <cmath>
namespace System {
    class AudioEngine {
    public:
        virtual ~AudioEngine() = default;
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
        virtual void Update() = 0;
        virtual void GenerateSource(AudioSource* source) = 0;
        virtual void DeleteSource(AudioSource* source) = 0;
        virtual void SetSourceClip(AudioSource* source, unsigned int bufferId) = 0;
        virtual void SetSourceVolume(AudioSource* source, float volume) = 0;
        virtual void SetSourceLoop(AudioSource* source, bool loop) = 0;
        virtual void PlaySourceOneShot(AudioSource* source, unsigned int bufferId, float volumeScale) = 0;
        virtual void PlaySource(AudioSource* source) = 0;
        virtual void SetSourcePosition(AudioSource* source, const Vector3& position) = 0;
        virtual void PauseSource(AudioSource* source) = 0;
        virtual void StopSource(AudioSource* source) = 0;
        virtual void PlayClipAtPoint(AudioClip* clip, const Vector3& position, float volume) = 0;
        virtual void SetListenerPosition(const Vector3& position) = 0;
        virtual void SetListenerOrientation(const Vector3& forward, const Vector3& up) = 0;
        inline float dbToVolume(float dB){
            return powf(10.0f, 0.05f * dB);
        }
        inline float VolumeTodB(float volume){
            return 20.0f * log10f(volume);
        }
    };
}
#endif