cmake_minimum_required(VERSION 3.13)
project(Thesis)

add_subdirectory(googletest)
include_directories(googletest/include)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_CXX_FLAGS  "-pthread")

SET(CMAKE_CXX_FLAGS_DEBUG "-g3")

SET(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
SET(CMAKE_CXX_FLAGS_RELEASE "-Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-Wextra")
SET(CMAKE_CXX_FLAGS_RELEASE "-pedantic")

FILE(GLOB SRCFILES tests/*.cpp)
ADD_EXECUTABLE(UnitTests ${SRCFILES})

add_executable(Thesis main.cpp)

target_link_libraries(UnitTests PRIVATE gtest)