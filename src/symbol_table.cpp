#include "symbol_table.hpp"

#include <iostream>

void SymbolTable::enterScope(const std::string& scope_name) {
    auto scope = std::make_unique<Scope>();

    scope->scope_name = scope_name;

    scope->parent = scopes.empty() ? nullptr : scopes.back().get();

    scopes.push_back(std::move(scope));
}

void SymbolTable::exitScope() {
    scopes.pop_back();
}

void SymbolTable::define(const Symbol& symbol) {
    symbols.insert({ symbol.qualified_name, symbol });
    scopes.back()->locals.insert({ symbol.name, symbol.qualified_name });
}

Symbol* SymbolTable::lookup(const std::string& name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        auto& scope = scopes[i];
        auto it = scope->locals.find(name);

        if (it != scope->locals.end()) {
            std::string qualified_name = it->second;
            return lookupQualified(qualified_name);
        } 
    }

    return nullptr;
}

Symbol* SymbolTable::lookupCurrent(const std::string& name) {
    auto scope = scopes.back().get();
    auto it = scope->locals.find(name);
    
    if (it != scope->locals.end()) {
        std::string qualified_name = it->second;
        return lookupQualified(qualified_name);
    }

    return nullptr;
}

Symbol* SymbolTable::lookupQualified(const std::string& qualified_name) {
    auto it = symbols.find(qualified_name);

    if (it == symbols.end()) {
        return nullptr;
    }
    
    return &it->second;
}

Symbol* SymbolTable::lookupName(const std::string& name) {
    for (auto& [qualified_name, symbol] : symbols) {
        if (symbol.name == name) {
            return &symbol;
        }
    }
    return nullptr;
}
