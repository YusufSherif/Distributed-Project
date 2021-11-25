cmake_minimum_required(VERSION 3.17)
project(Project_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project_2 main.cpp src/Message.cpp include/Message.h src/UdpSocket.cpp include/UdpSocket.h src/UdpClientSocket.cpp include/UdpClientSocket.h src/UdpServerSocket.cpp include/UdpServerSocket.h src/Client.cpp include/Client.h src/Server.cpp include/Server.h)