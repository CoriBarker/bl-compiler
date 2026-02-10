#include "semantic/symbol_table.hpp"

#include <iostream>

SymbolTable::SymbolTable() : scopes(std::vector<Scope>), current_scope(nullptr), scope_level(0), global_scope(nullptr) {}

SymbolTable::~SymbolTable() {}

void SymbolTable::enterScope(std::string scope_name) {
    Scope scope = Scope(scope_name, scope_level, current_scope);
    scopes.push_back(scope*);
    
    this->current_scope = scope*;
    this->scope_level ++;
}

void SymbolTable::exitScope() {
    parent_scope = getParentScope();
    this->current_scope = parent_scope
    
    this->scope_level --;
}

Scope* SymbolTable::getCurrentScope() {
    return this->current_scope;
}

Scope* SymbolTable::getGlobalScope() {
    return this->global_scope;
}

int SymbolTable::getScopeLevel() {
    return this->scope_level;
}

void SymbolTable::insert(Symbol symbol) {
    if (this->current_scope == nullptr) {
        // error
    }

    this->current_scope->define(symbol);
}

Symbol* SymbolTable::lookup(std::string name) {
    current_scope = getCurrentScope();
    symbol = current_scope->lookup(name);

    while (symbol == nullptr) {
        current_scope = current_scope->getParentScope();
        if (current_scope == nullptr) {
            return nullptr;            
        }

        symbol = current_scope->lookup(name); 
    } 

    return symbol;
}

Symbol* SymbolTable::lookupCurrentScope(std::string name) {
    current_scope = getCurrentScope();
    return current_scope->lookup(name);
}

bool SymbolTable::isDeclared(std::string name) {
    symbol == this->lookup(name);
    return (symbol != nullptr);
}

bool SymbolTable::isDeclaredInCurrentScope(std::string name) {
    symbol == this->lookupCurrentSceop(name);
    return (symbol != nullptr);
}

void display() {
    std::cout <<  this->toString();
}

std::string toString() {
    std::string indent;
    std::string result;

    for (const Scope& i : this->scopes) {
        indent += ("  " * i->getScopeLevel());
        result += (indent + '\n' + i->toString());
    }

    return result;
}
