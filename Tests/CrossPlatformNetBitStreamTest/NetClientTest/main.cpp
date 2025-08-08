#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <Headers/System/Net/Socket.h>
#include <Headers/System/Net/IPEndPoint.h> 
#include <Headers/System/Net/IPAddress.h>
#include <Headers/System/IO/BitstreamWriter.hpp>

using namespace System::Net;
using namespace System::Net::Sockets;

int main() {
    try {
        Socket clientSocket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);

        IPAddress serverAddress = IPAddress::Parse("127.0.0.1");
        IPEndPoint serverEndPoint(serverAddress, 8080);
		
		
        while (true) {
			std::vector<unsigned char> sendBuffer(1024);
			 System::IO::BitstreamWriter writer(sendBuffer);
			 writer.Clear();
			 writer.WriteBits(3, 2);
			
			
            clientSocket.SendTo(sendBuffer, serverEndPoint);
            std::cout << "Hello message sent." << std::endl;

            std::vector<unsigned char> receiveBuffer(1024);
            IPEndPoint remoteEndPoint(IPAddress({0,0,0,0}), 0);
            int bytesReceived = clientSocket.ReceiveFrom(receiveBuffer, remoteEndPoint);

            if (bytesReceived > 0) {
                std::string receivedMsg(receiveBuffer.begin(), receiveBuffer.begin() + bytesReceived);
                std::cout << "Server : " << receivedMsg << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}