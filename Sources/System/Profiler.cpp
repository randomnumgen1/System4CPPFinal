#include <System/Profiler.h>
#include <array>
#include <string>
#include <iostream>
namespace System{
    // Definition of static member variables
    int Profiler::sampleIndex = 0;
    std::chrono::high_resolution_clock::time_point Profiler::currentFrameStart;
    std::array<Profiler::Sample, Profiler::maxSamples> Profiler::samples;

 void Profiler::BeginFrame() {
    currentFrameStart = std::chrono::high_resolution_clock::now();
    sampleIndex = 0; // Reset the sample index at the beginning of each frame
}
 void Profiler::EndFrame() {
    std::cout << std::dec;
    auto frameEnd = std::chrono::high_resolution_clock::now();
    auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - currentFrameStart).count();
    std::cout << "Frame Time: " << frameDuration << " microseconds" << std::endl;

    // Print all samples at the end of the frame
    for (int i = 0; i < sampleIndex; ++i) {
        auto sampleDuration = std::chrono::duration_cast<std::chrono::microseconds>(samples[i].end - samples[i].start).count();
        std::cout << "Sample \"" << samples[i].name << "\" Time: " << sampleDuration << " microseconds" << std::endl;
    }
}
 void Profiler::BeginSample(const char* name) {
    if (sampleIndex < maxSamples) {
        samples[sampleIndex].name = name;
        samples[sampleIndex].start = std::chrono::high_resolution_clock::now();
        sampleIndex++;
    }
    else {
        std::cerr << "Maximum sample count reached. Ensure to keep within 31 samples per frame." << std::endl;
    }
}
 void Profiler::EndSample() {
    if (sampleIndex > 0) {
        auto sampleEnd = std::chrono::high_resolution_clock::now();
        samples[sampleIndex - 1].end = sampleEnd;
    }
    else {
        std::cerr << "No sample to end. Ensure BeginSample() was called." << std::endl;
    }
}









}