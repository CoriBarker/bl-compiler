#include "semantic/scope.hpp"

Scope::Scope(std::string name, int level, std::unique_ptr<Scope> parent) : scope_name(name), scope_level(level), parent_scope(parent) {}

std::string Scope::getScopeName() {
    return scope_name;
}

int Scope::getScopeLevel() {
    return scope_level;
}

std::unique_ptr<Scope> Scope::getParentScope() {
    return parent_scope;
}

void Scope::define(Symbol symbol) {
    if (isDeclared(symbol->getName())) {
        // error
    }
    
    else {
        symobls.add(symbol->getName(), symbol);
    }
}

std::unique_ptr<Symbol> Scope::lookup(std::string name) {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return &(it->second);
    }

    return nullptr;
}

bool Scope::isDeclared(std::string name) {
    return lookup(name) != nullptr;
}

std::unordered_map<std::string, Symbol> Scope::getAllSymbols() {
    return symbols;
}

std::string Scope::toString() {
    std::string result = "Scope: " + scope_name + "(level " + std::to_string(scope_level) + ") \n";

    for (auto& pair : symbols) {
        result += "  " + pair.first + " : " + pair.second.getDataType() + "\n";
    }
    
    return result;
}
