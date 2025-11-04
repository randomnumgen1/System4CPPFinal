#ifndef _SYSTEM_AUDIOSOURCE_H
#define _SYSTEM_AUDIOSOURCE_H

#include <System/Component.hpp>
#include "AudioClip.hpp"
#include "Vector3.hpp"

namespace System {
    /// <summary>
    /// usually attached to a gameobject to play sounds from that position.
    /// </summary>
    struct AudioSource : public System::Component {
        unsigned int sourceId;
        System::AudioClip* m_clip;
        float m_volume;
        bool m_loop;
        void SetClip(System::AudioClip* clip) {
            m_clip = clip;
        }
        void SetVolume(float volume) {
            m_volume = volume;
        }
        void SetLoop(bool loop) {
            m_loop = loop;
        }
        void PlayOneShot(AudioClip* clip) {
            PlayOneShot(clip,  1.0f);
        }
        void PlayOneShot(AudioClip* clip, float volumeScale) {
        
        }
        void Play(){
        
        }
        void Pause(){
        
        }
        void Stop() {
        
        }
        Component* Clone() const override {
            AudioSource* newSource = new AudioSource(*this);
            newSource->m_clip = m_clip;
            return newSource;
        }
        static void PlayClipAtPoint(AudioClip* clip, System::Vector3 position) {
            PlayClipAtPoint(clip, position, 1.0f);
        }
        static void PlayClipAtPoint(AudioClip* clip, System::Vector3 position, float volume) {
        
        }

    };
}

#endif