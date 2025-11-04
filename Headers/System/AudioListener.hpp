#ifndef _SYSTEM_AUDIOLISTENER_H
#define _SYSTEM_AUDIOLISTENER_H

#include <System/Component.hpp>

namespace System {
    /// <summary>
    /// usually attached to the main camera to represent the players point of hearing
    /// </summary>
    struct AudioListener : public System::Component {
        static AudioListener* main;
        float m_volume = 1.0f;
        AudioListener() {
            if (!main) {
                main = this;
                m_volume = 1.0f;
            }
        }
#if SYSTEM_AUDIO_OPENAL
        void Update() override {
            alListenerfv(AL_POSITION, this->transform.GetPosition());
        }
#endif
        void SetVolume(float volume) {
            m_volume = volume;
        }
        Component* Clone() const override {
            AudioListener* newSource = new AudioListener(*this);
            newSource->m_volume = m_volume;
            return newSource;
        }
    };
}

#endif