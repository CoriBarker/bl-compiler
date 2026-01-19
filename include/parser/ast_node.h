#ifndef AST_NODE_H
#define AST_NODE_H

#include "parser/node_type.h"

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    int line;
    int column;

    virtual ~ASTNode() = default;
};

class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> declarations;
};

class Declaration : public ASTNode {
public:
    std::string type;
    std::string var_name;
    std::unique_ptr<ASTNode> value; 

    Declaration(std::string type, std::string var_name, std::unique_ptr<ASTNode> value = nullptr) : type(type), var_name(var_name), value(std::move(value)) {}
};

class Assignment : public ASTNode {
public:
    std::string variable_name;
    std::unique_ptr<ASTNode> value;

    Assignment(std::string var, std::unique_ptr<ASTNode> val) : variable_name(var), value(std::move(val)) {}
};

class NumberLiteral : public ASTNode {
public:
    double value;

    NumberLiteral(double val) : value(val) {}
};

class StringLiteral : public ASTNode {
public:
    std::string value;
    
    StringLiteral(std::string val) : value(val) {}
};

class Identifier : public ASTNode {
public:
    std::string name;
    
    Identifier(std::string name) : name(name) {}
};

class BinaryOp : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string value;
    std::unique_ptr<ASTNode> right;

    BinaryOp(std::unique_ptr<ASTNode> left, std::string value, std::unique_ptr<ASTNode> right) : left(std::move(left)), value(std::move(value)), right(std::move(right)) {}
};

#endif
