#include "clangversion.h"
#include <iostream>

std::filesystem::path getClangVersion() {
    for (int i = 100; i > 1; --i) {
        auto path =
            std::filesystem::path{"/usr/bin/clangd-" + std::to_string(i)};
        if (std::filesystem::exists(path)) {
            return path;
        }
    }

    auto path = "/usr/bin/clangd";
    if (std::filesystem::exists(path)) {
        return path;
    }

    std::cerr << "no clangd installation found\n";

    return "clangd";
}
