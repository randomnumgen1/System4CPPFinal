#include <fstream>
#include <vector>
#include <chrono>
#include "System/Devices/Gamepad.hpp"



int main() {
    System::Devices::GamePad pad;
	pad.setDebug(true);
    while (true) {
        pad.update();


		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_A))) {
			std::cout << "A button was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_B))) {
			std::cout << "B button was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_X))) {
			std::cout << "X button was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_Y))) {
			std::cout << "Y button was just pressed\n";
		}
		
		
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_DPAD_UP))) {
			std::cout << "BTN_DPAD_UP button was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_DPAD_DOWN))) {
			std::cout << "BTN_DPAD_DOWN button was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_DPAD_LEFT))) {
			std::cout << "BTN_DPAD_LEFT button was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_DPAD_RIGHT))) {
			std::cout << "BTN_DPAD_RIGHT button was just pressed\n";
		}
		
		
		
		
        int lx = pad.getAxis(0);
        int ly = pad.getAxis(1);
        //std::cout << "Left stick: (" << lx << ", " << ly << ")\n";

        usleep(16000); // ~60Hz
    }
}