cmake_minimum_required(VERSION 3.5)
project(Practica1 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

add_executable(Practica1 main.c readFile.c readFile.h deleteFile.c deleteFile.h)