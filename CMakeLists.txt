cmake_minimum_required(VERSION 3.11)
project(tuna_gui)

set(CMAKE_CXX_STANDARD 14)
set(GIT_EXECUTABLE git)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif()

execute_process(COMMAND
        "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=7 --dirty
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_SHA1
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the date of the commit
execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DATE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the subject of the commit
execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%s
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_SUBJECT
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# generate version.cc
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.cc.in" "${CMAKE_CURRENT_BINARY_DIR}/version.cc" @ONLY)
include_directories("../tuna-ge/")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/version.hh" "${CMAKE_CURRENT_BINARY_DIR}/version.cc")
list(APPEND SOURCES main.cpp)

add_executable(tuna_gui ${SOURCES})
target_link_libraries(tuna_gui LINK_PUBLIC tuna_ge)
