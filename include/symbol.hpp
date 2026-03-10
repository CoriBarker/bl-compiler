#pragma once

#include "type.hpp"

#include <variant>
#include <vector>
#include <string>

enum SymbolType {
    VARIABLE,
    FUNCTION,
};

class Symbol {
public:
    std::string identifier;
    SymbolType symbol_type;
    Type data_type;
    Type return_type;
    std::vector<std::string> parameter_identifiers;
    std::vector<Type> parameter_types;
    int line;
    int column;

    Symbol(std::string identifier, Type data_type, int line, int column) : identifier(identifier), data_type(data_type), line(line), column(column), symbol_type(SymbolType::VARIABLE) { }

    Symbol(std::string identifier, Type return_type, std::vector<std::string> parameter_identifiers,  std::vector<Type> parameter_types, int line, int column) : identifier(identifier), data_type(data_type), return_type(return_type), parameter_identifiers(parameter_identifiers), parameter_types(parameter_types), line(line), column(column), symbol_type(SymbolType::FUNCTION) { }
};
