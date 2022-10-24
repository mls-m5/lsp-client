#pragma once

#include "nlohmann/json.hpp"
#include "requestqueue.h"
#include <fstream>
#include <thread>

struct Connection {
    Connection();
    ~Connection();

    using CallbackT = std::function<void(const nlohmann::json &)>;

    // Callback for notifications
    void callback(CallbackT f) {
        _callback = f;
    }

    void sendRaw(const nlohmann::json &json);

    template <typename T>
    long send(const T &value, long id = ++_messageId) {
        sendRaw({
            {"jsonrpc", "2.0"},
            {"id", id},
            {"method", T::method},
            {"params", value},
        });
        return id;
    }

    template <typename T>
    void request(const T &value, CallbackT callback) {
        auto id = ++_messageId;
        _handling.waitFor(id, callback);
        send(value, id);
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

    void handleCallback(const nlohmann::json &json);

    static inline long _messageId = 0;

    CallbackT _callback;

    std::filesystem::path inPath;
    std::filesystem::path outPath;
    std::filesystem::path errorPath;

    std::ofstream out;
    std::ifstream in;
    std::ifstream error;

    std::thread thread;
    std::thread clangdThread;

    RequestQueue _handling;
};
