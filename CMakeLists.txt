cmake_minimum_required(VERSION 3.17)
project(Project_2)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED system serialization)

include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(Project_2_server server.cpp src/Message.cpp include/Message.h src/UdpSocket.cpp include/UdpSocket.h src/UdpServerSocket.cpp include/UdpServerSocket.h src/Server.cpp include/Server.h libs/base64.cpp libs/base64.h src/Database.cpp include/Database.h include/serialization.hpp include/imageOperations.hpp libs/nlohmann_json/json.hpp)
add_executable(Project_2_client client.cpp src/Message.cpp include/Message.h src/UdpSocket.cpp include/UdpSocket.h src/UdpClientSocket.cpp include/UdpClientSocket.h src/Client.cpp include/Client.h libs/base64.cpp libs/base64.h src/Database.cpp include/Database.h include/serialization.hpp include/imageOperations.hpp libs/nlohmann_json/json.hpp)

target_link_libraries(Project_2_server ${OpenCV_LIBS} ${SQLite3_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(Project_2_client ${OpenCV_LIBS} ${SQLite3_LIBRARIES} ${Boost_LIBRARIES})

