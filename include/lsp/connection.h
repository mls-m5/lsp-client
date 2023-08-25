#pragma once

// #include "nlohmann/json_fwd.hpp"
#include <filesystem>
#include <fstream>
#include <functional>
#include <istream>
#include <string_view>
#include <thread>

namespace lsp {

struct Connection {
    using HandleFunctionT = std::function<void(std::istream &)>;

    Connection(const Connection &) = delete;
    Connection(Connection &&) = delete;
    Connection &operator=(const Connection &) = delete;
    Connection &operator=(Connection &&) = delete;

    /// A new thread is created that gets a reference to the input stream
    Connection(std::string args, HandleFunctionT callback);

    /// Note that you need to
    ~Connection();

    void send(std::string_view);

    /// When it is not possible to request the child program to exit, just close
    /// the pipes
    void closePipes();

    operator bool() const;

private:
    void readIn(HandleFunctionT);
    void startClangd();

    std::filesystem::path _inPath;
    std::filesystem::path _outPath;
    std::filesystem::path _errorPath;

    std::ofstream _out;
    std::ifstream _in;
    std::ifstream _error;

    std::thread _thread;
    std::thread _clangdThread;
    std::thread _errorThread;

    std::string _args;

    bool _isServerRunning = false;
};

} // namespace lsp
