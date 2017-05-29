#pragma once
#include <unordered_map>
#include "string_view.h"
using dict = std::unordered_map<string_view, std::vector<string_view>>;

void parse(string_view file, dict& values, std::unordered_map<string_view, dict>& named_objects);

