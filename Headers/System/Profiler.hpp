#ifndef _SYSTEM_PROFILER_H
#define _SYSTEM_PROFILER_H

#include <chrono>
#if defined(SYSTEM_PROFILER_ENABLED)
#define SYSTEM_PROFILER_BEGIN_FRAME() System::Profiler::BeginFrame();
#define SYSTEM_PROFILER_END_FRAME() System::Profiler::EndFrame();
#define SYSTEM_PROFILER_BEGIN_SAMPLE(x) System::Profiler::BeginSample(x);
#define SYSTEM_PROFILER_END_SAMPLE() System::Profiler::EndSample() ;
#else
#define SYSTEM_PROFILER_BEGIN_FRAME()
#define SYSTEM_PROFILER_END_FRAME()
#define SYSTEM_PROFILER_BEGIN_SAMPLE(x)
#define SYSTEM_PROFILER_END_SAMPLE() 
#endif

 

namespace System {
    class Profiler{

    public:
        static void BeginFrame();
        static void EndFrame();
        static void BeginSample(const char* name);
        static void EndSample();

    private: 
        struct Sample {
            const char* name = nullptr; // Initialize to a default value
            std::chrono::high_resolution_clock::time_point start;
            std::chrono::high_resolution_clock::time_point end; 
            Sample(const char* sampleName = nullptr)
                : name(sampleName), start(), end() {}
        };
        static const int maxSamples = 31;
        static int sampleIndex;
        static std::chrono::high_resolution_clock::time_point currentFrameStart;
        static std::array<Sample, maxSamples> samples;
    };
}

#endif // PROFILER_H
