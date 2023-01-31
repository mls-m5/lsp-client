#include "connection.h"
#include "notifications.h"
#include "requests.h"
#include <iostream>
#include <sstream>

std::filesystem::path testSrc =
    std::filesystem::absolute("tests/testsrc/test1.cpp");

using namespace std::literals;

template <typename T = nlohmann::json>
void justPrint(const T &json) {
    std::cout << std::setw(4) << nlohmann::json{json} << std::endl;
}

void getSymbolKinds(Connection &connection) {
    auto content =
        (std::ostringstream{} << std::ifstream{testSrc}.rdbuf()).str();

    auto params = DocumentSymbolParams{
        .textDocument = {.uri = "file://" + testSrc.string()}};

    connection.request(params, justPrint<std::vector<DocumentSymbol>>);
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

void completeTest(Connection &connection) {
    auto params = CompletionParams{};
    params.textDocument.uri = "file://" + testSrc.string();
    params.position.line = 3;
    connection.request(params, justPrint<CompletionList>);
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

    completeTest(connection);

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
            std::cout << nlohmann::json{params} << std::endl;

            for (auto &d : params.diagnostics) {
                std::cout << d.range.start.line << ":" << d.message
                          << std::endl;
            }
        }});

    connection.callback([](auto &&j) {
        std::cout << "unregistered notification:\n";
        std::cout << std::setw(2) << j << std::endl;
    });

    test1(connection);

    return 0;
}
