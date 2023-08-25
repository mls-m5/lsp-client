#include "lsp/lspclient.h"
#include "lsp/clangversion.h"
#include <sstream>

lsp::LspClient::LspClient(std::string args)
    : _connection{getClangVersion().string() + " " + args,
                  [this](auto &in) { inputThread(in); }} {}

lsp::LspClient::~LspClient() {
    _abort = true;
}

void lsp::LspClient::exit() {
    _abort = true;
    notify("exit", nlohmann::json{});
}

void lsp::LspClient::inputThread(std::istream &in) {
    using namespace std::literals;

    size_t contentLength = 0;

    auto contentLengthStr = "Content-Length: "sv;
    auto contentTypeStr = "Content-Type: "sv;

    for (std::string line; std::getline(in, line);) {
        if (_abort) {
            break;
        }
        if (line.empty()) {
            continue;
        }

        if (line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty() || contentLength) {
            auto json = nlohmann::json{};
            in >> json;

            handle(json);

            contentLength = 0;

            continue;
        }

        if (line.rfind(contentLengthStr, 0) == 0) {
            contentLength = std::stol(line.substr(contentLengthStr.size()));
        }

        if (_abort) {
            break;
        }
    }
}

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
