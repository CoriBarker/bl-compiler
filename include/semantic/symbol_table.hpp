#pragma once

class SymbolTable {
public:
    explicit SymbolTable();
    ~SymbolTable() = default;
    void enterScope(std::string scope_name);
    void exitScope();
    Scope* getCurrentScope();
    Scope* getGlobalScope();
    int getScopeLevel();
    void insert(Symbol symbol);
    Symbol* lookup(std::string name);
    Symbol* lookupCurrentScope(std::string name);
    bool isDeclared(std::string name);
    bool isDeclaredInCurrentScope(std::string name);
    void display();
    std::string toString();

private:
    std::vector<std::unique_ptr<Scope>> scopes;
    Scope* current_scope;
    int scope_level;
    Scope* global_scope;
};
