#include "lexer/lexer.h"

Lexer::Lexer(const std::string& src) : src(src), position(0), line(1), column(1) {}

std::vector<Token> Lexer::tokenise() {
    while (position < src.size()) {
        if (std::isdigit(src[position])) {
            std::string digit;
            int original_column = column;

            while (std::isdigit(src[position])) {
                digit.push_back(src[position]);
                advance();
            }

            tokens.push_back(Token(TokenType::NUMBER, digit, line, original_column));

        } else if (std::isalpha(src[position])) {
            if (src.substr(position, 3) == "int") {
                tokens.push_back(Token(TokenType::INT, "int", line, column));
                advance();
                advance();
                advance();

            } else {
                std::string identifier;
                int original_column = column;

                while (std::isalnum(src[position])) {
                    identifier.push_back(src[position]);
                    advance();
                }

                tokens.push_back(Token(TokenType::IDENTIFIER, identifier, line, original_column));
            }

        } else if (src[position] == '+') {
            tokens.push_back(Token(TokenType::PLUS, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '-') {
            tokens.push_back(Token(TokenType::MINUS, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '*') {
            tokens.push_back(Token(TokenType::MULTIPLY, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '/') {
            tokens.push_back(Token(TokenType::DIVIDE, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '=') {
            tokens.push_back(Token(TokenType::ASSIGN, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == ';') {
            tokens.push_back(Token(TokenType::SEMICOLON, std::string(1, src[position]), line, column));
            advance();

        } else {
            tokens.push_back(Token(TokenType::INVALID, std::string(1, src[position]), line, column));
            advance();

        }
        skipComment();
        skipWhitespace();

    }
    tokens.push_back(Token(TokenType::END_OF_FILE, std::string(), line, column));
    return tokens;
}

char Lexer::advance() {
    column++;
    return src[position++];
}

void Lexer::skipWhitespace() {
    while (src[position] == ' ' || src[position] == '\n') {
        if (src[position] == ' ') {
            position++;
            column++;

        } else if (src[position] == '\n') {
            position++;
            column = 1;
            line++;
        }
    }
}

void Lexer::skipComment() {
    if (src[position] == '/' && src[++position] == '/') {
        while (src[position] != '\n') {
            position++;
            column++;
        }
        position++;
        line += 1;
        column = 1;
    }
}
