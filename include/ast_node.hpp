#pragma once

#include "type.hpp"

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    int line;
    int column;

    virtual ~ASTNode() = default;
};

class ParameterNode : public ASTNode {
public:
    Type type;
    std::string identifier;

    ParameterNode() {}
};

class FunctionDeclarationNode : public ASTNode {
public:
    std::string identifier;
    std::vector<std::unique_ptr<ParameterNode>> parameters;
    Type return_type;
    std::vector<std::unique_ptr<ASTNode>> body;

    FunctionDeclarationNode() {}
};

class ProgramNode : public ASTNode {
public:
  std::vector<std::unique_ptr<FunctionDeclarationNode>> function_declarations;

    ProgramNode() {}
};

class FunctionCallNode : public ASTNode {
public:
    std::string identifier;
    std::vector<std::unique_ptr<ASTNode>> arguments;

    FunctionCallNode() {}
};

class VariableDeclarationNode : public ASTNode {
public:
    Type type;
    std::string identifier;
    std::unique_ptr<ASTNode> value;

    VariableDeclarationNode() {}
};

class VariableAssignmentNode : public ASTNode {
public:
    std::string identifier;
    std::unique_ptr<ASTNode> value;

    VariableAssignmentNode() {}
};

class ReturnNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    ReturnNode() {}
};


class BinaryOperationNode : public ASTNode {
public:
    std::string operation;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

  BinaryOperationNode(std::unique_ptr<ASTNode> left, const std::string& operation, std::unique_ptr<ASTNode> right) : left(std::move(left)), operation(operation), right(std::move(right)) {}
};

class UnaryOperationNode : public ASTNode {
public:
    std::string operation;
    std::unique_ptr<ASTNode> operand;
    
  UnaryOperationNode(const std::string& operation, std::unique_ptr<ASTNode> operand) : operation(operation), operand(std::move(operand)) {}
};

class StringLiteralNode : public ASTNode {
public:
    std::string value;

    StringLiteralNode(std::string value) : value(value) {}
};

class NumberLiteralNode : public ASTNode {
public:
    long long value;

    NumberLiteralNode(long long value) : value(value) {}
};

class BooleanLiteralNode : public ASTNode {
public:
    std::string value;

    BooleanLiteralNode(const std::string& value) : value(value) {}
};

class ElseStatementNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> body;

    ElseStatementNode() {}
};

class ElseIfStatementNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;
    std::vector<std::unique_ptr<ASTNode>> body;

    ElseIfStatementNode() {}
};

class IfStatementNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;
    std::vector<std::unique_ptr<ASTNode>> body;
    std::unique_ptr<ElseStatementNode> else_statement;
    std::unique_ptr<ElseIfStatementNode> else_if_statement;

    IfStatementNode() {}
};

class WhileStatementNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;
    std::vector<std::unique_ptr<ASTNode>> body;

    WhileStatementNode() {}
};

class ForInitNode : public ASTNode {
public:
    Type type = Type::VOID;
    std::string identifier;
    std::unique_ptr<ASTNode> expression;

    ForInitNode() {}
};

class ForStatementNode : public ASTNode {
public:
    std::unique_ptr<ForInitNode> init;
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> update;
    std::vector<std::unique_ptr<ASTNode>> body;

    ForStatementNode() {}
};

class IdentifierNode : public ASTNode {
public:
    std::string value;

    IdentifierNode(const std::string& value) : value(value) {}
};
