cmake_minimum_required(VERSION 3.3)
project(flash_tester)


if (0)
    MESSAGE(STATUS "BUILD ARMV7 (ProtoAir)...")

    add_definitions(-DOS_LINUX)
    set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
    set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-gcc")
elseif(1)
    MESSAGE(STATUS "BUILD ARMV5 (Brain)...")
    add_definitions(-DOS_LINUX)
    set(CMAKE_C_COMPILER "arm-linux-gnueabi-gcc")
    set(CMAKE_CXX_COMPILER "arm-linux-gnueabi-gcc")
else ()
endif ()

set(SOURCE_FILES main.c)
add_executable(flash_tester ${SOURCE_FILES})
target_link_libraries(flash_tester m)