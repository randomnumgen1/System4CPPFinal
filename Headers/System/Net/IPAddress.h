#ifndef _SYSTEM_NET_IPADDRESS_H
#define _SYSTEM_NET_IPADDRESS_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>

namespace System::Net {
    namespace Sockets {
        enum class AddressFamily {
            InterNetwork = 2,
            InterNetworkV6 = 23,
        };

        enum class SocketType {
            Stream = 1,
            Dgram = 2,
        };

        enum class ProtocolType {
            Tcp = 6,
            Udp = 17,
        };

        enum class SocketShutdown {
            Receive = 0,
            Send = 1,
            Both = 2,
        };

        enum class SocketOptionLevel {
            Socket = 1,
            IP = 0,
            IPv6 = 41,
            Tcp = 6,
            Udp = 17,
        };

        enum class SocketOptionName {
            Broadcast = 32,
            DontLinger = -129,
            DontRoute = 16,
            Linger = 128,
            NoDelay = 1,
            ReceiveBuffer = 4098,
            ReceiveTimeout = 4102,
            SendBuffer = 4097,
            SendTimeout = 4101,
        };
    }

    class IPAddress {
    private:
        std::vector<unsigned char> _address;
        Sockets::AddressFamily _family;

    public:
        IPAddress(const std::vector<unsigned char>& address) {
            if (address.size() == 4) {
                _family = Sockets::AddressFamily::InterNetwork;
            }
            else if (address.size() == 16) {
                _family = Sockets::AddressFamily::InterNetworkV6;
            }
            else {
                throw std::runtime_error("Invalid address byte array length.");
            }
            _address = address;
        }

        const std::vector<unsigned char>& GetAddressBytes() const {
            return _address;
        }

        Sockets::AddressFamily GetAddressFamily() const {
            return _family;
        }

        static IPAddress Parse(const std::string& ipString) {
            struct in_addr addr_v4;
            if (inet_pton(AF_INET, ipString.c_str(), &addr_v4) == 1) {
                std::vector<unsigned char> bytes(sizeof(addr_v4));
                memcpy(bytes.data(), &addr_v4, sizeof(addr_v4));
                return IPAddress(bytes);
            }

            struct in6_addr addr_v6;
            if (inet_pton(AF_INET6, ipString.c_str(), &addr_v6) == 1) {
                std::vector<unsigned char> bytes(sizeof(addr_v6));
                memcpy(bytes.data(), &addr_v6, sizeof(addr_v6));
                return IPAddress(bytes);
            }

            throw std::runtime_error("Invalid IP address format.");
        }
    };
}
#endif
