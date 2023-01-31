#pragma once

#include "nlohmann/json.hpp"
#include <functional>
#include <mutex>

namespace lsp {

// Match responses with requests made
struct RequestQueue {
    using CallbackT = std::function<void(const nlohmann::json &)>;

    void waitFor(long id, CallbackT callback) {
        auto g = std::lock_guard{_queueMutex};
        _waitingQueue.push_back({id, callback});
    }

    bool operator()(const nlohmann::json &json) {
        if (auto it = json.find("id"); it != json.end()) {
            auto callback = find(it->get<long>());
            if (callback) {
                callback(json);
                return true;
            }
        }
        return false;
    }

private:
    CallbackT find(long id) {
        auto g = std::lock_guard{_queueMutex};
        for (auto it = _waitingQueue.begin(); it != _waitingQueue.end(); ++it) {
            if (it->first == id) {
                auto callback = it->second;
                _waitingQueue.erase(it);
                return callback;
            }
        }

        return {};
    }

    std::mutex _queueMutex;
    std::vector<std::pair<long, CallbackT>> _waitingQueue;
};

} // namespace lsp
