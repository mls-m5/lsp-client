// Messages sent from the server to the client

#pragma once

#include "lsptypes.h"
#include <string>

enum TextDocumentSyncKind {
    None = 0,
    Full = 1,
    Incremental = 2,
};

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

struct DidOpenTextDocumentParams {
    static constexpr std::string_view method = "textDocument/didOpen";
    /**
     * The document that was opened.
     */
    TextDocumentItem textDocument;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DidOpenTextDocumentParams, textDocument)

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

struct TextDocumentContentChangeEvent {
    Range range;
    Integer rangeLength;
    std::string text;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(TextDocumentContentChangeEvent,
                                                range,
                                                rangeLength,
                                                text);

struct DidChangeTextDocumentParams {
    /**
     * The document that did change. The version number points
     * to the version after all provided content changes have
     * been applied.
     */
    VersionedTextDocumentIdentifier textDocument;

    /**
     * The actual content changes. The content changes describe single state
     * changes to the document. So if there are two content changes c1 (at
     * array index 0) and c2 (at array index 1) for a document in state S then
     * c1 moves the document from S to S' and c2 from S' to S''. So c1 is
     * computed on the state S and c2 is computed on the state S'.
     *
     * To mirror the content of a document using change events use the following
     * approach:
     * - start with the same initial content
     * - apply the 'textDocument/didChange' notifications in the order you
     *   receive them.
     * - apply the `TextDocumentContentChangeEvent`s in a single notification
     *   in the order you receive them.
     */
    std::vector<TextDocumentContentChangeEvent> contentChanges;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DidChangeTextDocumentParams,
                                   textDocument,
                                   contentChanges);
