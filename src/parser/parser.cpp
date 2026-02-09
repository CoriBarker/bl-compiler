#include <iostream>

#include "parser/parser.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens), position_(0) {}

std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();

    while (peek().type != TokenType::END_OF_FILE) {
        try {
            auto statement = parseStatement();
            program->declarations.push_back(std::move(statement));
            
            if (peek().type == TokenType::SEMICOLON) {
                advance();
            }
        }

        catch (const std::exception& e) {
            std::cerr << " Parser error: " << e.what() << std::endl;
            break;
        }
    } 
    
    return program;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    switch(peek().type) {
        case TokenType::INT:
        case TokenType::STRING:
        {
            return parseDeclaration();
            break;
        }

        default:
        {
            error("Unexpected statement");
            return nullptr;
        }
    }
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseAddSub(); 
}

std::unique_ptr<ASTNode> Parser::parseDeclaration() {
    std::string type;

    switch(peek().type) {
        case TokenType::INT:
        {
            type = "int";
            break;
        }

        case TokenType::STRING:
        {
            type = "string";
            break;
        }

        default:
        {
            error("Unexpected declaration: Invalid type");
            return nullptr;
        }
    }
    
    advance();
    std::string name = peek().value;
    advance();
    if (peek().type == TokenType::ASSIGN) {
        advance();
        auto value = parseExpression();
        return std::make_unique<Declaration>(type, name, std::move(value));
    } 

    else if (peek().type == TokenType::SEMICOLON) {
        return std::make_unique<Declaration>(type, name); 
    }

    else {
        error("Invalid declaration");
        return nullptr;
    }
}

std::unique_ptr<ASTNode> Parser::parseAssignment() {
    std::string name = peek().value;
    advance();
    advance(); // consume =
    auto value = parseExpression();
    return std::make_unique<Assignment>(name, std::move(value));
}

std::unique_ptr<ASTNode> Parser::parseAddSub() {
    auto left = parseMulDiv();

    while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
        std::string op = peek().value;
        advance();
        auto right = parseMulDiv();
        left = std::make_unique<BinaryOp>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseMulDiv() {
    auto left = parsePrimary();
    
    while (peek().type == TokenType::MULTIPLY || peek().type == TokenType::DIVIDE) {
        std::string op = peek().value;
        advance();
        auto right = parsePrimary();
        left = std::make_unique<BinaryOp>(std::move(left), op, std::move(right));
    } 

    return left;
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    switch(peek().type) {
        case TokenType::NUMBER:
        {
            double value = std::stod(peek().value);
            advance();
            return std::make_unique<NumberLiteral>(value);
        }

        case TokenType::IDENTIFIER:
        {
            std::string name = peek().value;
            advance();
            return std::make_unique<Identifier>(name);
        }

        case TokenType::STRING:
        {
            std::string value = peek().value;
            advance();
            return std::make_unique<StringLiteral>(value);
        }

        default:
        {
            error("Unknown keyword");
            return nullptr;
        }
    }
}

Token Parser::advance() {
    return tokens_[position_++];
}

Token Parser::peek() {
    return tokens_[position_];
}

bool Parser::isAtEnd() {
    return position_ >= tokens_.size();
}

void Parser::error(std::string s) {

}
