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
		
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_DPAD_UP))) {
			std::cout << "BTN_XBOX_DPAD_UP was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_DPAD_DOWN))) {
			std::cout << "BTN_XBOX_DPAD_DOWN was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_DPAD_LEFT))) {
			std::cout << "BTN_XBOX_DPAD_LEFT was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_DPAD_RIGHT))) {
			std::cout << "BTN_XBOX_DPAD_RIGHT was just pressed\n";
		}		
		
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_STICK_LEFTCLICK))) {
			std::cout << "BTN_XBOX_STICK_LEFTCLICK was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_STICK_RIGHTCLICK))) {
			std::cout << "BTN_XBOX_STICK_RIGHTCLICK was just pressed\n";
		}			
		
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_LT))) {
			std::cout << "BTN_XBOX_LT was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_RT))) {
			std::cout << "BTN_XBOX_RT was just pressed\n";
		}			
		
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_LB))) {
			std::cout << "BTN_XBOX_LB was just pressed\n";
		}
		if (pad.wasPressedThisFrame(static_cast<int>(System::Devices::GamePad::ButtonCode::BTN_XBOX_RB))) {
			std::cout << "BTN_XBOX_RB was just pressed\n";
		}			
		
		
        int lx = pad.getAxis(System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_LEFT_X);
        int ly = pad.getAxis(System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_LEFT_Y);
		
	    int rx = pad.getAxis(System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_RIGHT_X);
        int ry = pad.getAxis(System::Devices::GamePad::ButtonCode::AXIS_CONTROLLER_RIGHT_Y);

		
        std::cout << "Left stick: (" << lx << ", " << ly << ") " << "Right stick: (" << rx << ", " << ry << ")\n";

        usleep(16000); // ~60Hz
    }
}