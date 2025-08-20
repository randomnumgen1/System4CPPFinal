#include <fstream>
#include <vector>
#include <chrono>
#include "System/Devices/Gamepad.hpp"



int main() {
    GamePad pad;

    while (true) {
        pad.update();


		if (pad.wasPressedThisFrame(static_cast<int>(GamePad::ButtonCode::BTN_XBOX_A))) {
			std::cout << "A button was just pressed\n";
		}
        int lx = pad.getAxis(0);
        int ly = pad.getAxis(1);
        std::cout << "Left stick: (" << lx << ", " << ly << ")\n";

        usleep(16000); // ~60Hz
    }
}