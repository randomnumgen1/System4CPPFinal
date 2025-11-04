#ifndef _SYSTEM_AUDIOENGINE_HPP_
#define _SYSTEM_AUDIOENGINE_HPP_

#include "../AudioSource.hpp"
#include "../Vector3.hpp"

namespace System {
    class AudioEngine {
    public:
        virtual ~AudioEngine() = default;
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
        virtual void Play(AudioSource* source) = 0;
        virtual void Pause(AudioSource* source) = 0;
        virtual void Stop(AudioSource* source) = 0;
        virtual void SetListenerPosition(const Vector3& position) = 0;
    };
}
#endif