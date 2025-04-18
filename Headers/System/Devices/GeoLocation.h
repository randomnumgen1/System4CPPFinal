#pragma once
/*


usage:
    Geolocator loldfsgf;
    loldfsgf.OnStatusChanged = [](Status newStatus) {
        if (newStatus == Status::Ready) {
            std::cout << "Geolocator status: Ready" << std::endl;
        }
        };
    loldfsgf.Start();
*/
#include <functional>
#include <thread>
#include <chrono>
namespace System {
    namespace Devices {
        namespace Geolocation {
            enum class PositionAccuracy : char { Default = 0, High = 1 };
            enum class Status { Ready, Initializing, Disabled };

            struct Geolocator {
                PositionAccuracy DesiredAccuracy;
                std::thread statusThread;
                std::function<void(Status)> OnStatusChanged;

                Geolocator() {}

                void Start() {
                    statusThread = std::thread(&Geolocator::StatusThreadFunction, this);
                }

                void TriggerStatusChange(Status newStatus) {
                    if (OnStatusChanged) {
                        OnStatusChanged(newStatus);
                    }
                }

                ~Geolocator() {
                    if (statusThread.joinable()) {
                        statusThread.join();
                    }
                }

            private:
                void StatusThreadFunction() {
                    while (true) {
                        TriggerStatusChange(Status::Ready);
                        std::this_thread::sleep_for(std::chrono::seconds(10));

                    }
                }
            };
        }
    }
}