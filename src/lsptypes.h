#pragma once

#include "nlohmann/json.hpp"
#include <optional>

using DocumentUri = std::string;
using Integer = long;
using UInteger = long;

// template <typename T>
// void to_json(nlohmann::json &j, const std::optional<T> &o) {
//     if (o) {
//         j = o.value();
//     }
// }

//// If it does not exist this function will never be called
// template <typename T>
// void from_json(nlohmann::json &j, const std::optional<T> &o) {
//     o = j.get<T>();
// }
