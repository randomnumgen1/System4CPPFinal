#include <gtest/gtest.h>
#include "System/Image.h"
#include <chrono>

TEST(ImageTests, LoadFromPNGTiming) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i) {
        System::Image image = System::Image::LoadFromPNG("workingpng.png");
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "LoadFromPNG timing (10 iterations): " << elapsed.count() << " ms" << std::endl;
    SUCCEED();
}
