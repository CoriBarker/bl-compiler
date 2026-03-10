#pragma once

#include "symbol_type.hpp"
#include "type.hpp"

#include <variant>
#include <vector>
#include <string>

class Symbol {
public:
    std::string identifier;
    int line;
    int column;

    virtual ~Symbol() = default;
};

class FunctionDeclarationSymbol : public Symbol {
    Type return_type;
    std::vector<std::string> parameters;

    FunctionDeclarationSymbol(std::string identifier, Type return_type, std::vector<std::string> parameters, int line, int column) : identifier(identifier), return_type(return_type), parameters(parameters), line(line), column(column) { }
};

class VariableDeclarationSymbol : public Symbol {
    Type type;

    VariableDeclarationSymbol
};
