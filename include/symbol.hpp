#pragma once

#include "type.hpp"
#include "symbol_kind.hpp"

#include <vector>
#include <string>

struct Symbol {
    std::string name;
    std::string qualified_name;
    SymbolKind kind;
    Type type;
    std::vector<Type> parameter_types;
    int line;
    int column;
};
