#ifndef _SYSTEM_TIME_H
#define _SYSTEM_TIME_H
#include <chrono>

namespace System {
    struct Time {
    private:
        static float m_deltaTime;
        static std::chrono::steady_clock::time_point lastTime;
    public:
        // The interval in seconds from the last frame to the current one (Read Only).
        static float const& deltaTime;

        static void startframe() {
            lastTime = std::chrono::high_resolution_clock::now();
        }

        static void endframe() {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = currentTime - lastTime;
            m_deltaTime = elapsed.count();
            lastTime = currentTime;
        }
    };

    // Static member definitions
    float Time::m_deltaTime = 42;
    std::chrono::steady_clock::time_point Time::lastTime = std::chrono::high_resolution_clock::now();
    float const& Time::deltaTime = Time::m_deltaTime;
}
#endif