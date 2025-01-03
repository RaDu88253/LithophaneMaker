cmake_minimum_required(VERSION 3.29)
project(LithophaneMakerV2 C)

set(CMAKE_C_STANDARD 11)

add_executable(LithophaneMakerV2 main.c
        stl_generator.h
        stl_generator.c
        bitmap_processing.h
        bitmap_processing.c)
