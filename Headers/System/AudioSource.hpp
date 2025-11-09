#ifndef _SYSTEM_AUDIOSOURCE_H
#define _SYSTEM_AUDIOSOURCE_H

#include <System/Component.hpp>
#include "AudioClip.hpp"
#include "Vector3.hpp"

namespace System {
    /// <summary>
    /// Component that can be optionally attached to a GameObject to enable spatial audio playback.
    /// Plays sounds from the GameObject's position in the scene, using the assigned AudioClip.
    /// we don't need multiple copies of the sound data, just multiple places where it’s being emitted from in our 3D space.
    /// </summary>
    struct AudioSource : public System::Component {
        unsigned int sourceId;
        System::AudioClip* m_clip;
        float m_volume;
        bool m_loop;

        AudioSource();
        ~AudioSource();

        void SetClip(System::AudioClip* clip);
        void SetVolume(float volume);
        void SetLoop(bool loop);
        void PlayOneShot(AudioClip* clip);
        void PlayOneShot(AudioClip* clip, float volumeScale);
        void Play();
        void Pause();
        void Stop();
        void Update();

        Component* Clone() const override {
            AudioSource* newSource = new AudioSource(*this);
            newSource->m_clip = m_clip;
            return newSource;
        }

        static void PlayClipAtPoint(AudioClip* clip, System::Vector3 position);
        static void PlayClipAtPoint(AudioClip* clip, System::Vector3 position, float volume);
    };
}

#endif