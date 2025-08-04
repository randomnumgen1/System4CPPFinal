#ifndef _SYSTEM_NET_SOCKETS_H
#define _SYSTEM_NET_SOCKETS_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib
typedef SOCKET socket_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
typedef int socket_t;
#define INVALID_SOCKET -1
#endif

#include <vector>
#include <cstdint>
#include <cstddef>
#include <stdexcept>


namespace System::Net::Sockets{
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

    class Socket {
    private:
        socket_t _socket;
        AddressFamily _addressFamily;
        SocketType _socketType;
        ProtocolType _protocolType;
        bool _isBlocking = true;
        bool _connected = false;
    public:
        void Bind(const IPEndPoint& localEP){
            if (localEP.AddressFamily() == AddressFamily::InterNetwork) {
                sockaddr_in addr;
                memset(&addr, 0, sizeof(addr));
                addr.sin_family = AF_INET;
                addr.sin_port = htons(localEP.Port());
                memcpy(&addr.sin_addr, localEP.Address().GetAddressBytes().data(), 4);
                if (::bind(_socket, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
                    throw std::runtime_error("Bind failed.");
                }
            }
            else { // IPv6
                sockaddr_in6 addr;
                memset(&addr, 0, sizeof(addr));
                addr.sin6_family = AF_INET6;
                addr.sin6_port = htons(localEP.Port());
                memcpy(&addr.sin6_addr, localEP.Address().GetAddressBytes().data(), 16);
                if (::bind(_socket, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
                    throw std::runtime_error("Bind failed.");
                }
            }
        }
        void Listen(int backlog) {
            if (::listen(_socket, backlog) != 0) {
                throw std::runtime_error("Listen failed.");
            }
        }
        Socket Accept() {
            socket_t clientSocket = ::accept(_socket, NULL, NULL);
            if (clientSocket == INVALID_SOCKET) {
                throw std::runtime_error("Accept failed.");
            }
            return Socket(clientSocket, _addressFamily, _socketType, _protocolType, true);
        }
        void Connect(const IPEndPoint& remoteEP) {
            int result;
            if (remoteEP.AddressFamily() == AddressFamily::InterNetwork) {
                sockaddr_in addr;
                memset(&addr, 0, sizeof(addr));
                addr.sin_family = AF_INET;
                addr.sin_port = htons(remoteEP.Port());
                memcpy(&addr.sin_addr, remoteEP.Address().GetAddressBytes().data(), 4);
                result = ::connect(_socket, (struct sockaddr*)&addr, sizeof(addr));
            }
            else { // IPv6
                sockaddr_in6 addr;
                memset(&addr, 0, sizeof(addr));
                addr.sin6_family = AF_INET6;
                addr.sin6_port = htons(remoteEP.Port());
                memcpy(&addr.sin6_addr, remoteEP.Address().GetAddressBytes().data(), 16);
                result = ::connect(_socket, (struct sockaddr*)&addr, sizeof(addr));
            }
            if (result != 0) {
                throw std::runtime_error("Connect failed.");
            }
            _connected = true;
        }

        int Send(const std::vector<unsigned char>& buffer) {
            return Send(buffer, buffer.size(), 0);
        }
        int Send(const std::vector<unsigned char>& buffer, int size, int socketFlags) {
            int bytesSent = ::send(_socket, (const char*)buffer.data(), size, socketFlags);
            if (bytesSent < 0) {
                throw std::runtime_error("Send failed.");
            }
            return bytesSent;
        }
        int Receive(std::vector<unsigned char>& buffer) {
            return Receive(buffer, buffer.size(), 0);
        }
        int Receive(std::vector<unsigned char>& buffer, int size, int socketFlags){
            int bytesReceived = ::recv(_socket, (char*)buffer.data(), size, socketFlags);
            if (bytesReceived < 0) {
                throw std::runtime_error("Receive failed.");
            }
            return bytesReceived;
        }
        void Close() {
            if (_socket != INVALID_SOCKET) {
#ifdef _WIN32
                ::closesocket(_socket);
#else
                ::close(_socket);
#endif
                _socket = INVALID_SOCKET;
                _connected = false;
            }
        }


        bool IsBlocking() const {
            return _isBlocking;
        }
        void SetBlocking(bool value) {
#ifdef _WIN32
            u_long mode = value ? 0 : 1;
            if (ioctlsocket(_socket, FIONBIO, &mode) != 0) {
                throw std::runtime_error("Failed to set blocking mode.");
            }
#else
            int flags = fcntl(_socket, F_GETFL, 0);
            if (flags == -1) {
                throw std::runtime_error("Failed to get socket flags.");
            }
            flags = value ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
            if (fcntl(_socket, F_SETFL, flags) == -1) {
                throw std::runtime_error("Failed to set blocking mode.");
            }
#endif
            _isBlocking = value;
        }
        bool Connected() const {
            return _connected;
        }
        int Available() const {
            unsigned long count = 0;
#ifdef _WIN32
            if (ioctlsocket(_socket, FIONREAD, &count) != 0) {
#else
            if (ioctl(_socket, FIONREAD, &count) != 0) {
#endif
                throw std::runtime_error("Failed to get available bytes.");
            }
            return count;
        }
    };
































 
}
#endif