// Messages sent from the client to the server

#pragma once

#include "lsptypes.h"
#include "nlohmann/json.hpp"

namespace lsp {

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

// ----------------

// https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#textDocument_completion

struct TextDocumentPositionParams {
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The position inside the text document.
     */
    Position position;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextDocumentPositionParams,
                                   textDocument,
                                   position);

struct PartialResultParams {
    /**
     * An optional token that a server can use to report partial results (e.g.
     * streaming) to the client.
     */
    //	partialResultToken?: ProgressToken;
    nlohmann::json partialResultToken; // int or string
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PartialResultParams, partialResultToken);

enum class CompletionTriggerKind {
    /**
     * Completion was triggered by typing an identifier (24x7 code
     * complete), manual invocation (e.g Ctrl+Space) or via API.
     */
    Invoked = 1,

    /**
     * Completion was triggered by a trigger character specified by
     * the `triggerCharacters` properties of the
     * `CompletionRegistrationOptions`.
     */
    TriggerCharacter = 2,

    /**
     * Completion was re-triggered as the current completion list is incomplete.
     */
    TriggerForIncompleteCompletions = 3,
};

struct CompletionContext {
    /**
     * How the completion was triggered.
     */
    CompletionTriggerKind triggerKind;
    //	triggerKind: CompletionTriggerKind;

    /**
     * The trigger character (a single character) that has trigger code
     * complete. Is undefined if
     * `triggerKind !== CompletionTriggerKind.TriggerCharacter`
     */
    //	triggerCharacter?: string;
    std::string triggerCharacter;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CompletionContext,
                                   triggerKind,
                                   triggerCharacter);

/// Result is of typestruct CompletionList
struct CompletionParams : public TextDocumentPositionParams,
                          public WorkDoneProgressParams,
                          public PartialResultParams {
    static constexpr std::string_view method = "textDocument/completion";
    /**
     * The completion context. This is only available if the client specifies
     * to send this using the client capability
     * `completion.contextSupport === true`
     */
    //    context ?: CompletionContext;
    CompletionContext context;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CompletionParams,
                                   textDocument,
                                   position,
                                   workDoneToken,
                                   partialResultToken,
                                   context);

enum class CompletionItemTag {
    /**
     * Render a completion as obsolete, usually using a strike-out.
     */
    Deprecated = 1,
};

struct CompletionItemLabelDetails {

    /**
     * An optional string which is rendered less prominently directly after
     * {@link CompletionItem.label label}, without any spacing. Should be
     * used for function signatures or type annotations.
     */
    std::string detail;
    //	detail?: string;

    /**
     * An optional string which is rendered less prominently after
     * {@link CompletionItemLabelDetails.detail}. Should be used for fully
     * qualified names or file path.
     */
    //	description?: string;
    std::string description;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(CompletionItemLabelDetails,
                                                detail,
                                                description);

enum class CompletionItemKind {
    Text = 1,
    Method = 2,
    Function = 3,
    Constructor = 4,
    Field = 5,
    Variable = 6,
    Class = 7,
    Interface = 8,
    Module = 9,
    Property = 10,
    Unit = 11,
    Value = 12,
    Enum = 13,
    Keyword = 14,
    Snippet = 15,
    Color = 16,
    File = 17,
    Reference = 18,
    Folder = 19,
    EnumMember = 20,
    Constant = 21,
    Struct = 22,
    Event = 23,
    Operator = 24,
    TypeParameter = 25,
};

/**
 * Defines whether the insert text in a completion item should be interpreted as
 * plain text or a snippet.
 */
enum class InsertTextFormat {
    /**
     * The primary text to be inserted is treated as a plain string.
     */
    PlainText = 1,

    /**
     * The primary text to be inserted is treated as a snippet.
     *
     * A snippet can define tab stops and placeholders with `$1`, `$2`
     * and `${3:foo}`. `$0` defines the final tab stop, it defaults to
     * the end of the snippet. Placeholders with equal identifiers are linked,
     * that is typing in one will update others too.
     */
    Snippet = 2,
};

/**
 * How whitespace and indentation is handled during completion
 * item insertion.
 *
 * @since 3.16.0
 */
enum InsertTextMode {
    /**
     * The insertion or replace strings is taken as it is. If the
     * value is multi line the lines below the cursor will be
     * inserted using the indentation defined in the string value.
     * The client will not apply any kind of adjustments to the
     * string.
     */
    asIs = 1,

    /**
     * The editor adjusts leading whitespace of new lines so that
     * they match the indentation up to the cursor of the line for
     * which the item is accepted.
     *
     * Consider a line like this: <2tabs><cursor><3tabs>foo. Accepting a
     * multi line completion item is indented using 2 tabs and all
     * following lines inserted will be indented using 2 tabs as well.
     */
    adjustIndentation = 2,
};

struct TextEdit {
    /**
     * The range of the text document to be manipulated. To insert
     * text into a document create a range where start === end.
     */
    //	range: Range;
    Range range;

    /**
     * The string to be inserted. For delete operations use an
     * empty string.
     */
    std::string newText;
    //	newText: string;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(TextEdit, range, newText);

struct CompletionItem {

    /**
     * The label of this completion item.
     *
     * The label property is also by default the text that
     * is inserted when selecting this completion.
     *
     * If label details are provided the label itself should
     * be an unqualified name of the completion item.
     */
    std::string label;
    //    label : string;

    /**
     * Additional details for the label
     *
     * @since 3.17.0
     */
    //    labelDetails ?: CompletionItemLabelDetails;
    CompletionItemLabelDetails labelDetails;

    /**
     * The kind of this completion item. Based of the kind
     * an icon is chosen by the editor. The standardized set
     * of available values is defined in `CompletionItemKind`.
     */
    //    kind ?: CompletionItemKind;
    CompletionItemKind kind;

    /**
     * Tags for this completion item.
     *
     * @since 3.15.0
     */
    //    tags ?: CompletionItemTag[];
    std::vector<CompletionItemTag> tags;

    /**
     * A human-readable string with additional information
     * about this item, like type or symbol information.
     */
    //    detail ?: string;
    std::string detail;

    /**
     * A human-readable string that represents a doc-comment.
     */
    //    documentation ?: string | MarkupContent;

    /**
     * Indicates if this item is deprecated.
     *
     * @deprecated Use `tags` instead if supported.
     */
    //    deprecated ?: boolean;

    /**
     * Select this item when showing.
     *
     * *Note* that only one completion item can be selected and that the
     * tool / client decides which item that is. The rule is that the *first*
     * item of those that match best is selected.
     */
    //    preselect ?: boolean;
    bool preselect = false;

    /**
     * A string that should be used when comparing this item
     * with other items. When `falsy` the label is used
     * as the sort text for this item.
     */
    //    sortText ?: string;
    std::string sortText;

    /**
     * A string that should be used when filtering a set of
     * completion items. When `falsy` the label is used as the
     * filter text for this item.
     */
    //    filterText ?: string;
    std::string filterText;

    /**
     * A string that should be inserted into a document when selecting
     * this completion. When `falsy` the label is used as the insert text
     * for this item.
     *
     * The `insertText` is subject to interpretation by the client side.
     * Some tools might not take the string literally. For example
     * VS Code when code complete is requested in this example
     * `con<cursor position>` and a completion item with an `insertText` of
     * `console` is provided it will only insert `sole`. Therefore it is
     * recommended to use `textEdit` instead since it avoids additional client
     * side interpretation.
     */
    //    insertText ?: string;
    std::string insertText;

    /**
     * The format of the insert text. The format applies to both the
     * `insertText` property and the `newText` property of a provided
     * `textEdit`. If omitted defaults to `InsertTextFormat.PlainText`.
     *
     * Please note that the insertTextFormat doesn't apply to
     * `additionalTextEdits`.
     */
    //    insertTextFormat ?: InsertTextFormat;
    InsertTextFormat insertTextFormat;

    /**
     * How whitespace and indentation is handled during completion
     * item insertion. If not provided the client's default value depends on
     * the `textDocument.completion.insertTextMode` client capability.
     *
     * @since 3.16.0
     * @since 3.17.0 - support for `textDocument.completion.insertTextMode`
     */
    //    insertTextMode ?: InsertTextMode;
    InsertTextMode insertTextMode;

    /**
     * An edit which is applied to a document when selecting this completion.
     * When an edit is provided the value of `insertText` is ignored.
     *
     * *Note:* The range of the edit must be a single line range and it must
     * contain the position at which completion has been requested.
     *
     * Most editors support two different operations when accepting a completion
     * item. One is to insert a completion text and the other is to replace an
     * existing text with a completion text. Since this can usually not be
     * predetermined by a server it can report both ranges. Clients need to
     * signal support for `InsertReplaceEdit`s via the
     * `textDocument.completion.completionItem.insertReplaceSupport` client
     * capability property.
     *
     * *Note 1:* The text edit's range as well as both ranges from an insert
     * replace edit must be a [single line] and they must contain the position
     * at which completion has been requested.
     * *Note 2:* If an `InsertReplaceEdit` is returned the edit's insert range
     * must be a prefix of the edit's replace range, that means it must be
     * contained and starting at the same position.
     *
     * @since 3.16.0 additional type `InsertReplaceEdit`
     */
    //    textEdit ?: TextEdit | InsertReplaceEdit;
    TextEdit textEdit;

    /**
     * The edit text used if the completion item is part of a CompletionList and
     * CompletionList defines an item default for the text edit range.
     *
     * Clients will only honor this property if they opt into completion list
     * item defaults using the capability `completionList.itemDefaults`.
     *
     * If not provided and a list's default range is provided the label
     * property is used as a text.
     *
     * @since 3.17.0
     */
    std::string textEditText;

    /**
     * An optional array of additional text edits that are applied when
     * selecting this completion. Edits must not overlap (including the same
     * insert position) with the main edit nor with themselves.
     *
     * Additional text edits should be used to change text unrelated to the
     * current cursor position (for example adding an import statement at the
     * top of the file if the completion item will insert an unqualified type).
     */
    //    additionalTextEdits ?: TextEdit[];

    /**
     * An optional set of characters that when pressed while this completion is
     * active will accept it first and then type that character. *Note* that all
     * commit characters should have `length=1` and that superfluous characters
     * will be ignored.
     */
    //    commitCharacters ?: string[];

    /**
     * An optional command that is executed *after* inserting this completion.
     * *Note* that additional modifications to the current document should be
     * described with the additionalTextEdits-property.
     */
    //    command ?: Command;

    /**
     * A data entry field that is preserved on a completion item between
     * a completion and a completion resolve request.
     */
    //    data ?: LSPAny;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(CompletionItem,
                                                label,
                                                labelDetails,
                                                kind,
                                                tags,
                                                detail,
                                                preselect,
                                                sortText,
                                                insertText,
                                                insertTextFormat,
                                                insertTextMode,
                                                textEdit,
                                                textEditText);

/**
 * Represents a collection of [completion items](#CompletionItem) to be
 * presented in the editor.
 */
struct CompletionList {
    /**
     * This list is not complete. Further typing should result in recomputing
     * this list.
     *
     * Recomputed lists have all their items replaced (not appended) in the
     * incomplete completion sessions.
     */
    bool isIncomplete = false;

    /**
     * The completion items.
     */
    std::vector<CompletionItem> items;

    //	/**
    //	 * In many cases the items of an actual completion result share the same
    //	 * value for properties like `commitCharacters` or the range of a text
    //	 * edit. A completion list can therefore define item defaults which will
    //	 * be used if a completion item itself doesn't specify the value.
    //	 *
    //	 * If a completion list specifies a default value and a completion item
    //	 * also specifies a corresponding value the one from the item is used.
    //	 *
    //	 * Servers are only allowed to return default values if the client
    //	 * signals support for this via the `completionList.itemDefaults`
    //	 * capability.
    //	 *
    //	 * @since 3.17.0
    //	 */
    //	itemDefaults?: {
    //		/**
    //		 * A default commit character set.
    //		 *
    //		 * @since 3.17.0
    //		 */
    //		commitCharacters?: string[];

    //		/**
    //		 * A default edit range
    //		 *
    //		 * @since 3.17.0
    //		 */
    //		editRange?: Range | {
    //			insert: Range;
    //			replace: Range;
    //		};

    //		/**
    //		 * A default insert text format
    //		 *
    //		 * @since 3.17.0
    //		 */
    //		insertTextFormat?: InsertTextFormat;

    //		/**
    //		 * A default insert text mode
    //		 *
    //		 * @since 3.17.0
    //		 */
    //		insertTextMode?: InsertTextMode;

    //		/**
    //		 * A default data value.
    //		 *
    //		 * @since 3.17.0
    //		 */
    //		data?: LSPAny;
    //	}
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(CompletionList,
                                                isIncomplete,
                                                items);

} // namespace lsp