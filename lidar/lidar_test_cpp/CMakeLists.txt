cmake_minimum_required(VERSION 3.20)
project(lidar_test LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_policy(SET CMP0167 NEW)


# find all source code
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

# find boost
find_package(Boost REQUIRED COMPONENTS system thread chrono atomic)

# find OpenGL
find_package(OpenGL REQUIRED)
message("Find OpenGL Library at: " ${OPENGL_INCLUDE_DIR})

# find GLUT 
find_package(GLUT REQUIRED)

message("Find GLUT Include DIR at: " ${GLUT_INCLUDE_DIR})
message("Find GLUT Library DIR at: " ${GLUT_LIBRARIES})


include_directories(
    inc
    ${GLUT_INCLUDE_DIR}
)

add_executable(lidar_udp_dump
    ${SRC_FILES})

target_link_libraries(lidar_udp_dump 
    PRIVATE 
    Boost::system
    Boost::thread
    Boost::chrono
    Boost::atomic
    ${GLUT_LIBRARIES}
    ${OPENGL_LIBRARIES}
)
