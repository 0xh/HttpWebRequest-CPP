cmake_minimum_required(VERSION 3.5)
project(HttpWebRequest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp utils/KeyValuePair.hpp utils/HeaderMap.hpp net/HttpWebRequest.hpp utils/Vector.hpp)
add_executable(HttpWebRequest ${SOURCE_FILES})