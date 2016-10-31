cmake_minimum_required(VERSION 3.6)
project(seedcup4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp calculator.cpp calculator.hpp fileHandler.hpp fileHandler.cpp grammarAnalyst.cpp grammarAnalyst.hpp iteratorManager.hpp iteratorManager.cpp memoryStack.hpp memoryStack.cpp token.hpp wordAnalyst.hpp wordAnalyst.cpp)
add_executable(seedcup4 ${SOURCE_FILES})