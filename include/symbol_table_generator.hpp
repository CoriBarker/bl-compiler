#pragma once

#include "symbol_table.hpp"
#include "ast_node.hpp"

#include <string>
#include <vector>

class SymbolTableGenerator {
public:
    explicit SymbolTableGenerator();
    SymbolTable generate(ProgramNode* program);

    std::vector<std::string> getErrors() const { return errors; }
   
private:
    SymbolTable table;
    std::vector<std::string> errors;
    int scope_counter;

    void visitProgram(ProgramNode* node);
    void registerImport(const std::string& module_name);
    void visitFunctionDeclaration(FunctionDeclarationNode* node);
    void visitStatement(ASTNode* node);
    void visitVariableDeclaration(VariableDeclarationNode* node);
    void visitArrayDeclaration(ArrayDeclarationNode* node);
    void visitVariableAssignment(VariableAssignmentNode* node);
    void visitArrayAssignment(ArrayAssignmentNode* node);
    void visitIfStatement(IfStatementNode* node);
    void visitWhileStatement(WhileStatementNode* node);
    void visitForStatement(ForStatementNode* node);
    void visitExpression(ASTNode* node);
    void visitFunctionCall(FunctionCallNode* node);

    void error(const std::string& message, int line, int column);
};
