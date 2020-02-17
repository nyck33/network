cmake_minimum_required(VERSION 3.15)
project(projectPart3 C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
        sdp.c
        swap_client.c
        swap_server.c
        test_swap_client.c
        test_swap_server.c)

add_executable(
        test_swap_client.c
        test_swap_server.c)
