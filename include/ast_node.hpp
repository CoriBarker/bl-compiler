#pragma once

#include "node_type.hpp"
#include "type.hpp"

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    int line;
    int col;

    virtual ~ASTNode() = default;
}

class ProgramNode : ASTNode {
public:
    std::vector<FunctionDeclarationNode> function_declarations;

    ProgramNode(std::vector<FunctionDeclarationNode> function_declarations, int line, int col) : function_declarations(function_declarations), line(line), col(col) { }
};

class FunctionDeclarationNode : public ASTNode {
public:
    std::string identifier;
    std::vector<ParameterNode> parameters;
    Type return_type;
    std::vector<StatementNode> body;

    FunctionDeclarationNode(std::string identifier, std::vector<ParameterNode> parameters, Type return_type, std::vector<StatementNode> body, int line, int col) : identifier(identifier), parameters(parameters), return_type(return_type), body(body), line(line), col(col) { }
};

class ParameterNode : public ASTNode {
public:
    Type type;
    std::string identifier;

    ParameterNode(Type type, std::string identifier, int line, int column) : type(type), identifier(identifier), line(line), column(column) { }
};

class ArgumentNode : public ASTNode {
    ASTNode* expression;
    ASTNode* variable;

    ArgumentNode(ASTNode* expression, ASTNode* variable, int line int column) : expression(expression), variable(variable), line(line), column(column) { }
};

class VariableDeclarationNode : public ASTNode {
public:
    Type type;
    std::string identifier;
    ASTNode* value;

    VariableDeclarationNode(Type type, std::string identifier, ASTNode* value, int line, int column) : type(type), identifier(identifier), value(value), line(line), column(column) { }
};

class VariableAssignmentNode : public ASTNode {
public:
    std::string identifier;
    ASTNode* value;

    VariableAssignmentNode(std::string identifier, ASTNode* value, int line, int column) : identifier(identifier), value(value), line(line), column(column) { }
};

class ReturnNode : public ASTNode {
public:
    ASTNode* expression;

    ReturnNode(ASTNode* expression, int line, int column) : expression(expression), line(line), column(column) { }
};


class BinaryOperationNode : public ASTNode {
    char operation;
    ASTNode* left;
    ASTNode* right;

    BinaryOperationNode(std::string operation, ASTNode* left, ASTNode* right, int line, int column) : operation(operation), left(left), right(right), line(line), column(column) { }
};

class UnaryOperationNode : public ASTNode {
    char operation;
    ASTNode* operand;
    
    UnaryOperationNode(char operation, ASTNode* operand, int line, int column) : operation(operation), operand(operand), line(line), column(column) { }
};

class StringLiteralNode : public ASTNode {
    std::string value;

    StringLiteralNode(std::string value, int line, int column) : value(value), line(line), column(column) { }
};

class NumberLiteralNode : public ASTNode {
    int value;

    NumberLiteralNode(int value, int line, int column) : value(value), line(line), column(column) { }
};

class ComparisonNode : public ASTNode {
    std::string operation;
    ASTNode* left;
    ASTNode* right;

    ComparisonNode(std::string operation, ASTNode* left, ASTNode* right, int line, int col) : operation(operation), left(left), right(right), line(line), column(column) { }
};
