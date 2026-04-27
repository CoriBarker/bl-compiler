#pragma once

#include "symbol_table.hpp"

#include <string>
#include <vector>

class TypeChecker {
public:
    TypeChecker(SymbolTable& table);

    void check(ProgramNode* node);
    std::vector<std::string>> getErrors();

private:
    SymbolTable& table;
    std::vector<std::string> errors;
    Type current_function_return_type;
    
    void checkFunction(FunctionDeclarationNode* node);
    void checkStatement(ASTNode* node);
    void checkVariableDeclaration(VariableDeclarationNode* node);
    void checkVariableAssignment(VariableAssignmentNode* node);
    void checkReturn(ReturnNode* node);
    void checkIf(IfStatementNode* node);
    void checkWhile(WhileStatementNode* node);
    void checkFor(ForStatementNode* node);

    Type inferType(ASTNode* node);
    Type inferBinaryOp(BinaryOperationNode* node);
    Type inferFunctionCall(FunctionCallNode* node);

    void error(const std::string& message, int line, int column);
    std::string typeToString(Type type);
};
