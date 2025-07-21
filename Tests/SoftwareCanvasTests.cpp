#include <gtest/gtest.h>
#include "System/Tools/SoftwareCanvas.h"
#include <fstream>
#include <vector>
#include <chrono>

TEST(SoftwareCanvasTests, Constructor) {
    System::Tools::SoftwareCanvas canvas(100, 100);
    SUCCEED();
}
TEST(SoftwareCanvasTests, ConstructorTiming) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        System::Tools::SoftwareCanvas canvas(100, 100);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Constructor timing (1000 iterations): " << elapsed.count() << " ms" << std::endl;
    SUCCEED();
}
TEST(SoftwareCanvasTests, TimingTest) {
    System::Tools::SoftwareCanvas canvas(10, 10);
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; ++i) {
		canvas.rect(2, 2, 6, 6);
		canvas.clip();
		canvas.setFillStyle(255, 0, 0, 255);
		canvas.rect(0, 0, 10, 10);
		canvas.fill();
	}
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
	
	//fail when the total elapsed time exceeds 30 ms
	ASSERT_LT(elapsed.count(), 30) << "Canvas rendering took too long: " << elapsed.count() << " ms";
    std::cout << "TimingTest timing (1000 iterations): " << elapsed.count() << " ms" << std::endl;	
    SUCCEED();
}
TEST(SoftwareCanvasTests, RectTimingTest) {
    System::Tools::SoftwareCanvas canvas(10, 10);
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; ++i) {
		canvas.beginPath();
		canvas.rect(95, 50, 40, 40);
		canvas.stroke();
	}
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
	
	//fail when the total elapsed time exceeds 2 ms as chromes canvas can do this in under 2 ms.
	ASSERT_LT(elapsed.count(), 2) << "Canvas rendering took too long: " << elapsed.count() << " ms";
    std::cout << "TimingTest timing (1000 iterations): " << elapsed.count() << " ms" << std::endl;	
    SUCCEED();
}

