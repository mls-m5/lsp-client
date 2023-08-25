#include "lsp/lspclient.h"
#include <sstream>

void lsp::LspClient::exit() {
    notify("exit", nlohmann::json{});
}

void lsp::LspClient::inputThread(std::istream &in) {}

void lsp::LspClient::send(const nlohmann::json &json) {
    const auto str = [&] {
        auto ss = std::stringstream{};
        ss << std::setw(2) << json;
        return ss.str();
    }();

    if (!_connection) {
        throw std::runtime_error{"clangd is not running"};
    }

    auto ss = std::ostringstream{};

    ss << "Content-Length: " << str.length() << "\r\n";
    ss << "\r\n";
    ss << str;
    ss << std::endl;

    _connection.send(ss.str());
}
