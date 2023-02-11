cmake_minimum_required(VERSION 3.13)
project(thesis)

add_subdirectory(googletest)
include_directories(googletest/include)

set(CMAKE_CXX_STANDARD 20)

set(TARGET_FILES main.cpp)
SET(CMAKE_CXX_FLAGS  "-pthread")

SET(CMAKE_CXX_FLAGS_DEBUG "-g3")

SET(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
SET(CMAKE_CXX_FLAGS_RELEASE "-Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-Wextra")
SET(CMAKE_CXX_FLAGS_RELEASE "-pedantic")

add_executable(UnitTests tests.cpp)
add_executable(thesis ${TARGET_FILES})

target_link_libraries(UnitTests PRIVATE gtest)