
cmake_minimum_required(VERSION 3.23)
project(clangd-client)

add_executable(
    clangd-client
    src/main.cpp
    )

target_compile_features(
    clangd-client
    PRIVATE
    cxx_std_17
    )

find_package(Threads)
target_link_libraries(
    clangd-client
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    )
