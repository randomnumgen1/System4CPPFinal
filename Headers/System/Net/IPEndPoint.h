#ifndef _SYSTEM_NET_IPENDPOINT_H
#define _SYSTEM_NET_IPENDPOINT_H

#include "IPAddress.h"

namespace System::Net {
    class IPEndPoint {
    private:
        IPAddress _address;
        int _port;

    public:
        IPEndPoint(IPAddress address, int port)
            : _address(std::move(address)), _port(port) {}

        const IPAddress& Address() const {
            return _address;
        }

        int Port() const {
            return _port;
        }

        System::Net::Sockets::AddressFamily AddressFamily() const {
            return _address.GetAddressFamily();
        }
    };
}
#endif
