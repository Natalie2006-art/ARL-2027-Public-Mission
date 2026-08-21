cmake_minimum_required(VERSION 3.16)
project(RoverSafetyMonitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(rover_safety_monitor
	main.cpp
	safety_monitor.cpp
)

target_include_directories(rover_safety_monitor PRIVATE include)