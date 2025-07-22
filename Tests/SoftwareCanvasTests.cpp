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
TEST(SoftwareCanvasTests, TimingTest) {
	System::Image image = System::Image(1920,1080);
    System::Tools::SoftwareCanvas canvas(image);
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
TEST(SoftwareCanvasTests, RectStrokeTimingTest) {
	System::Image image = System::Image(1920,1080);
    System::Tools::SoftwareCanvas canvas(image);
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
    std::cout << "RectStrokeTimingTest timing (1000 iterations): " << elapsed.count() << " ms" << std::endl;	
    SUCCEED();
}
TEST(SoftwareCanvasTests, RectFillTimingTest) {
	System::Image image = System::Image(1920,1080);
    System::Tools::SoftwareCanvas canvas(image);
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; ++i) {
		canvas.beginPath();
		canvas.rect(95, 50, 40, 40);
		canvas.fill();
	}
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
	
	//fail when the total elapsed time exceeds 2 ms as chromes canvas can do this in under 2 ms.
	ASSERT_LT(elapsed.count(), 2) << "Canvas rendering took too long: " << elapsed.count() << " ms";
    std::cout << "RectFillTimingTest timing (1000 iterations): " << elapsed.count() << " ms" << std::endl;	
    SUCCEED();
}

TEST(SoftwareCanvasTests, LineStrokeTimingTest) {
	System::Image image = System::Image(1920,1080);
    System::Tools::SoftwareCanvas canvas(image);
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; ++i) {

		canvas.beginPath();
		canvas.moveTo(0, 0);
		canvas.lineTo(200, 100);
		canvas.stroke();


	}
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
	
	//fail when the total elapsed time exceeds 2 ms as chromes canvas can do this in under 2 ms.
	ASSERT_LT(elapsed.count(), 2) << "Canvas rendering took too long: " << elapsed.count() << " ms";
    std::cout << "LineStrokeTimingTest timing (1000 iterations): " << elapsed.count() << " ms" << std::endl;	
    SUCCEED();
}
TEST(SoftwareCanvasTests, CircleStrokeTimingTest) {
	System::Image image = System::Image(1920,1080);
    System::Tools::SoftwareCanvas canvas(image);
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; ++i) {

		canvas.beginPath();
		canvas.arc(95, 50, 40, 0, 2 * 3.14f);
		canvas.stroke();


	}
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
	
	//fail when the total elapsed time exceeds 2 ms as chromes canvas can do this in under 2 ms.
	ASSERT_LT(elapsed.count(), 2) << "Canvas rendering took too long: " << elapsed.count() << " ms";
    std::cout << "CircleStrokeTimingTest timing (1000 iterations): " << elapsed.count() << " ms" << std::endl;	
    SUCCEED();
}

 TEST(SoftwareCanvasTests, CircleStrokeTimingTest) {
	System::Image image = System::Image(1920,1080);
    System::Tools::SoftwareCanvas canvas(image);
 
 
 
 
 
     canvas.beginPath(); 
     canvas.moveTo(50,50);
     canvas.lineTo(120,150);
     canvas.lineTo(0,180); 
     canvas.lineTo(120,210);
     canvas.lineTo(50,310);  
     canvas.lineTo(160,250);
     canvas.lineTo(190,370);
     canvas.lineTo(220,250);
     canvas.lineTo(330,310);
     canvas.lineTo(260,210);
     canvas.lineTo(380,180);
     canvas.closePath();
     canvas.stroke();
	 
 }