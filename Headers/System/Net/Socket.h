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

#include "IPEndPoint.h"

#ifdef _WIN32
class WinsockInitializer {
public:
    WinsockInitializer() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed.");
        }
    }
    ~WinsockInitializer() {
        WSACleanup();
    }
};
static WinsockInitializer wsInitializer;
#endif

namespace System::Net::Sockets {
    class Socket {
    private:
        socket_t _socket;
        AddressFamily _addressFamily;
        SocketType _socketType;
        ProtocolType _protocolType;
        bool _isBlocking = true;
        bool _connected = false;

        Socket(socket_t socket, AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType, bool connected)
            : _socket(socket), _addressFamily(addressFamily), _socketType(socketType), _protocolType(protocolType), _connected(connected) {}
    public:
        Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType)
            : _addressFamily(addressFamily), _socketType(socketType), _protocolType(protocolType) {
            _socket = ::socket((int)addressFamily, (int)socketType, (int)protocolType);
            if (_socket == INVALID_SOCKET) {
                throw std::runtime_error("Socket creation failed.");
            }
        }

        ~Socket() {
            Close();
        }

        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        Socket(Socket&& other) noexcept
            : _socket(other._socket),
            _addressFamily(other._addressFamily),
            _socketType(other._socketType),
            _protocolType(other._protocolType),
            _isBlocking(other._isBlocking),
            _connected(other._connected) {
            other._socket = INVALID_SOCKET;
            other._connected = false;
        }

        Socket& operator=(Socket&& other) noexcept {
            if (this != &other) {
                Close();

                _socket = other._socket;
                _addressFamily = other._addressFamily;
                _socketType = other._socketType;
                _protocolType = other._protocolType;
                _isBlocking = other._isBlocking;
                _connected = other._connected;

                other._socket = INVALID_SOCKET;
                other._connected = false;
            }
            return *this;
        }

        void Bind(const IPEndPoint& localEP) {
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
        int Receive(std::vector<unsigned char>& buffer, int size, int socketFlags) {
            int bytesReceived = ::recv(_socket, (char*)buffer.data(), size, socketFlags);
            if (bytesReceived < 0) {
                throw std::runtime_error("Receive failed.");
            }
            return bytesReceived;
        }

        int SendTo(const std::vector<unsigned char>& buffer, const IPEndPoint& remoteEP) {
            return SendTo(buffer, buffer.size(), 0, remoteEP);
        }

        int SendTo(const std::vector<unsigned char>& buffer, int size, int socketFlags, const IPEndPoint& remoteEP) {
            int bytesSent;
            if (remoteEP.AddressFamily() == AddressFamily::InterNetwork) {
                sockaddr_in addr;
                memset(&addr, 0, sizeof(addr));
                addr.sin_family = AF_INET;
                addr.sin_port = htons(remoteEP.Port());
                memcpy(&addr.sin_addr, remoteEP.Address().GetAddressBytes().data(), 4);
                bytesSent = ::sendto(_socket, (const char*)buffer.data(), size, socketFlags, (struct sockaddr*)&addr, sizeof(addr));
            }
            else { // IPv6
                sockaddr_in6 addr;
                memset(&addr, 0, sizeof(addr));
                addr.sin6_family = AF_INET6;
                addr.sin6_port = htons(remoteEP.Port());
                memcpy(&addr.sin6_addr, remoteEP.Address().GetAddressBytes().data(), 16);
                bytesSent = ::sendto(_socket, (const char*)buffer.data(), size, socketFlags, (struct sockaddr*)&addr, sizeof(addr));
            }
            if (bytesSent < 0) {
                throw std::runtime_error("SendTo failed.");
            }
            return bytesSent;
        }

        int ReceiveFrom(std::vector<unsigned char>& buffer, IPEndPoint& remoteEP) {
            return ReceiveFrom(buffer, buffer.size(), 0, remoteEP);
        }

        int ReceiveFrom(std::vector<unsigned char>& buffer, int size, int socketFlags, IPEndPoint& remoteEP) {
            int bytesReceived;
            if (_addressFamily == AddressFamily::InterNetwork) {
                sockaddr_in addr;
                socklen_t addrLen = sizeof(addr);
                memset(&addr, 0, sizeof(addr));
                bytesReceived = ::recvfrom(_socket, (char*)buffer.data(), size, socketFlags, (struct sockaddr*)&addr, &addrLen);
                if (bytesReceived >= 0) {
                    std::vector<unsigned char> ipBytes(4);
                    memcpy(ipBytes.data(), &addr.sin_addr, 4);
                    remoteEP = IPEndPoint(IPAddress(ipBytes), ntohs(addr.sin_port));
                }
            }
            else { // IPv6
                sockaddr_in6 addr;
                socklen_t addrLen = sizeof(addr);
                memset(&addr, 0, sizeof(addr));
                bytesReceived = ::recvfrom(_socket, (char*)buffer.data(), size, socketFlags, (struct sockaddr*)&addr, &addrLen);
                if (bytesReceived >= 0) {
                    std::vector<unsigned char> ipBytes(16);
                    memcpy(ipBytes.data(), &addr.sin6_addr, 16);
                    remoteEP = IPEndPoint(IPAddress(ipBytes), ntohs(addr.sin6_port));
                }
            }

            if (bytesReceived < 0) {
                throw std::runtime_error("ReceiveFrom failed.");
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