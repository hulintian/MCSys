cmake_minimum_required(VERSION 3.20)
project(lidar_test LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 源码
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

# Boost
find_package(Boost REQUIRED COMPONENTS system thread chrono atomic)

# OpenGL / GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(lidar_udp_dump
    ${SRC_FILES}
)

target_include_directories(lidar_udp_dump
    PRIVATE
    inc
)

target_link_libraries(lidar_udp_dump
    PRIVATE
    Boost::system
    Boost::thread
    Boost::chrono
    Boost::atomic

    OpenGL::GL
    GLUT::GLUT
)
