cmake_minimum_required(VERSION 3.18)
project(PendulumButterfly)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED network audio graphics window system)
set(SFML_ROOT "/usr/share/SFML/cmake")

add_executable(PendulumButterfly main.cpp double_pendulum.cpp double_pendulum.h)
target_link_libraries(PendulumButterfly sfml-graphics sfml-audio sfml-window sfml-system)
