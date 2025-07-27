#include <gtest/gtest.h>
#include "System/Tools/SoftwareCanvas.h"
#include <fstream>
#include <vector>
#include <chrono>

TEST(SoftwareCanvasTests, ConstructorTiming) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
		System::Image image = System::Image(1920,1080);
        System::Tools::SoftwareCanvas canvas(image);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Constructor timing (1000 iterations): " << elapsed.count() << " ms" << std::endl;
    SUCCEED();
}

