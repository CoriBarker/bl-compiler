#ifndef PARSER_H
#define PARSER_H

#include "lexer/token.hpp"
#include "parser/ast_node.hpp"

#include <vector>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse(); 

private:
    std::vector<Token> tokens_;
    int position_;

    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseAddSub();
    std::unique_ptr<ASTNode> parseMulDiv();
    std::unique_ptr<ASTNode> parsePrimary();
    std::unique_ptr<ASTNode> parseDeclaration();
    std::unique_ptr<ASTNode> parseAssignment();
    Token advance();
    Token peek();
    bool isAtEnd();
    void error(std::string s);
};

#endif
