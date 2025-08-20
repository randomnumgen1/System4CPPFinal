

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
class GamePad {
public:
    GamePad() {
        inotifyFd = inotify_init1(IN_NONBLOCK);
        watchFd = inotify_add_watch(inotifyFd, "/dev/input", IN_CREATE | IN_DELETE);
        scanDevices();
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

    void scanDevices() {
        DIR* dir = opendir("/dev/input");
        if (!dir) return;

        struct dirent* entry;
        while ((entry = readdir(dir))) {
            std::string path = "/dev/input/" + std::string(entry->d_name);
            if (path.find("event") != std::string::npos && isSonyController(path)) {
                fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                if (fd >= 0) break;
            }
        }
        closedir(dir);
    }

    bool isGamepad(const std::string& devicePath) {
        int testFd = open(devicePath.c_str(), O_RDONLY);
        if (testFd < 0) return false;

        unsigned long evbits[EV_MAX / sizeof(unsigned long) + 1] = {};
        ioctl(testFd, EVIOCGBIT(0, sizeof(evbits)), evbits);
        close(testFd);

        return evbits[EV_KEY / 8] & (1 << (EV_KEY % 8)) && evbits[EV_ABS / 8] & (1 << (EV_ABS % 8));
    }
    bool isSonyController(const std::string& devicePath) {
        int localFd = open(devicePath.c_str(), O_RDONLY);
        if (localFd < 0) return false;

        input_id id;
        bool result = false;

        if (ioctl(localFd, EVIOCGID, &id) != -1) {
            constexpr unsigned short sonyVendorId = 0x054c;
            constexpr unsigned short sonyProducts[] = {
                0x0ce6, // PS5 DualSense
                0x05c4, // PS4 DualShock 4 (original)
                0x09cc  // PS4 DualShock 4 (revised)
            };

            if (id.vendor == sonyVendorId) {
                for (unsigned short pid : sonyProducts) {
                    if (id.product == pid) {
                        result = true;
                        break;
                    }
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

        input_event ev;
        while (read(fd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_KEY) buttons[ev.code] = ev.value;
            else if (ev.type == EV_ABS) axes[ev.code] = ev.value;
        }
    }
};