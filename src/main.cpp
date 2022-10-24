#include "connection.h"
#include "notifications.h"
#include "requests.h"
#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
    auto connection = Connection{};

    connection.callback(
        [](auto &&j) { std::cout << std::setw(2) << j << std::endl; });

    connection.send(InitializeParams{});

    using namespace std::literals;

    // Must wait for server to respond
    // If debugging this might need to be increased to prevent the pipes from
    // being closed prematurely
    std::this_thread::sleep_for(100ms);

    connection.send("exit", {});

    std::this_thread::sleep_for(100ms);

    return 0;
}
