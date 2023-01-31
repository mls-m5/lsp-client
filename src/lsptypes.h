#pragma once

#include "nlohmann/json.hpp"
#include <optional>

using DocumentUri = std::string;
using URI = std::string;
using Integer = long;
using UInteger = long;

struct TextDocumentItem {
    /**
     * The text document's URI.
     */
    DocumentUri uri;

    /**
     * The text document's language identifier.
     */
    std::string languageId;

    /**
     * The version number of this document (it will increase after each
     * change, including undo/redo).
     */
    Integer version;

    /**
     * The content of the opened text document.
     */
    std::string text;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    TextDocumentItem, uri, languageId, version, text);

struct TextDocumentIdentifier {
    DocumentUri uri;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextDocumentIdentifier, uri)

struct VersionedTextDocumentIdentifier : public TextDocumentIdentifier {
    /**
     * The version number of this document.
     *
     * The version number of a document will increase after each change,
     * including undo/redo. The number doesn't need to be consecutive.
     */
    Integer version;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VersionedTextDocumentIdentifier,
                                   uri,
                                   version)

struct Position {
    UInteger line = 0;
    UInteger character = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Position, line, character);

struct Range {
    Position start;
    Position end;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Range, start, end);

struct Location {
    DocumentUri uri;
    Range range;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Location, uri, range);
