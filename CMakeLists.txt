cmake_minimum_required(VERSION 3.28)
project(B_Plus_Tree_HW)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(B_Plus_Tree_HW
        BPlusTree.cpp
        BPlusTree.h
        main.cpp)
