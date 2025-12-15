#include <System/Time.hpp>

namespace System {

    // Static member definitions
    float Time::m_deltaTime = 0.0f;
    float Time::m_time = 0.0f;
    float Time::m_realtimeSinceStartup = 0.0f;
    std::chrono::steady_clock::time_point Time::lastTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point Time::startTime = std::chrono::steady_clock::now();

    float const& Time::deltaTime = Time::m_deltaTime;
    float const& Time::time = Time::m_time;
    float const& Time::realtimeSinceStartup = Time::m_realtimeSinceStartup;

    void Time::startframe() {
        // mark the beginning of the frame
        lastTime = std::chrono::steady_clock::now();
    }

    void Time::endframe() {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        m_deltaTime = elapsed.count();
        m_time += m_deltaTime;
        m_realtimeSinceStartup =
            std::chrono::duration<float>(currentTime - startTime).count();
        lastTime = currentTime;
    }

}