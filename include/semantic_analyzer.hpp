#pragma once

#include "symbol_table.hpp"

class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer();

private:
    SymbolTable symbol_table;
    Scope* current_scope;
    int current_scope_level;
};
