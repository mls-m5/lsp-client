#pragma once

#include "nlohmann/json.hpp"
#include "requestqueue.h"
#include "subscriptions.h"
#include <fstream>
#include <thread>

struct Connection {
    Connection(std::string args);
    ~Connection();

    using CallbackT = std::function<void(const nlohmann::json &)>;

    // Callback for notifications
    void callback(CallbackT f) {
        _callback = f;
    }

    void sendRaw(const nlohmann::json &json);

    // Specify negative id to send without id
    template <typename T>
    long send(const T &value, long id = -1) {
        if (id >= 0) {
            sendRaw({
                {"jsonrpc", "2.0"},
                {"id", id},
                {"method", T::method},
                {"params", value},
            });
        }
        else {
            sendRaw({
                {"jsonrpc", "2.0"},
                {"method", T::method},
                {"params", value},
            });
        }
        return id;
    }

    // Alternative way
    void send(std::string_view method,
              const nlohmann::json &json,
              long id = -1) {
        if (id >= 0) {
            sendRaw({
                {"jsonrpc", "2.0"},
                {"id", id},
                {"method", method},
                {"params", json},
            });
        }
        else {
            sendRaw({
                {"jsonrpc", "2.0"},
                {"method", method},
                {"params", json},
            });
        }
    }

    void request(std::string_view method,
                 const nlohmann::json &value,
                 CallbackT callback) {
        auto id = ++_messageId;
        _handling.waitFor(id, callback);
        send(method, value, id);
    }

    template <typename ReqT, typename FT>
    void request(const ReqT &value, FT callback) {
        CallbackT f = [callback](const nlohmann::json &json) {
            callback(json["result"]);
        };

        auto id = ++_messageId;
        _handling.waitFor(id, f);
        send(value, id);
    }

    /// Send notification from the client to the server
    template <typename T>
    void notification(const T &value) {
        send(T::method, value, -1);
    }

    template <typename NotificationT>
    using SubscriptionCallbackT = std::function<void(NotificationT)>;

    template <typename T>
    void subscribe(std::function<void(const T &)> callback) {
        _subscriptions.subscribe<T>(callback);
    }

    template <typename T>
    void unsubscribe() {
        _subscriptions.unsubcribe<T>();
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
    std::thread errorThread;

    RequestQueue _handling;
    Subscriptions _subscriptions;

    std::string args;

    bool isClangDRunning = false;
};
