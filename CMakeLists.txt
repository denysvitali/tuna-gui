cmake_minimum_required(VERSION 3.0)
project(tuna_gui)

set(CMAKE_CXX_STANDARD 14)

add_executable(tuna_gui main.cpp)
include_directories(tuna-ge)
target_link_libraries(tuna_gui ${PROJECT_SOURCE_DIR}/tuna-ge/cmake-build-debug/libtuna_ge.a)
