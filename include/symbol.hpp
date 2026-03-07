#pragma once

#include "symbol_type.hpp"
#include "type.hpp"

#include <variant>
#include <string>

class Symbol {
public:
    explicit Symbol();

private:
    std::string identifier;

    SymbolType symbol_type;
    std::variant<int, bool, std::string> value;
};
