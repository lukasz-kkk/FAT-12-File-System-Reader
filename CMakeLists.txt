cmake_minimum_required(VERSION 3.20)
project(FAT_12_FILE_READER C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(FAT_12_FILE_READER
        file_reader.c
        file_reader.h
        main.c)
