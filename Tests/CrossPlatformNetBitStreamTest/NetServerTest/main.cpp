#include <iostream>
#include <vector>
#include <string>
#include <Headers/System/Net/Socket.h>
#include <Headers/System/Net/IPEndPoint.h>
#include <Headers/System/Net/IPAddress.h>
#include <Headers/System/IO/BitstreamWriter.hpp>
#include <Headers/System/IO/BitstreamReader.hpp>



using namespace System::Net;
using namespace System::Net::Sockets;

int main() {
    try {
        Socket serverSocket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);

        IPEndPoint localEndPoint(IPAddress::Any, 8080);

        serverSocket.Bind(localEndPoint);
        std::cout << "Server listening on port 8080..." << std::endl;

        while (true) {
            std::vector<unsigned char> buffer(1024);
			System::IO::BitstreamReader reader(buffer);
            IPEndPoint clientEndPoint(IPAddress({0,0,0,0}), 0);

            int bytesReceived = serverSocket.ReceiveFrom(buffer, clientEndPoint);
            if (bytesReceived > 0) {
				std::cout << "[Server recieved client message]" << std::endl;
                std::string receivedMsg(buffer.begin(), buffer.begin() + bytesReceived);
                std::cout << "Client : " << receivedMsg << std::endl;
				std::cout << "Client (decoded opcode): " << reader.ReadBits(2) << std::endl;
				
				
				

                std::string responseMsg = "Hello from server";
                std::vector<unsigned char> responseBuffer(responseMsg.begin(), responseMsg.end());
                serverSocket.SendTo(responseBuffer, clientEndPoint);
                std::cout << "Hello message sent." << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
