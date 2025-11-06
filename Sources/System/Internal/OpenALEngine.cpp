#include <System/Internal/OpenALEngine.hpp>
#include <System/AudioSource.hpp>
#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <al.h>
#include <alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

namespace System {
    OpenALEngine::OpenALEngine() : m_device(nullptr), m_context(nullptr) {}

    OpenALEngine::~OpenALEngine() {}

    void OpenALEngine::Initialize() {
        m_device = alcOpenDevice(nullptr);
        if (!m_device) {
            std::cerr << "Failed to open OpenAL device" << std::endl;
            return;
        }
        m_context = alcCreateContext(m_device, nullptr);
        if (!m_context) {
            std::cerr << "Failed to create OpenAL context" << std::endl;
            alcCloseDevice(m_device);
            return;
        }
        alcMakeContextCurrent(m_context);
    }

    void OpenALEngine::Shutdown() {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_context);
        alcCloseDevice(m_device);
    }

    void OpenALEngine::GenerateSource(AudioSource* source) {
        alGenSources(1, &source->sourceId);
    }

    void OpenALEngine::DeleteSource(AudioSource* source) {
        alDeleteSources(1, &source->sourceId);
    }

    void OpenALEngine::SetSourceClip(AudioSource* source, unsigned int bufferId) {
        alSourcei(source->sourceId, AL_BUFFER, bufferId);
    }

    void OpenALEngine::SetSourceVolume(AudioSource* source, float volume) {
        alSourcef(source->sourceId, AL_GAIN, volume);
    }

    void OpenALEngine::SetSourceLoop(AudioSource* source, bool loop) {
        alSourcei(source->sourceId, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    }

    void OpenALEngine::PlaySourceOneShot(AudioSource* source, unsigned int bufferId, float volumeScale) {
        alSourcei(source->sourceId, AL_BUFFER, bufferId);
        alSourcef(source->sourceId, AL_GAIN, volumeScale);
        alSourcePlay(source->sourceId);
    }

    void OpenALEngine::PlaySource(AudioSource* source) {
        alSourcePlay(source->sourceId);
    }

    void OpenALEngine::SetSourcePosition(AudioSource* source, const Vector3& position) {
        alSource3f(source->sourceId, AL_POSITION, position.x, position.y, position.z);
    }

    void OpenALEngine::PauseSource(AudioSource* source) {
        alSourcePause(source->sourceId);
    }

    void OpenALEngine::StopSource(AudioSource* source) {
        alSourceStop(source->sourceId);
    }

    void OpenALEngine::Update() {
        for (auto it = m_temporarySources.begin(); it != m_temporarySources.end();) {
            ALint state;
            alGetSourcei(*it, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING) {
                alDeleteSources(1, &(*it));
                it = m_temporarySources.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void OpenALEngine::PlayClipAtPoint(AudioClip* clip, const Vector3& position, float volume) {
        unsigned int sourceId;
        alGenSources(1, &sourceId);
        alSourcei(sourceId, AL_BUFFER, clip->bufferId);
        alSourcef(sourceId, AL_GAIN, volume);
        alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
        alSourcePlay(sourceId);
        m_temporarySources.push_back(sourceId);
    }

    void OpenALEngine::SetListenerPosition(const Vector3& position) {
        alListener3f(AL_POSITION, position.x, position.y, position.z);
    }

    void OpenALEngine::SetListenerOrientation(const Vector3& forward, const Vector3& up) {
        ALfloat orientation[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
        alListenerfv(AL_ORIENTATION, orientation);
    }
}
