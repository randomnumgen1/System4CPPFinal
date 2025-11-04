#ifndef _SYSTEM_AUDIOLISTENER_H
#define _SYSTEM_AUDIOLISTENER_H

#include <System/Component.hpp>
#include <System/Vector3.hpp>
#include <System/Transform.hpp>
#include <System/Scene.hpp> 
#include <System/Internal/AudioEngine.hpp>
namespace System {
    /// <summary>
    /// usually attached to the main camera to represent the players point of hearing
    /// </summary>
    struct AudioListener : public System::Component {
        float m_volume = 1.0f;
        AudioListener() { 
                m_volume = 1.0f; 
        }
        void Update() override {
            Scene::audioEngine->SetListenerPosition(this->transform()->GetPosition()); 
        }
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