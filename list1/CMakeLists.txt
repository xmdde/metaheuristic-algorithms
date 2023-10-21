cmake_minimum_required(VERSION 3.26)
project(list1)

set(CMAKE_CXX_STANDARD 20)

add_executable(list1 main.cpp
        Edge.cpp
        Edge.h
        Graph.cpp
        Graph.h
        Vertex.cpp
        Vertex.h)
