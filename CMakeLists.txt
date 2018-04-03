cmake_minimum_required(VERSION 3.0)
project (padsolver)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -pipe -O0 -g")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -pipe -O3")

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(padsolver padsolver.cpp board.cpp orb.cpp solver.cpp solvestate.cpp)




