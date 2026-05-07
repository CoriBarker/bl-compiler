#pragma once

#include "symbol_table.hpp"
#include "ast_node.hpp"

#include <string>
#include <vector>

class TypeChecker {
public:
    TypeChecker(SymbolTable& table);

    void check(ProgramNode* node);
    std::vector<std::string> getErrors();

private:
    SymbolTable& table;
    std::vector<std::string> errors;
    Type current_function_return_type;
    std::string current_function_name;
    
    void checkFunction(FunctionDeclarationNode* node);
    void checkStatement(ASTNode* node);
    void checkVariableDeclaration(VariableDeclarationNode* node);
    void checkArrayDeclaration(ArrayDeclarationNode* node);
    void checkVariableAssignment(VariableAssignmentNode* node);
    void checkArrayAssignment(ArrayAssignmentNode* node);
    void checkReturn(ReturnNode* node);
    void checkIf(IfStatementNode* node);
    void checkWhile(WhileStatementNode* node);
    void checkFor(ForStatementNode* node);

    Type inferType(ASTNode* node);
    Type inferIdentifier(IdentifierNode* node);
    Type inferBinaryOp(BinaryOperationNode* node);
    Type inferUnaryOp(UnaryOperationNode* node);
    Type inferFunctionCall(FunctionCallNode* node);
    Type inferArrayAccess(ArrayAccessNode* node);
    Type inferArrayLiteral(ArrayLiteralNode* node);

    void error(const std::string& message, int line, int column);
    std::string typeToString(Type type);
    Symbol* lookupVariable(const std::string& name);
    bool isInt(Type t);
    int getBitWidth(Type t);
    bool isSigned(Type t);
    bool isAssignable(Type target, ASTNode* node);
};
