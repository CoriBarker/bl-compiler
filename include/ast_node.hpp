#pragma once

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
    std::vector<FunctionDeclarationNode*> function_declarations;

    ProgramNode(std::vector<FunctionDeclarationNode*> function_declarations, int line, int col) : function_declarations(function_declarations), line(line), col(col) { }
};

class FunctionDeclarationNode : public ASTNode {
public:
    std::string identifier;
    std::vector<ParameterNode*> parameters;
    Type return_type;
    std::vector<ASTNode*> body;

    FunctionDeclarationNode(std::string identifier, std::vector<ParameterNode*> parameters, Type return_type, std::vector<ASTNode*> body, int line, int col) : identifier(identifier), parameters(parameters), return_type(return_type), body(body), line(line), col(col) { }
};

class FunctionCallNode : public ASTNode {
public:
    std::string identifier;
    std::vectot<ArgumentNode*> arguments;

    FunctionCallNode(std::string identifier, std::vector<ArgumentNode*> arguments, int line, int column) : identifier(identifier), arguments(arguments), line(line), column(column) { }
};

class ParameterNode : public ASTNode {
public:
    Type type;
    std::string identifier;

    ParameterNode(Type type, std::string identifier, int line, int column) : type(type), identifier(identifier), line(line), column(column) { }
};

class ArgumentNode : public ASTNode {
public:
    ASTNode* expression;
    VariableNode* variable;

    ArgumentNode(ASTNode* expression, VariableNode* variable, int line int column) : expression(expression), variable(variable), line(line), column(column) { }
};

class VariableDeclarationNode : public ASTNode {
public:
    Type type;
    VariableNode* variable;
    ASTNode* value;

    VariableDeclarationNode(Type type, VariableNode* variable, ASTNode* value, int line, int column) : type(type), variable(variable), value(value), line(line), column(column) { }
};

class VariableAssignmentNode : public ASTNode {
public:
    VariableNode* variable;
    ASTNode* value;

    VariableAssignmentNode(VariableNode* variable, ASTNode* value, int line, int column) : variable(variable), value(value), line(line), column(column) { }
};

class ReturnNode : public ASTNode {
public:
    ASTNode* expression;

    ReturnNode(ASTNode* expression, int line, int column) : expression(expression), line(line), column(column) { }
};


class BinaryOperationNode : public ASTNode {
public:
    char operation;
    ASTNode* left;
    ASTNode* right;

    BinaryOperationNode(std::string operation, ASTNode* left, ASTNode* right, int line, int column) : operation(operation), left(left), right(right), line(line), column(column) { }
};

class UnaryOperationNode : public ASTNode {
public:
    char operation;
    ASTNode* operand;
    
    UnaryOperationNode(char operation, ASTNode* operand, int line, int column) : operation(operation), operand(operand), line(line), column(column) { }
};

class StringLiteralNode : public ASTNode {
public:
    std::string value;

    StringLiteralNode(std::string value, int line, int column) : value(value), line(line), column(column) { }
};

class NumberLiteralNode : public ASTNode {
public:
    int value;

    NumberLiteralNode(int value, int line, int column) : value(value), line(line), column(column) { }
};

class ComparisonNode : public ASTNode {
public:
    std::string operation;
    ASTNode* left;
    ASTNode* right;

    ComparisonNode(std::string operation, ASTNode* left, ASTNode* right, int line, int col) : operation(operation), left(left), right(right), line(line), column(column) { }
};

class IfStatementNode : public ASTNode {
public:
    ASTNode* expression;
    ASTNode* body;
    ASTNode* else_statement;
    ASTNode* elif_statement;

    IfStatementNode(ASTNode* expression, ASTNode* body, ASTNode* else_statement, ASTNode* elif_statement, int line, int column) : expression(expression), body(body), else_statement(else_statement), elif_statement(elif_statement), line(line), column(column) { }
};

class WhileStatementNode : public ASTNode {
public:
    ASTNode* expression;
    ASTNode* body;

    WhileStatementNode(ASTNode* expression, ASTNode* body, int line, int column) : expression(expresssion), body(body), line(line), column(column) { }
};

class VariableNode : public ASTNode {
public:
    std::string identifier;

    VariableNode(std::string identifier, int line, int column) : identifier(identifier), line(line), column(column) { }
};

