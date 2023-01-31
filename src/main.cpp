#include "connection.h"
#include "notifications.h"
#include "requests.h"
#include <iostream>
#include <sstream>

std::filesystem::path testSrc =
    std::filesystem::absolute("tests/testsrc/test1.cpp");

using namespace std::literals;

void justPrint(const nlohmann::json &json) {
    std::cout << std::setw(4) << json << std::endl;
}

void getSymbolKinds(Connection &connection) {
    auto content =
        (std::ostringstream{} << std::ifstream{testSrc}.rdbuf()).str();

    auto params = DocumentSymbolParams{
        .textDocument = {.uri = "file://" + testSrc.string()}};

    connection.request(params, justPrint);
}

void openDocument(Connection &connection) {
    auto content =
        (std::ostringstream{} << std::ifstream{testSrc}.rdbuf()).str();

    auto params = DidOpenTextDocumentParams{
        .textDocument =
            TextDocumentItem{
                .uri = "file://" + testSrc.string(),
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

    std::string argstr;
    {
        // Use this to pass for example "--compile-commands-dir="
        auto args = std::vector<std::string>{argv + 1, argv + argc};
        for (auto &arg : args) {
            argstr += " " + arg;
        }
    }

    auto connection = Connection{argstr};

    connection.subscribe(
        std::function{[](const PublishDiagnosticsParams &params) {
            std::cout << "hello notification!\n";
            std::cout << "registered notification" << std::endl;
            std::cout << std::setw(2);
            std::cout << nlohmann::json{params};
        }});

    connection.callback([](auto &&j) {
        std::cout << "unregistered notification:\n";
        std::cout << std::setw(2) << j << std::endl;
    });

    test1(connection);

    return 0;
}
