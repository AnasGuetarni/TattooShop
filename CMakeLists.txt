cmake_minimum_required(VERSION 3.8)
project(TattooShop)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c main_functions.h main_functions.c)
add_executable(TattooShop ${SOURCE_FILES})