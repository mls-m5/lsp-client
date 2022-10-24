#pragma once

#include "nlohmann/json.hpp"
#include <fstream>
#include <thread>

struct Connection {
    Connection();
    ~Connection();

    using CallbackT = std::function<void(const nlohmann::json &)>;

    void callback(CallbackT f) {
        _callback = f;
    }

    void sendRaw(const nlohmann::json &json);

    template <typename T>
    void send(const T &value) {
        sendRaw({
            {"jsonrpc", "2.0"},
            {"id", ++_messageId},
            {"method", T::method},
            {"params", value},
        });
    }

    // Alternative way
    void send(std::string_view method, const nlohmann::json &json) {
        sendRaw({
            {"jsonrpc", "2.0"},
            {"id", ++_messageId},
            {"method", method},
            {"params", json},
        });
    }

private:
    void readIn();
    void startClangd();

    long _messageId = 4;

    CallbackT _callback;

    std::filesystem::path inPath;
    std::filesystem::path outPath;
    std::filesystem::path errorPath;

    std::ofstream out;
    std::ifstream in;
    std::ifstream error;

    std::thread thread;
    std::thread clangdThread;
};
