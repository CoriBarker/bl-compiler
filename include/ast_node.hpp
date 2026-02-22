#pragma once

#include "node_type.hpp"
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

class ProgramNode : ASTNode {
public:
    std::vector<ASTNode> function_declarations;

    explicit ProgramNode(std::vector<ASTNode> f) : function_declarations(f) { }
};

class FunctionDeclNode : ASTNode {
    Type type;
    std::vector<ParameterNode> parameters;
    std::vector<ASTNode> body;

    FunctionDeclNode(std::string t,std::vector<ASTNode> p, std::vector<ASTNode> b) : type(t), parameters(p), body(b) { }
};

class ParameterNode : ASTNode {
    std::string name;
    std::string type;
};
