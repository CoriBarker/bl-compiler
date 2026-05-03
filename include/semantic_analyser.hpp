#pragma once

#include "symbol_table.hpp"
#include "ast_node.hpp"
#include "type.hpp"

#include <unordered_set>
#include <string>
#include <vector>

struct Context {
    std::string name;
    Type return_type;
    int loop_depth = 0;
    std::unordered_set<std::string> initialised_vars;
};

class SemanticAnalyser {
public:
    explicit SemanticAnalyser(SymbolTable& table);

    void analyse(ProgramNode* node);
    std::vector<std::string> getErrors();

private:
    SymbolTable& table;
    std::vector<std::string> errors;

    void analyseFunction(FunctionDeclarationNode* node, Context& ctx);
    void analyseStatement(ASTNode* node, Context& ctx);
    void analyseVariableDeclaration(VariableDeclarationNode* node, Context& ctx);
    void analyseVariableAssignment(VariableAssignmentNode* node, Context& ctx);
    void analyseReturn(ReturnNode* node, Context& ctx);
    void analyseIf(IfStatementNode* node, Context& ctx);
    void analyseWhile(WhileStatementNode* node, Context& ctx);
    void analyseFor(ForStatementNode* node, Context& ctx);
    void analyseFunctionCall(FunctionCallNode* node, Context& ctx);
    void analyseExpression(ASTNode* node, Context& ctx);
    void analyseIdentifier(IdentifierNode* node, Context& ctx);

    bool allPathsReturn(std::vector<std::unique_ptr<ASTNode>>& nodes);

    void error(const std::string& message, int line, int column);
};
