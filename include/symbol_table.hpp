#pragma once

#include "scope.hpp"

#include <vector>

lass SymbolTable {
public:
    explicit SymbolTable();
    void addScope(Scope* scope);

private:
    std::vector<Scope*> scopes;
};
