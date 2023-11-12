cmake_minimum_required(VERSION 3.26)
project(list2)

set(CMAKE_CXX_STANDARD 17)

add_executable(list2 main.cpp
        Edge.h
        Vertex.cpp
        Vertex.h
        Graph.cpp
        Graph.h)
