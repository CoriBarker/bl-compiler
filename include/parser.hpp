#pragma once

#include "token.hpp"
#include "ast_node.hpp"
#include "type.hpp"

#include <vector>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ProgramNode> parseProgram(); 
    std::vector<std::string> getErrors();

private:
    std::vector<Token> tokens;
    int position;
    std::vector<std::string> errors;

    std::unique_ptr<FunctionDeclarationNode> parseFunctionDeclaration();
    std::vector<std::unique_ptr<ParameterNode>> parseParameterList();
    std::unique_ptr<ParameterNode> parseParameter();
    Type parseType();
    std::vector<std::unique_ptr<ASTNode>> parseBlock();
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<VariableDeclarationNode> parseVariableDeclaration();
    std::unique_ptr<ArrayDeclarationNode> parseArrayDeclaration();
    std::unique_ptr<VariableAssignmentNode> parseVariableAssignment();
    std::unique_ptr<ArrayAssignmentNode> parseArrayAssignment();
    std::unique_ptr<ArrayAccessNode> parseArrayAccess();
    std::unique_ptr<ReturnNode> parseReturnStatement();
    std::unique_ptr<ASTNode> parseExpressionStatement();
    std::unique_ptr<IfStatementNode> parseIfStatement();
    std::unique_ptr<ElseStatementNode> parseElseStatement();
    std::unique_ptr<WhileStatementNode> parseWhileStatement();
    std::unique_ptr<ForStatementNode> parseForStatement();
    std::unique_ptr<ForInitNode> parseForInit();
    std::unique_ptr<VariableAssignmentNode> parseForUpdate();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseLogicalOr();
    std::unique_ptr<ASTNode> parseLogicalAnd();
    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parsePrimary();
    std::unique_ptr<ArrayLiteralNode> parseArrayLiteral();
    std::unique_ptr<FunctionCallNode> parseFunctionCall();
    std::vector<std::unique_ptr<ASTNode>> parseArgumentList();
    std::unique_ptr<ImportNode> parseImport();
  
    Token advance();
    Token peek();
    void error(const std::string& message, int line, int column);
    void error(const std::string& message);
    void expect(TokenType expected, const std::string& context);
    std::string tokenTypeToString(TokenType type);
    bool isExpression();
};
