#include "connection.h"
#include "clangversion.h"
#include "randomutil.h"
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

Connection::Connection(std::string args)
    : args{args} {
    inPath = "lsp-in-pipe-" + std::to_string(randomNumber(10000));
    createFifo(inPath);
    outPath = "lsp-out-pipe-" + std::to_string(randomNumber(10000));
    createFifo(outPath);
    errorPath = "lsp-error-pipe-" + std::to_string(randomNumber(10000));
    createFifo(errorPath);

    clangdThread = std::thread{[this] { startClangd(); }};
    errorThread = std::thread{
        [this] { std::system(("cat " + errorPath.string()).c_str()); }};

    thread = std::thread{[this] { readIn(); }};

    out.open(outPath);
    error.open(errorPath);
}

Connection::~Connection() {
    in.close();
    out.close();
    error.close();
    thread.join();
    clangdThread.join();
    errorThread.join();

    std::filesystem::remove(inPath);
    std::filesystem::remove(outPath);
    std::filesystem::remove(errorPath);
}

void Connection::sendRaw(const nlohmann::json &json) {
    const auto str = [&] {
        auto ss = std::stringstream{};
        ss << std::setw(2) << json;
        return ss.str();
    }();

    if (!isClangDRunning) {
        std::cerr << "clangd is not running, probably an error\n";
    }

    out << "Content-Length: " << str.length() << "\r\n";
    out << "\r\n";
    out << str;
    out << std::endl;

    {
        std::cout << "sending:\n";
        std::cout << "Content-Length: " << str.length() << "\r\n";
        std::cout << "\r\n";
        std::cout << str << std::endl;
    }
}

void Connection::readIn() {
    in.open(inPath);

    size_t contentLength = 0;

    auto contentLengthStr = "Content-Length: "sv;
    auto contentTypeStr = "Content-Type: "sv;

    for (std::string line; std::getline(in, line);) {
        if (line.empty()) {
            continue;
        }

        if (line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty() || contentLength) {
            auto json = nlohmann::json{};
            in >> json;

            if (!_handling(json) && !_subscriptions(json)) {
                _callback(json);
            }

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
    ss << clangd << " " << args << " > " << inPath << " < " << outPath << " 2> "
       << errorPath;
    std::cout << "starting clangd with: " << ss.str() << std::endl;
    isClangDRunning = true;
    std::system(ss.str().c_str());
    isClangDRunning = false;
}
