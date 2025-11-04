#include <System/Internal/OpenALEngine.hpp>
#include <System/AudioSource.hpp>

 
#include <al.h>
#include <alc.h>
#include <iostream>

namespace System {
    OpenALEngine::OpenALEngine() : m_device(nullptr), m_context(nullptr) {

    }
    OpenALEngine::~OpenALEngine() {

    }
    void OpenALEngine::Initialize() {
        m_device = alcOpenDevice(nullptr);
        if(!m_device) {
            std::cerr << "Failed to open OpenAL device" << std::endl;
            return;
        }
        m_context = alcCreateContext(m_device, nullptr);
        if(!m_context) {
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
    void OpenALEngine::Play(AudioSource* source) {
        ALuint bufferId = source->m_clip->bufferId;
        alGenSources(1, &source->sourceId);
        alSourcei(source->sourceId, AL_BUFFER, bufferId);
        alSourcePlay(source->sourceId);
    }
    void OpenALEngine::Pause(AudioSource* source) {
        alSourcePause(source->sourceId);
    }
    void OpenALEngine::Stop(AudioSource* source) {
        alSourceStop(source->sourceId);
    }
    void OpenALEngine::SetListenerPosition(const Vector3& position) {
        alListener3f(AL_POSITION, position.x, position.y, position.z);
    }
}
 