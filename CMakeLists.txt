cmake_minimum_required(VERSION 3.0)
project(tuna_gui)

set(CMAKE_CXX_STANDARD 14)
set(GIT_EXECUTABLE git)

execute_process(COMMAND
        "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=7 --dirty
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_SHA1
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the date of the commit
execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DATE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the subject of the commit
execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%s
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_SUBJECT
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# generate version.cc
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.cc.in" "${CMAKE_CURRENT_BINARY_DIR}/version.cc" @ONLY)
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/version.cc" version.hh)

add_executable(tuna_gui main.cpp)
include_directories(tuna-ge)
target_link_libraries(tuna_gui ${PROJECT_SOURCE_DIR}/tuna-ge/cmake-build-debug/libtuna_ge.a)
target_sources(tuna_gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/version.hh ${CMAKE_CURRENT_BINARY_DIR}/version.cc)