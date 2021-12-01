cmake_minimum_required(VERSION 3.17)
project(Project_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project_2_client client.cpp src/Message.cpp include/Message.h src/UdpSocket.cpp include/UdpSocket.h src/UdpClientSocket.cpp include/UdpClientSocket.h src/UdpServerSocket.cpp include/UdpServerSocket.h src/Client.cpp include/Client.h src/Server.cpp include/Server.h libs/base64.cpp libs/base64.h)

add_executable(Project_2_server server.cpp src/Message.cpp include/Message.h src/UdpSocket.cpp include/UdpSocket.h src/UdpClientSocket.cpp include/UdpClientSocket.h src/UdpServerSocket.cpp include/UdpServerSocket.h src/Client.cpp include/Client.h src/Server.cpp include/Server.h libs/base64.cpp libs/base64.h)