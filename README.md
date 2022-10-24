# clangd-client

Clangd client implemented using linux pipes.

Clangd is started as a separate process, and communication is done through
stdin and stdout.


An example interchange between the client (this program) and the server (clangd)
can look like this:

Request:
```http
Content-Length: 210

{
    "id": 1,
    "jsonrpc": "2.0",
    "method": "initialize",
    "params": {
        "clientInfo": {
            "name": "clangd-client",
            "version": "0"
        },
        "processId": 0
    }
}
```

Notice how the message id of the response matches that of the request from the
server:
```http
Content-Length: 1734

{
  "id": 1,
  "jsonrpc": "2.0",
  "result": {
    "capabilities": {
      "astProvider": true,
      "callHierarchyProvider": true,
      "codeActionProvider": true,
      "compilationDatabase": {
        "automaticReload": true
      },
      "completionProvider": {
        "allCommitCharacters": [
          " ",
          "\t",
          "(",
          ")",
          "[",
          "]",
          "{",
          "}",
          "<",
          ">",
          ":",
          ";",
          ",",
          "+",
          "-",
          "/",
          "*",
          "%",
          "^",
          "&",
          "#",
          "?",
          ".",
          "=",
          "\"",
          "'",
          "|"
        ],
        "resolveProvider": false,
        "triggerCharacters": [
          ".",
          "<",
          ">",
          ":",
          "\"",
          "/"
        ]
      },
      "declarationProvider": true,
      "definitionProvider": true,
      "documentFormattingProvider": true,
      "documentHighlightProvider": true,
      "documentLinkProvider": {
        "resolveProvider": false
      },
      "documentOnTypeFormattingProvider": {
        "firstTriggerCharacter": "\n",
        "moreTriggerCharacter": []
      },
      "documentRangeFormattingProvider": true,
      "documentSymbolProvider": true,
      "executeCommandProvider": {
        "commands": [
          "clangd.applyFix",
          "clangd.applyTweak"
        ]
      },
      "hoverProvider": true,
      "implementationProvider": true,
      "memoryUsageProvider": true,
      "referencesProvider": true,
      "renameProvider": true,
      "selectionRangeProvider": true,
      "semanticTokensProvider": {
        "full": {
          "delta": true
        },
        "legend": {
          "tokenModifiers": [
            "declaration",
            "deprecated",
            "deduced",
            "readonly",
            "static",
            "abstract",
            "dependentName",
            "defaultLibrary",
            "functionScope",
            "classScope",
            "fileScope",
            "globalScope"
          ],
          "tokenTypes": [
            "variable",
            "variable",
            "parameter",
            "function",
            "method",
            "function",
            "property",
            "variable",
            "class",
            "interface",
            "enum",
            "enumMember",
            "type",
            "type",
            "unknown",
            "namespace",
            "typeParameter",
            "concept",
            "type",
            "macro",
            "comment"
          ]
        },
        "range": false
      },
      "signatureHelpProvider": {
        "triggerCharacters": [
          "(",
          ","
        ]
      },
      "textDocumentSync": {
        "change": 2,
        "openClose": true,
        "save": true
      },
      "typeHierarchyProvider": true,
      "workspaceSymbolProvider": true
    },
    "serverInfo": {
      "name": "clangd",
      "version": "Ubuntu clangd version 13.0.1-2ubuntu2.1 linux+grpc x86_64-pc-linux-gnu"
    }
  }
}
```


## Licence

Just slapped on a gnu licence.

## References
https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/
