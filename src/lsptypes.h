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

enum class SymbolKind {
    File = 1,
    Module = 2,
    Namespace = 3,
    Package = 4,
    Class = 5,
    Method = 6,
    Property = 7,
    Field = 8,
    Constructor = 9,
    Enum = 10,
    Interface = 11,
    Function = 12,
    Variable = 13,
    Constant = 14,
    String = 15,
    Number = 16,
    Boolean = 17,
    Array = 18,
    Object = 19,
    Key = 20,
    Null = 21,
    EnumMember = 22,
    Struct = 23,
    Event = 24,
    Operator = 25,
    TypeParameter = 26,
};

enum class SymbolTag {
    Deprecated = 1,
};

/**
 * Represents programming constructs like variables, classes, interfaces etc.
 * that appear in a document. Document symbols can be hierarchical and they
 * have two ranges: one that encloses its definition and one that points to its
 * most interesting range, e.g. the range of an identifier.
 */
struct DocumentSymbol {

    /**
     * The name of this symbol. Will be displayed in the user interface and
     * therefore must not be an empty string or a string only consisting of
     * white spaces.
     */
    std::string name;

    /**
     * More detail for this symbol, e.g the signature of a function.
     */
    std::string detail;

    /**
     * The kind of this symbol.
     */
    SymbolKind kind;

    /**
     * Tags for this document symbol.
     *
     * @since 3.16.0
     */
    std::vector<SymbolTag> tags;

    /**
     * The range enclosing this symbol not including leading/trailing whitespace
     * but everything else like comments. This information is typically used to
     * determine if the clients cursor is inside the symbol to reveal in the
     * symbol in the UI.
     */
    Range range;

    /**
     * The range that should be selected and revealed when this symbol is being
     * picked, e.g. the name of a function. Must be contained by the `range`.
     */
    Range selectionRange;

    /**
     * Children of this symbol, e.g. properties of a class.
     */
    std::vector<DocumentSymbol> children;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
    DocumentSymbol, name, detail, kind, tags, range, selectionRange, children);
