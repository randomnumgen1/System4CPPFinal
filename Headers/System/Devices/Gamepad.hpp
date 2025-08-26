

#ifdef _WIN32
#else
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <sys/ioctl.h>
#endif
#include <string>
#include <map>
#include <vector>
#include <iostream>
/*
int main() {
    GamePad pad;

    while (true) {
        pad.update();

        if (pad.isButtonPressed(BTN_SOUTH)) {
            std::cout << "X button pressed\n";
        }

        int lx = pad.getAxis(ABS_X);
        int ly = pad.getAxis(ABS_Y);
        std::cout << "Left stick: (" << lx << ", " << ly << ")\n";

        usleep(16000); // ~60Hz
    }
}

*/
namespace System::Devices {



#ifdef _WIN32
    class GamePad {

    };


#else




    class GamePad {
        bool isDebug = false;

        struct Gamepad_Type {
            char name[256];//Human-readable device name (EVIOCGNAME)
            char serial[256];// Unique identifier (EVIOCGUNIQ)
            float ChargeLevel;// Battery percentage (0.0f to 1.0f)
            uint16_t vendorId;
            uint16_t productId;
            //state
            bool isWireless;
            bool isConnected;

        } ; 
        Gamepad_Type gamepads[4];


    public:
        enum class ButtonCode {
            // Xbox layout (evdev codes)
            BTN_XBOX_A = 304,  // A button
            BTN_XBOX_B = 305,  // B button
            BTN_XBOX_X = 308,  // X button
            BTN_XBOX_Y = 307,  // Y button
            BTN_XBOX_L3 = 317, // Left stick click
            BTN_XBOX_R3 = 318, // Right stick click
            // PlayStation aliases (mapped to same evdev codes)
            BTN_PS_CROSS = BTN_XBOX_A, // Cross (X)
            BTN_PS_CIRCLE = BTN_XBOX_B, // Circle
            BTN_PS_SQUARE = BTN_XBOX_X, // Square
            BTN_PS_TRIANGLE = BTN_XBOX_Y,  // Triangle
            BTN_PS_L3 = 317, // Left stick click
            BTN_PS_R3 = 318, // Right stick click

            BTN_XBOX_DPAD_UP = 544,
            BTN_XBOX_DPAD_DOWN = 545,
            BTN_XBOX_DPAD_LEFT = 546,
            BTN_XBOX_DPAD_RIGHT = 547,


            //face buttons
            BTN_CONTROLLER_NORTH = 0,// Y (Xbox), Triangle (PS)
            BTN_CONTROLLER_EAST = 0,// B (Xbox), Circle (PS)
            BTN_CONTROLLER_SOUTH = 0,// A (Xbox), Cross (PS)
            BTN_CONTROLLER_WEST = 0,// X (Xbox), Square (PS)
            //D-Pad directions
            BTN_CONTROLLER_DPAD_UP = 0,
            BTN_CONTROLLER_DPAD_DOWN = 0,
            BTN_CONTROLLER_DPAD_LEFT = 0,
            BTN_CONTROLLER_DPAD_RIGHT = 0,
            //Stick Clicks
            BTN_CONTROLLER_STICK_LEFTCLICK = 0, //L3
            BTN_CONTROLLER_STICK_RIGHTCLICK = 0,//R3

            BTN_KEYBOARD_A = 30,
            BTN_KEYBOARD_S = 31,
            BTN_KEYBOARD_D = 32,
            BTN_KEYBOARD_W = 17,

            AXIS_PS_LEFT_X = 0,


        };
        GamePad() {
            inotifyFd = inotify_init1(IN_NONBLOCK);
            watchFd = inotify_add_watch(inotifyFd, "/dev/input", IN_CREATE | IN_DELETE);
            scanDevices();
        }
        void setDebug(bool debug) {
            isDebug = debug;
        }
        void setDeadZone() {

        }
        ~GamePad() {
            if (fd >= 0) close(fd);
            if (watchFd >= 0) inotify_rm_watch(inotifyFd, watchFd);
            if (inotifyFd >= 0) close(inotifyFd);
        }

        void update() {
            checkHotplug();
            pollEvents();
        }
        bool wasPressedThisFrame(int code) const {
            auto it = buttonsPressedThisFrame.find(code);
            return it != buttonsPressedThisFrame.end() && it->second;
        }
        bool wasReleasedThisFrame(int code) const {
            auto it = buttonsReleasedThisFrame.find(code);
            return it != buttonsReleasedThisFrame.end() && it->second;
        }
        bool isButtonPressed(int code) const {
            auto it = buttons.find(code);
            return it != buttons.end() && it->second;
        }

        int getAxis(int code) const {
            auto it = axes.find(code);
            return it != axes.end() ? it->second : 0;
        }

    private:
        int fd = -1;
        int inotifyFd = -1;
        int watchFd = -1;
        std::map<int, int> buttons;
        std::map<int, int> axes;
        std::map<int, bool> buttonsPressedThisFrame;
        std::map<int, bool> buttonsReleasedThisFrame;

        void scanDevices() {
            DIR* dir = opendir("/dev/input");
            if (!dir) return;
            if (isDebug) {
                std::cout << "---devices---" << std::endl;
            }
            struct dirent* entry;
            while ((entry = readdir(dir))) {
                if (isDebug) {
                    std::cout << "found device: " << std::string(entry->d_name) << std::endl;
                }
                std::string path = "/dev/input/" + std::string(entry->d_name);

                if (isDebug) {
                    std::cout << "isGamepad: " << isGamepad(path) << std::endl;
                    std::cout << "isGamepad2: " << isGamepad2(path) << std::endl;
                    std::cout << "isKnownController: " << isKnownController(path) << std::endl;
                    std::cout << "isLikelyGamepad: " << isLikelyGamepad(path) << std::endl;


                }

                if (path.find("event") != std::string::npos && isGamepad(path)) {
                    fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                    if (fd >= 0) break;
                }
            }
            if (isDebug) {
                std::cout << "---end devices---" << std::endl;
            }
            closedir(dir);
        }
        inline bool bittest(const unsigned long* bits, int bit) {
            return bits[bit / (sizeof(unsigned long) * 8)] & (1UL << (bit % (sizeof(unsigned long) * 8)));
        }
        bool isLikelyGamepad(const std::string& devicePath) {
            int fd = open(devicePath.c_str(), O_RDONLY);
            if (fd < 0) return false;

            unsigned long evbits[(EV_MAX + 1) / (sizeof(unsigned long) * 8)] = {};
            unsigned long keybits[(KEY_MAX + 1) / (sizeof(unsigned long) * 8)] = {};

            ioctl(fd, EVIOCGBIT(0, sizeof(evbits)), evbits);
            ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybits)), keybits);
            close(fd);

            bool hasKey = bittest(evbits, EV_KEY);
            bool hasAbs = bittest(evbits, EV_ABS);
            bool hasGamepadButton = bittest(keybits, BTN_GAMEPAD);
            for (int i = 0; i < KEY_MAX; ++i) {
                if (bittest(keybits, i)) {
                    std::cout << "Key supported: " << i << std::endl;
                }
            }
            return hasKey && hasAbs && hasGamepadButton;
        }
        bool isGamepad(const std::string& devicePath) {
            int testFd = open(devicePath.c_str(), O_RDONLY);
            if (testFd < 0) return false;

            unsigned long keyBits[(KEY_MAX + 1) / (sizeof(unsigned long) * 8)] = {};
            ioctl(testFd, EVIOCGBIT(EV_KEY, sizeof(keyBits)), keyBits);
            close(testFd);

            bool hasGamepadButton = keyBits[BTN_GAMEPAD / 8] & (1 << (BTN_GAMEPAD % 8));
            return hasGamepadButton;
        }
        bool isGamepad2(const std::string& devicePath) {
            int testFd = open(devicePath.c_str(), O_RDONLY);
            if (testFd < 0) return false;

            unsigned long evbits[EV_MAX / sizeof(unsigned long) + 1] = {};
            ioctl(testFd, EVIOCGBIT(0, sizeof(evbits)), evbits);
            close(testFd);

            return evbits[EV_KEY / 8] & (1 << (EV_KEY % 8)) && evbits[EV_ABS / 8] & (1 << (EV_ABS % 8));
        }
        bool isKnownController(const std::string& devicePath) {
            int localFd = open(devicePath.c_str(), O_RDONLY);
            if (localFd < 0) return false;

            input_id id;
            bool result = false;

            if (ioctl(localFd, EVIOCGID, &id) != -1) {
                struct ControllerID {
                    unsigned short vendor;
                    unsigned short product;
                };

                constexpr ControllerID knownControllers[] = {
                    {0x054c, 0x0ce6}, // PS5
                    {0x054c, 0x05c4}, // PS4 original
                    {0x054c, 0x09cc}, // PS4 revised
                    {0x045e, 0x028e}, // Xbox 360
                    {0x045e, 0x02d1}, // Xbox One
                    {0x045e, 0x0b12}  // Xbox Series X
                };

                for (const auto& c : knownControllers) {
                    if (id.vendor == c.vendor && id.product == c.product) {
                        result = true;
                        break;
                    }
                }
            }

            close(localFd);
            return result;
        }
        void checkHotplug() {
            char buffer[4096];
            int len = read(inotifyFd, buffer, sizeof(buffer));
            if (len > 0) {
                if (fd >= 0) close(fd);
                scanDevices();
            }
        }

        void pollEvents() {
            if (fd < 0) return;

            buttonsPressedThisFrame.clear();
            buttonsReleasedThisFrame.clear();

            input_event ev;
            while (read(fd, &ev, sizeof(ev)) > 0) {
                if (ev.type == EV_KEY) {
                    int prev = buttons[ev.code];
                    buttons[ev.code] = ev.value;

                    if (isDebug) {
                        std::cout << "polling: Code: " << ev.code << ", Value: " << ev.value << std::endl;
                    }

                    if (ev.value == 1 && prev == 0) {
                        buttonsPressedThisFrame[ev.code] = true;
                    }
                    else if (ev.value == 0 && prev == 1) {
                        buttonsReleasedThisFrame[ev.code] = true;
                    }
                }
                else if (ev.type == EV_ABS) {
                    axes[ev.code] = ev.value;
                }
            }
        }
    };

#endif








}