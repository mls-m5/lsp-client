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
        static long messageId = 0;
        sendRaw({
            {"jsonrpc", "2.0"},
            {"id", ++messageId},
            {"method", T::method},
            {"params", value},
        });
    }

private:
    void readIn();
    void startClangd();

    CallbackT _callback;

    std::ifstream createIn(std::filesystem::path &path);
    std::ofstream createOut(std::filesystem::path &path);

    std::filesystem::path inPath;
    std::filesystem::path outPath;
    std::filesystem::path errorPath;

    std::ifstream in = createIn(inPath);
    std::ofstream out = createOut(outPath);
    std::ifstream error = createIn(errorPath);

    std::thread thread;
    std::thread clangdThread;
};
