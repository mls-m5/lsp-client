#include "connection.h"
#include <iostream>
#include <thread>

void sendInit(Connection &conn) {
    conn.sendRaw("");
}

struct ClientInfo {
    std::string name = "clangd-client";
    std::string version = "0";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientInfo, name, version)

struct InitializeParams {
    static constexpr std::string_view method = "initialize";

    int processId = 0;

    ClientInfo clientInfo;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(InitializeParams, processId, clientInfo)

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

    return 0;
}
