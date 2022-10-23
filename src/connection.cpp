#include "connection.h"
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

Connection::Connection() {
    thread = std::thread{[this] { readIn(); }};
    clangdThread = std::thread{[this] { startClangd(); }};
}

Connection::~Connection() {
    in.close();
    out.close();
    error.close();
}

void Connection::readIn() {
    for (std::string line; std::getline(in, line);) {
        if (line.empty()) {
            continue;
        }
        if (line.back() == '\r') {
            line.pop_back();
        }

        std::cout << line << std::endl;
    }
}

void Connection::startClangd() {
    auto ss = std::ostringstream{};
    ss << "clangd > " << inPath << " < " << outPath;
    std::system(ss.str().c_str());
}

std::ifstream Connection::createIn(std::filesystem::path &path) {
    path = "in-" + std::to_string(randomNumber(10000));

    createFifo(path);

    return std::ifstream{path.string()};
}

std::ofstream Connection::createOut(std::filesystem::path &path) {
    path = "out-" + std::to_string(randomNumber(10000));

    createFifo(path);

    return std::ofstream{path.string()};
}
