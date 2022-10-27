// Messages sent from the client to the server

#pragma once

#include "lsptypes.h"
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

struct WorkDoneProgressParams {
    /**
     * An optional token that a server can use to report work done progress.
     */
    nlohmann::json workDoneToken;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(WorkDoneProgressParams,
                                                workDoneToken)

struct DocumentSymbolParams : public WorkDoneProgressParams
/*, PartialResultParams*/ {

    static constexpr std::string_view method = "textDocument/documentSymbol";

    TextDocumentIdentifier textDocument;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DocumentSymbolParams,
                                   textDocument,
                                   workDoneToken)
