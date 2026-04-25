#pragma once

#include "scope.hpp"
#include "symbol.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class SymbolTable {
public:
    SymbolTable() = default;
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;
    SymbolTable(SymbolTable&&) = default;
    SymbolTable& operator=(SymbolTable&&) = default;

    void enterScope(const std::string& scope_name);
    void exitScope();
    void define(const Symbol& symbol);
    Symbol* lookup(const std::string& name);
    Symbol* lookupCurrent(const std::string& name);
    Symbol* lookupQualified(const std::string& qualified_name);
    
    std::unordered_map<std::string, Symbol> symbols;
    std::vector<std::unique_ptr<Scope>> scopes;
};
