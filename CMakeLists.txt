
cmake_minimum_required(VERSION 3.23)
project(clangd-client LANGUAGES CXX)

add_executable(
    clangd-client
    src/main.cpp
    src/connection.cpp
    src/clangversion.cpp
    )

target_compile_features(
    clangd-client
    PRIVATE
    cxx_std_17
    )

target_include_directories(
    clangd-client
    PRIVATE
    src
    )

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
                cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(Threads)
target_link_libraries(
    clangd-client
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    nlohmann_json::nlohmann_json
    )


file(COPY src DESTINATION .)
file(COPY tests DESTINATION .)
