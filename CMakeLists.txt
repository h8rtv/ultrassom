cmake_minimum_required(VERSION 3.10)
project(Ultrassom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(ultrassom src/main.cpp)
target_link_libraries(ultrassom ${CONAN_LIBS})
