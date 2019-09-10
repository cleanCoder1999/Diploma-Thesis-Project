cmake_minimum_required(VERSION 3.9)
project(_1_MMP_CrackTest)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(_1_MMP_CrackTest main.cpp fileHandler.cpp fileHandler.h disc.cpp disc.h disc-Test.cpp fileHandler-Test.cpp socket.cpp socket.h socket-Test.cpp iTestTypes.h stabilityTestV1.cpp breakTest.cpp manualTest.cpp multiThreading-Test.cpp iTestTypes-Test.cpp iTestTypes.cpp stabilityTest.cpp)
