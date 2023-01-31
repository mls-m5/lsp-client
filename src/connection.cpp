#include "lsp/connection.h"
#include "lsp/clangversion.h"
#include "lsp/randomutil.h"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <iostream>
#include <sstream>

using namespace std::literals;

namespace {
void createFifo(std::filesystem::path path) {
    auto command = "mkfifo "s + path.string();
    std::system(command.c_str());
}
} // namespace

namespace lsp {

Connection::Connection(std::string args, CallbackT callback)
    : _args{args}
    , _callback{callback} {
    _inPath = "lsp-in-pipe-" + std::to_string(randomNumber(10000));
    createFifo(_inPath);
    _outPath = "lsp-out-pipe-" + std::to_string(randomNumber(10000));
    createFifo(_outPath);
    _errorPath = "lsp-error-pipe-" + std::to_string(randomNumber(10000));
    createFifo(_errorPath);

    _clangdThread = std::thread{[this] { startClangd(); }};
    _errorThread = std::thread{
        [this] { std::system(("cat " + _errorPath.string()).c_str()); }};

    _thread = std::thread{[this] { readIn(); }};

    _out.open(_outPath);
    _error.open(_errorPath);
}

Connection::~Connection() {
    _in.close();
    _out.close();
    _error.close();
    _thread.join();
    _clangdThread.join();
    _errorThread.join();

    std::filesystem::remove(_inPath);
    std::filesystem::remove(_outPath);
    std::filesystem::remove(_errorPath);
}

void Connection::send(const nlohmann::json &json) {
    const auto str = [&] {
        auto ss = std::stringstream{};
        ss << std::setw(2) << json;
        return ss.str();
    }();

    if (!_isServerRunning) {
        std::cerr << "clangd is not running, probably an error\n";
    }

    _out << "Content-Length: " << str.length() << "\r\n";
    _out << "\r\n";
    _out << str;
    _out << std::endl;

    {
        std::cout << "sending:\n";
        std::cout << "Content-Length: " << str.length() << "\r\n";
        std::cout << "\r\n";
        std::cout << str << std::endl;
    }
}

void Connection::readIn() {
    _in.open(_inPath);

    size_t contentLength = 0;

    auto contentLengthStr = "Content-Length: "sv;
    auto contentTypeStr = "Content-Type: "sv;

    for (std::string line; std::getline(_in, line);) {
        if (line.empty()) {
            continue;
        }

        if (line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty() || contentLength) {
            auto json = nlohmann::json{};
            _in >> json;

            _callback(json);

            contentLength = 0;

            continue;
        }

        if (line.rfind(contentLengthStr, 0) == 0) {
            contentLength = std::stol(line.substr(contentLengthStr.size()));
        }

        std::cout << line << std::endl;
    }
}

void Connection::startClangd() {
    auto ss = std::ostringstream{};
    auto clangd = getClangVersion();
    ss << clangd << " " << _args << " > " << _inPath << " < " << _outPath
       << " 2> " << _errorPath;
    std::cout << "starting clangd with: " << ss.str() << std::endl;
    _isServerRunning = true;
    std::system(ss.str().c_str());
    _isServerRunning = false;
}

} // namespace lsp
