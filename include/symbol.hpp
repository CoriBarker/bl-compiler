#pragma once

#include "type.hpp"
#include "symbol_kind.hpp"

#include <vector>
#include <string>
#include <cstdint>

struct Symbol {
    std::string name;
    std::string qualified_name;
    SymbolKind kind;
    Type type;
    std::vector<Type> parameter_types;
    uint64_t array_size;
    int line;
    int column;
};
