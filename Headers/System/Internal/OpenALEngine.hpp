#ifndef _SYSTEM_AUDIO_OPENALENGINE_HPP_
#define _SYSTEM_AUDIO_OPENALENGINE_HPP_

#include <System/Internal/AudioEngine.hpp>

 
#include <al.h>
#include <alc.h>
 

namespace System {
    class OpenALEngine : public AudioEngine {
    public:
        OpenALEngine();
        ~OpenALEngine();
        void Initialize() override;
        void Shutdown() override;
        void Play(AudioSource* source) override;
        void Pause(AudioSource* source) override;
        void Stop(AudioSource* source) override;
        void SetListenerPosition(const Vector3& position) override;
    private:
 
        ALCdevice* m_device;
        ALCcontext* m_context;
 
    };
}

#endif