
add_executable(
    test-client
    test1.cpp
    )

target_link_libraries(
    test-client
    PUBLIC
    lsp-client
    )

add_test(COMMAND test-client)

enable_testing()
