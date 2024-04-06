cmake_minimum_required(VERSION 3.26)
project(emb1 C)

set(CMAKE_C_STANDARD 11)

add_executable(emb1 main.c
        prints.c
        prints.h
        input.c
        input.h
        operations.c
        operations.h)
