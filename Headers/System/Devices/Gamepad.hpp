#ifndef _SYSTEM_DEVICES_GAMEPAD_H 
#define _SYSTEM_DEVICES_GAMEPAD_H
#define USE_MULTIPAD 1
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
            int playerIndex;
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
        struct Device_t {
        public:
            uint8_t classBitmask[(EV_MAX + 1) / 8];
            uint8_t keyBitmask[(KEY_MAX + 1) / 8];
            uint8_t absBitmask[(ABS_MAX + 1) / 8];
            uint8_t relBitmask[(REL_MAX + 1) / 8];
            uint8_t swBitmask[(SW_MAX + 1) / 8];
            input_id id;



            inline bool bittest8(const uint8_t* bits, int bit) {
                return bits[bit / 8] & (1 << (bit % 8));
            }
            bool getDeviceCapabilities(const std::string& devicePath) {
                int fd = open(devicePath.c_str(), O_RDONLY);
                if (fd < 0) return false;

                ioctl(fd, EVIOCGBIT(0, sizeof(classBitmask)), classBitmask);
                ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keyBitmask)), keyBitmask);
                ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absBitmask)), absBitmask);
                ioctl(fd, EVIOCGBIT(EV_REL, sizeof(relBitmask)), relBitmask);
                ioctl(fd, EVIOCGBIT(EV_SW, sizeof(swBitmask)), swBitmask);

                ioctl(fd, EVIOCGID, &id);


                close(fd);
                return true;
            }
            void print() {
                std::cout << "Device Capabilities:" << std::endl;
                std::cout << "  EV_KEY: " << bittest8(classBitmask, EV_KEY) << std::endl;
                std::cout << "  EV_ABS: " << bittest8(classBitmask, EV_ABS) << std::endl;
                std::cout << "  EV_REL: " << bittest8(classBitmask, EV_REL) << std::endl;
                std::cout << "  EV_SW: " << bittest8(classBitmask, EV_SW) << std::endl;

                if (bittest8(classBitmask, EV_KEY)) {
                    std::cout << "[Keys are Supported]" << std::endl;
                    std::cout << "Supported Keys:" << std::endl;
                    for (int i = 0; i < KEY_MAX; ++i) {
                        if (bittest8(keyBitmask, i)) {
                            std::cout << "    Key " << i << std::endl;
                        }
                    }
                }
                if (bittest8(classBitmask, EV_ABS)) {
                    std::cout << "[axes are Supported]" << std::endl;

                }

            }
            bool hasgamepadkey(){
                return bittest8(keyBitmask, 304);
            }
            bool isKnownController(){
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
                        return true;
                    }
                }
                return false;
            }
        };
    public:
        enum class ButtonCode {
            // *****FACE BUTTONS*****
            
            // Xbox layout (evdev codes)
            BTN_XBOX_A = 304,  // A button
            BTN_XBOX_B = 305,  // B button
            BTN_XBOX_X = 307,  // X button
            BTN_XBOX_Y = 308,  // Y button
            // Playstation layout
            BTN_PS_CROSS = BTN_XBOX_A, // Cross (X)
            BTN_PS_CIRCLE = BTN_XBOX_B, // Circle
            BTN_PS_SQUARE = BTN_XBOX_X, // Square
            BTN_PS_TRIANGLE = BTN_XBOX_Y,  // Triangle
            // Generic layout
            BTN_CONTROLLER_NORTH = BTN_XBOX_Y,// Y (Xbox), Triangle (PS)
            BTN_CONTROLLER_EAST = BTN_XBOX_B,// B (Xbox), Circle (PS)
            BTN_CONTROLLER_SOUTH = BTN_XBOX_A,// A (Xbox), Cross (PS)
            BTN_CONTROLLER_WEST = BTN_XBOX_X,// X (Xbox), Square (PS)

            // *****D-PAD BUTTONS*****

            // Xbox layout (evdev codes)
            BTN_XBOX_DPAD_UP = 544,
            BTN_XBOX_DPAD_DOWN = 545,
            BTN_XBOX_DPAD_LEFT = 546,
            BTN_XBOX_DPAD_RIGHT = 547,
            // Playstation layout
            BTN_PS_DPAD_UP = 544,
            BTN_PS_DPAD_DOWN = 545,
            BTN_PS_DPAD_LEFT = 546,
            BTN_PS_DPAD_RIGHT = 547,
            // Generic layout
            BTN_CONTROLLER_DPAD_UP = 544,
            BTN_CONTROLLER_DPAD_DOWN = 545,
            BTN_CONTROLLER_DPAD_LEFT = 546,
            BTN_CONTROLLER_DPAD_RIGHT = 547,

            // *****STICK BUTTONS*****

            // Xbox layout (evdev codes)
            BTN_XBOX_STICK_LEFTCLICK = 317,
            BTN_XBOX_STICK_RIGHTCLICK = 318,
            // Playstation layout
            BTN_PS_STICK_LEFTCLICK = 317,
            BTN_PS_STICK_RIGHTCLICK = 318,
            // Generic layout
            BTN_CONTROLLER_STICK_LEFTCLICK = 317,
            BTN_CONTROLLER_STICK_RIGHTCLICK = 318,
            // *****TRIGGERS*****

            // Xbox layout (evdev codes)
            BTN_XBOX_LT = 312,  // Left Trigger
            BTN_XBOX_RT = 313,  // Right Trigger

            // Playstation layout
            BTN_PS_L2 = BTN_XBOX_LT,  // L2
            BTN_PS_R2 = BTN_XBOX_RT,  // R2

            // Generic layout
            BTN_CONTROLLER_TRIGGER_LEFT = BTN_XBOX_LT,
            BTN_CONTROLLER_TRIGGER_RIGHT = BTN_XBOX_RT,

            // *****SHOULDER BUTTONS*****

            // Xbox layout (evdev codes)
            BTN_XBOX_LB = 310,  // Left Bumper
            BTN_XBOX_RB = 311,  // Right Bumper

            // Playstation layout
            BTN_PS_L1 = BTN_XBOX_LB,  // L1
            BTN_PS_R1 = BTN_XBOX_RB,  // R1

            // Generic layout
            BTN_CONTROLLER_SHOULDER_LEFT = BTN_XBOX_LB,
            BTN_CONTROLLER_SHOULDER_RIGHT = BTN_XBOX_RB,



     
             

            BTN_KEYBOARD_A = 30,
            BTN_KEYBOARD_S = 31,
            BTN_KEYBOARD_D = 32,
            BTN_KEYBOARD_W = 17,
             

            AXIS_CONTROLLER_LEFT_X = 0,
            AXIS_CONTROLLER_LEFT_Y = 1,

            AXIS_CONTROLLER_RIGHT_X = 3,
            AXIS_CONTROLLER_RIGHT_Y = 4,


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
#if USE_MULTIPAD
        ~GamePad() {
            for (size_t i = 0; i < 4; ++i) {
                if (joysticks[i].fd >= 0) {
                    close(joysticks[i].fd);
                }
            }
            if (watchFd >= 0) inotify_rm_watch(inotifyFd, watchFd);
            if (inotifyFd >= 0) close(inotifyFd);
        }

#else

        ~GamePad() {
            if (fd >= 0) close(fd);
            if (watchFd >= 0) inotify_rm_watch(inotifyFd, watchFd);
            if (inotifyFd >= 0) close(inotifyFd);
        }
#endif
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




        bool wasPressedThisFrame(int playerIndex, ButtonCode code) const {
            if (playerIndex < 0 || playerIndex >= 4) return false;
            const auto& pad = joysticks[playerIndex];
            auto it = pad.buttonsPressedThisFrame.find(static_cast<int>(code));
            return it != pad.buttonsPressedThisFrame.end() && it->second;
        }
        bool wasReleasedThisFrame(int playerIndex, ButtonCode code) const {
            if (playerIndex < 0 || playerIndex >= 4) return false;
            const auto& pad = joysticks[playerIndex];
            auto it = pad.buttonsReleasedThisFrame.find(static_cast<int>(code));
            return it != pad.buttonsReleasedThisFrame.end() && it->second;
        }
        bool isButtonPressed(int playerIndex, ButtonCode code) const {
            if (playerIndex < 0 || playerIndex >= 4) return false;
            const auto& pad = joysticks[playerIndex];
            auto it = pad.buttons.find(static_cast<int>(code));
            return it != pad.buttons.end() && it->second;
        }

        int getAxis(int playerIndex, ButtonCode code) const {
            if (playerIndex < 0 || playerIndex >= 4) return 0;
            const auto& pad = joysticks[playerIndex];
            auto it = pad.axes.find(static_cast<int>(code));
            return it != pad.axes.end() ? it->second : 0;
        }





    private:
        struct Joystick {
            int fd = -1;
            std::string path;
            input_id id;
            std::map<int, int> buttons;
            std::map<int, int> axes;
            std::map<int, bool> buttonsPressedThisFrame;
            std::map<int, bool> buttonsReleasedThisFrame;

            bool isSlotFree() const {
                return fd < 0;
            }
            bool isConnected() const {
                return fd >= 0;
            }
        };
        Joystick joysticks[4];










        int fd = -1;
        int inotifyFd = -1;
        int watchFd = -1;
        std::map<int, int> buttons;
        std::map<int, int> axes;
        std::map<int, bool> buttonsPressedThisFrame;
        std::map<int, bool> buttonsReleasedThisFrame;
#if USE_MULTIPAD

        void scanDevices() {
            std::vector<std::string> presentDevicePaths;
            DIR* dir = opendir("/dev/input");
            if (!dir) return;

            struct dirent* entry;
            while ((entry = readdir(dir))) {
                std::string path = "/dev/input/" + std::string(entry->d_name);
                if (path.find("event") != std::string::npos) {

                    Device_t Device = {}
                    Device.getDeviceCapabilities(path);
                    if (Device.hasgamepadkey() && Device.isKnownController()){
                        presentDevicePaths.push_back(path);
                    }

                    //if (isGamepad(path) && isKnownController(path) && isLikelyGamepad(path)) {
                    //    presentDevicePaths.push_back(path);
                    //}
                }
            }
            closedir(dir);

            // Handle disconnections
            for (int i = 0; i < 4; ++i) {
                if (joysticks[i].isConnected()) {
                    bool found = false;
                    for (const auto& path : presentDevicePaths) {
                        if (joysticks[i].path == path) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        close(joysticks[i].fd);
                        joysticks[i] = {}; // Reset the struct
                    }
                }
            }

            // Handle connections
            for (const auto& path : presentDevicePaths) {
                bool alreadyConnected = false;
                for (int i = 0; i < 4; ++i) {
                    if (joysticks[i].path == path) {
                        alreadyConnected = true;
                        break;
                    }
                }

                if (!alreadyConnected) {
                    for (int i = 0; i < 4; ++i) {
                        if (joysticks[i].isSlotFree()) {
                            int tempFd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                            if (tempFd >= 0) {
                                joysticks[i].fd = tempFd;
                                joysticks[i].path = path;
                                ioctl(tempFd, EVIOCGID, &joysticks[i].id);
                                // You might want to populate more joystick info here
                            }
                            break;
                        }
                    }
                }
            }
        }
#else
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



                if (path.find("event") != std::string::npos) {


                    if (isDebug) {
                        std::cout << "isGamepad: " << isGamepad(path) << std::endl;
                        std::cout << "isKnownController: " << isKnownController(path) << std::endl;
                        std::cout << "isLikelyGamepad: " << isLikelyGamepad(path) << std::endl;
                    }

                    Device_t Device = {}
                    Device.getDeviceCapabilities(path);
                    if (Device.hasgamepadkey() && Device.isKnownController()) {
                        fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                        if (fd >= 0) break;
                    }




                   // if (isGamepad(path) && isKnownController(path) && isLikelyGamepad(path)) {
                   //     fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                    //    if (fd >= 0) break;
                    //}



                }
            }
            if (isDebug) {
                std::cout << "---end devices---" << std::endl;
            }
            closedir(dir);
        }
#endif
        inline bool bittest8(const uint8_t* bits, int bit) {
            return bits[bit / 8] & (1 << (bit % 8));
        }
        inline bool bittest32(const unsigned long* bits, int bit) {
            return bits[bit / (sizeof(unsigned long) * 8)] & (1UL << (bit % (sizeof(unsigned long) * 8)));
        }
        bool isLikelyGamepad(const std::string& devicePath) {
            Device_t Device = {};
            Device.getDeviceCapabilities(devicePath);
            Device.print(); 

            return Device.hasgamepadkey();
        } 
        bool isGamepad(const std::string& devicePath) {
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
#if USE_MULTIPAD
        void checkHotplug() {
            char buffer[4096];
            int len = read(inotifyFd, buffer, sizeof(buffer));
            if (len > 0) {
                scanDevices();
            }
        }
#else
        void checkHotplug() {
            char buffer[4096];
            int len = read(inotifyFd, buffer, sizeof(buffer));
            if (len > 0) {
                if (fd >= 0) close(fd);
                scanDevices();
            }
        }
#endif


#if USE_MULTIPAD
        void pollEvents(){
            for (size_t i = 0; i < 4; ++i) {
                Joystick& joy = joysticks[i];
                //JoyStick doesnt exist so skip it
                if (joy.fd < 0) continue;
                joy.buttonsPressedThisFrame.clear();
                joy.buttonsReleasedThisFrame.clear();

                input_event ev;
                while (read(joy.fd, &ev, sizeof(ev)) > 0) {
                    if (ev.type == EV_KEY){
                        int prev = joy.buttons[ev.code];
                        joy.buttons[ev.code] = ev.value;

                        if (isDebug) {
                            std::cout << "polling: Player " << i << ", Code: " << ev.code << ", Value: " << ev.value << std::endl;
                        }

                        if (ev.value == 1 && prev == 0) {
                            joy.buttonsPressedThisFrame[ev.code] = true;
                        }else if (ev.value == 0 && prev == 1) {
                            joy.buttonsReleasedThisFrame[ev.code] = true;
                        }
                    }else if (ev.type == EV_ABS) {
                        joy.axes[ev.code] = ev.value;
                    }
                }
            }
        }
#else
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
#endif

    };

#endif








}


#endif