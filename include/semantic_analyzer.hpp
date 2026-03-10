#pragma once

#include "symbol_table.hpp"
#include "ast_node.hpp"

class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer();
    SymbolTable analyze(ASTNode* node);
    Symbol* analyzeVariable(ASTNode* node);
    Symbol* analyzeFunction(ASTNode* node);
    ASTNode* advance();
    ASTNode* peek();

private:
    SymbolTable symbol_table;
    Scope* current_scope;
    int current_scope_level;
};
