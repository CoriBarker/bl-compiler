#pragma once

#include <string>
#include <memory>

class Scope {
public:
    explicit Scope(std::string name, int level, *Scope parent);
    std::string getScopeName();
    int getScopeLevel();
    *Scope getParentScope();
    void define(Symbol symbol);
    *Symbol lookup(std::string name);
    bool isDeclared(std::string name);
    std::unordered_map<std::string, Symbol> getAllSymbols();
    std::string toString();

private:
    std::string scope_name;
    int scope_level
    *Scope parent_scope;
    std::unordered_map<std::string, Symbol> symbols;
};
