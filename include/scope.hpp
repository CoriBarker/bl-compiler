#pragma once

#include <string>
#include <unordered_map>

struct Scope {
    std::string scope_name;
    std::unordered_map<std::string, std::string> locals;
    Scope* parent = nullptr;
};
