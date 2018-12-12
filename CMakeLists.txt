cmake_minimum_required(VERSION 2.6)

project(armsim)

option(BUILD_SHARED_LIBS "Create kernel as shared/dynamic library." ON)
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Export all non-static symbols on Win32" ON)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

file(GLOB LibSources src/*.cpp)
file(GLOB LibHeaders lib/*.hpp)

add_library(ask ${LibSources} ${LibHeaders})

add_executable(test1 test/test1.cpp ${LibSources} ${LibHeaders})

enable_testing()
add_test(NAME RoughDesignTest COMMAND test1)