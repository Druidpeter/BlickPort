# CMakeLists written 
cmake_minimum_required(VERSION 3.21.4)

# Specify C++ 17.0 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Project Name is BlickPort
project(blickport VERSION 0.1)
configure_file(blickport_config.h.in blickport_config.h)

# Our executable output:
add_executable(blickport blickport.cpp entity.cpp main.cpp map.cpp menu.cpp spawner.cpp state.cpp)

target_include_directories(blickport PUBLIC "${PROJECT_BINARY_DIR}" /usr/local/include /usr/include)

# Specify system libs
link_directories(/usr/local/lib64 /usr/lib64)
target_link_libraries(blickport ncurses)
