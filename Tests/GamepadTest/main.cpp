#include <fstream>
#include <vector>
#include <chrono>
#include "System/Devices/Gamepad.hpp"



int main() {
    GamePad pad;

    while (true) {
        pad.update();

        if (pad.isButtonPressed(0)) {
            std::cout << "X button pressed\n";
        }

        int lx = pad.getAxis(0);
        int ly = pad.getAxis(1);
        std::cout << "Left stick: (" << lx << ", " << ly << ")\n";

        usleep(16000); // ~60Hz
    }
}