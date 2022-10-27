#include "connection.h"
#include "notifications.h"
#include "requests.h"
#include <iostream>
#include <sstream>

using namespace std::literals;

void justPrint(const nlohmann::json &json) {
    std::cout << std::setw(4) << json << std::endl;
}

void getSymbolKinds(Connection &connection) {
    auto content =
        (std::ostringstream{} << std::ifstream{"src/main.cpp"}.rdbuf()).str();

    auto params =
        DocumentSymbolParams{.textDocument = {.uri = "file://src/main.cpp"}};

    connection.request(params, justPrint);
}

void openDocument(Connection &connection) {
    auto content =
        (std::ostringstream{} << std::ifstream{"src/main.cpp"}.rdbuf()).str();

    auto params = DidOpenTextDocumentParams{
        .textDocument =
            TextDocumentItem{
                .uri = "file://src/main.cpp",
                .languageId = "cpp",
                .version = 1,
                .text = content,
            },
    };

    connection.notification(params);

    getSymbolKinds(connection);
}

void test1(Connection &connection) {
    connection.request(InitializeParams{}, [](const nlohmann::json &j) {
        std::cout << "initialization response:\n";
        std::cout << std::setw(2) << j << std::endl;
    });

    // Must wait for server to respond
    // If debugging this might need to be increased to prevent the pipes
    // from being closed prematurely
    std::this_thread::sleep_for(100ms);

    openDocument(connection);

    std::this_thread::sleep_for(1s);
}

int main(int argc, char *argv[]) {
    auto connection = Connection{};

    connection.callback([](auto &&j) {
        std::cout << "unregistered notification:\n";
        std::cout << std::setw(2) << j << std::endl;
    });

    test1(connection);

    return 0;
}
