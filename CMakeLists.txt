cmake_minimum_required(VERSION 3.25)
project(Sockets)

set(CMAKE_CXX_STANDARD 17)


link_libraries(ws2_32 wsock32)
add_executable(Sockets main.cpp ServerSide.h ClientSide.h)
add_executable(Sockets_client client.cpp ServerSide.h ClientSide.h)

