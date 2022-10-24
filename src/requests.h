// Messages sent from the client to the server

#pragma once

#include "nlohmann/json.hpp"

struct ClientInfo {
    std::string name = "clangd-client";
    std::string version = "0";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientInfo, name, version)

struct InitializeParams {
    static constexpr std::string_view method = "initialize";

    int processId = 0;

    ClientInfo clientInfo;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(InitializeParams, processId, clientInfo)
