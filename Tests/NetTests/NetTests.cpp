#include <gtest/gtest.h>
#include "System/Net/Socket.h"
#include "System/Net/IPAddress.h"
#include "System/Net/IPEndPoint.h"
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace System::Net;
using namespace System::Net::Sockets;

TEST(SocketTests, TcpEchoTest) {
    const int port = 54321;
    IPEndPoint serverEndPoint(IPAddress::Parse("127.0.0.1"), port);

    std::string server_received_data;
    bool server_ready = false;
    std::mutex mtx;
    std::condition_variable cv;

    std::thread serverThread([&]() {
        Socket listener(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
        listener.Bind(serverEndPoint);
        listener.Listen(1);
        
        {
            std::lock_guard<std::mutex> lock(mtx);
            server_ready = true;
        }
        cv.notify_one();

        Socket client = listener.Accept();

        std::vector<unsigned char> buffer(1024);
        int received_count = client.Receive(buffer, buffer.size(), 0);
        
        if (received_count > 0) {
            server_received_data.assign(buffer.begin(), buffer.begin() + received_count);
            client.Send(buffer, received_count, 0);
        }
    });
    
    // Wait for server to be ready
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{ return server_ready; });
    }

    Socket client(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
    client.Connect(serverEndPoint);

    std::string message = "Hello, Socket!";
    std::vector<unsigned char> send_buffer(message.begin(), message.end());
    client.Send(send_buffer);

    std::vector<unsigned char> recv_buffer(1024);
    int received_count = client.Receive(recv_buffer, recv_buffer.size(), 0);
    
    ASSERT_GT(received_count, 0);
    recv_buffer.resize(received_count);
    
    std::string received_message(recv_buffer.begin(), recv_buffer.end());

    EXPECT_EQ(message, received_message);

    serverThread.join();

    EXPECT_EQ(message, server_received_data);
}
