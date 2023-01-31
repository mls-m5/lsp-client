#pragma once

#include "nlohmann/json_fwd.hpp"
#include <filesystem>
#include <fstream>
#include <functional>
#include <thread>

namespace lsp {

struct Connection {
    using CallbackT = std::function<void(const nlohmann::json &)>;

    Connection(std::string args, CallbackT callback);
    ~Connection();

    void send(const nlohmann::json &json);

private:
    void readIn();
    void startClangd();

    CallbackT _callback;

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
