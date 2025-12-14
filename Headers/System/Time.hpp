#ifndef _SYSTEM_TIME_H
#define _SYSTEM_TIME_H

#include <chrono>

namespace System {
    struct Time {
    private:
        static float m_deltaTime;
        static float m_time;
        static float m_realtimeSinceStartup;
        static std::chrono::steady_clock::time_point lastTime;
        static std::chrono::steady_clock::time_point startTime;

    public:
        // The interval in seconds from the last frame to the current one (Read Only).
        static float const& deltaTime;
        static float const& time;
        static float const& realtimeSinceStartup;

        static void startframe() {
            // mark the beginning of the frame
            lastTime = std::chrono::steady_clock::now();

        }

        static void endframe() {
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = currentTime - lastTime;
            m_deltaTime = elapsed.count();
            m_time += m_deltaTime;
            m_realtimeSinceStartup =
                std::chrono::duration<float>(currentTime - startTime).count();
            lastTime = currentTime;

        }
    };

    // Static member definitions
    float Time::m_deltaTime = 0;
    float Time::m_time = 0;
    float Time::m_realtimeSinceStartup = 0;
    std::chrono::steady_clock::time_point Time::lastTime = std::chrono::high_resolution_clock::now();
    std::chrono::steady_clock::time_point Time::startTime = std::chrono::steady_clock::now();
    float const& Time::deltaTime = Time::m_deltaTime;
    float const& Time::time = Time::m_time;
    float const& Time::realtimeSinceStartup = Time::m_realtimeSinceStartup;

}
#endif