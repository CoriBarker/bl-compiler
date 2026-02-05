#pragma once

#include <string>
#include <memory>

class Scope {
public:
    explicit Scope(std::string name, int level, std::unique_ptr<Scope> parent);
    std::string getScopeName();
    int getScopeLevel();
    std::unique_ptr<Scope> getParentScope();
    void define(Symbol symbol);
    std::unique_ptr<Symbol> lookup(std::string name);
    bool isDeclared(std::string name);
    std::unordered_map<std::string, Symbol> getAllSymbols();
    std::string toString();

private:
    std::string scope_name;
    int scope_level
    std::unique_ptr<Scope> parent_scope;
    std::unordered_map<std::string, Symbol>;
};
