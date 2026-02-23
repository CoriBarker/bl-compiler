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
};

class ProgramNode : ASTNode {
public:
    std::vector<ASTNode> function_declarations;

    ProgramNode(std::vector<ASTNode> function_declarations, int line, int col) : function_declarations(function_declarations), line(line), col(col) { }
};

class FunctionDeclNode : ASTNode {
    Type type;
    std::vector<ParameterNode> parameters;
    std::vector<ASTNode> body;

    FunctionDeclNode(Type type, std::vector<ASTNode> parameters, std::vector<ASTNode> body, int line, int col) : type(type), parameters(parameters), body(body), line(line), col(col) { }
};

class ParameterNode : ASTNode {
    std::string name;
    Type type;

    ParameterNode(std::string name, std::string type, int line, int col) : name(name), type(type), line(line) col(col) { }
};

class VarDeclNode : ASTNode {
    std::string name;
    Type type;

    VarDeclNode(std::string name, std::string type, int line, int col) : name(name), type(type), line(line), col(col) { }
};

class AssignmentNode : ASTNode {
    std::string var_name;
    Type var_type;
    ASTNode* value;
    
    AssignmentNode(std::string var_name, Type var_type, ASTNode* value) : var_name(var_name), var_type(var_type), value(value), line(line), col(col) { }
};

class IfStatementNode : ASTNode {
    ASTNode* expression;
    ASTNode* body;
    ASTNode* else_statement = nullptr;
    ASTNode* elif_statement = nullptr;

    IfStatementNode(ASTNode* expression, ASTNode* body, ASTNode* else_statement, ASTNode* elif_statement, int line, int col) : expression(expression), body(body), else_statement(else_statement), elif_statement(elif_statement), line(line), col(col) { }
};

class WhileStatementNode : ASTNode {
    ASTNode* expression;
    ASTNode* body;

    WhileStatementNode(ASTNode* expression, ASTNode* body, int line, int col) : expression(expression), body(body), line(line), col(col) { }
};

class ForStatementNode : ASTNode {
    StatementNode* init;
    ASTNode* 
};
