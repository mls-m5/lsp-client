#include "lsp/connection.h"
#include "lsp/clangversion.h"
#include "lsp/randomutil.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

using namespace std::literals;

namespace {
void createFifo(std::filesystem::path path) {
    auto command = "mkfifo "s + path.string();
    std::system(command.c_str());
}
} // namespace

namespace lsp {

Connection::Connection(std::string args, HandleFunctionT handle)
    : _args{args} {

    auto tmp = std::filesystem::temp_directory_path();
    _inPath = tmp / ("lsp-in-pipe-" + std::to_string(randomNumber(100000)));
    createFifo(_inPath);
    _outPath = tmp / ("lsp-out-pipe-" + std::to_string(randomNumber(100000)));
    createFifo(_outPath);
    _errorPath =
        tmp / ("lsp-error-pipe-" + std::to_string(randomNumber(100000)));
    createFifo(_errorPath);

    _clangdThread = std::thread{[this] { startClangd(); }};
    _errorThread = std::thread{
        [this] { std::system(("cat " + _errorPath.string()).c_str()); }};

    _thread = std::thread{[handle, this] { readIn(handle); }};

    _out.open(_outPath);
    _error.open(_errorPath);
}

Connection::~Connection() {
    std::ofstream{_inPath}
        << std::endl; // Force the in thread to check if abort is called
    _thread.join();
    _clangdThread.join();
    _errorThread.join();

    std::filesystem::remove(_inPath);
    std::filesystem::remove(_outPath);
    std::filesystem::remove(_errorPath);
}

void Connection::send(std::string_view str) {
    _out << str << std::endl;
}

void Connection::closePipes() {
    _in.close();
    _out.close();
    _error.close();
}

Connection::operator bool() const {
    return _isServerRunning;
}

void Connection::readIn(HandleFunctionT f) {
    _in.open(_inPath);

    f(_in);
}

void Connection::startClangd() {
    auto ss = std::ostringstream{};
    auto clangd = getClangVersion();
    ss << clangd << " " << _args << " > " << _inPath << " < " << _outPath
       << " 2> " << _errorPath;
    _isServerRunning = true;
    std::system(ss.str().c_str());
    _isServerRunning = false;
}

} // namespace lsp
