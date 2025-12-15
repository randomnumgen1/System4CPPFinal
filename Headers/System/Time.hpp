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
        // Read-only references to the internal values
        static float const& deltaTime;
        static float const& time;
        static float const& realtimeSinceStartup;

        static void startframe();
        static void endframe();
    };
}

#endif