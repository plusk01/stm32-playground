cmake_minimum_required(VERSION 3.0)
include(../../cmake/arm-gcc-toolchain.cmake) # or call with cmake -DCMAKE_TOOLCHAIN_FILE=../../../cmake/arm-gcc-toolchain.cmake ..
project(blinky)

# enable_language(ASM)

# find_package(CMSIS REQUIRED)
# find_package(STM32HAL COMPONENTS gpio tim REQUIRED)

add_executable(${PROJECT_NAME} blinky.c)