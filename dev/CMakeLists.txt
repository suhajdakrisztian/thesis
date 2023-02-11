cmake_minimum_required(VERSION 3.13)
project(thesis)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES pstl.cpp)

SET(CMAKE_CXX_FLAGS  "-pthread")

SET(CMAKE_CXX_FLAGS_DEBUG  "--g3")

SET(CMAKE_CXX_FLAGS_RELEASE  "-O3")
SET(CMAKE_CXX_FLAGS_RELEASE  "-Wall")
SET(CMAKE_CXX_FLAGS_RELEASE  "-Wextra")
SET(CMAKE_CXX_FLAGS_RELEASE  "-Wunused")
SET(CMAKE_CXX_FLAGS_RELEASE  "-pedantic")


add_executable(thesis ${SOURCE_FILES})