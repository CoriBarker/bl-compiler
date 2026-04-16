#include "lexer.hpp"

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
                for (int i=0; i<3; i++) {
                    advance();
                }

            } else if (src.substr(position, 8) == "function") {
                tokens.push_back(Token(TokenType::FUNCTION, "function", line, column));
                for (int i=0; i<8; i++) {
                    advance();
                }

            } else if (src.substr(position, 6) == "return") {
                tokens.push_back(Token(TokenType::RETURN, "return", line, column));
                for (int i=0; i<6; i++) {
                    advance();
                }

            } else if (src.substr(position, 2) == "if") {
              tokens.push_back(Token(TokenType::IF, "if", line, column));
                for (int i=0; i<2; i++) {
                    advance();
                }

            } else if (src.substr(position, 5) == "while") {
              tokens.push_back(Token(TokenType::WHILE, "while", line, column));
                for (int i=0; i<5; i++) {
                    advance();
                }

            } else if (src.substr(position, 3) == "for") {
              tokens.push_back(Token(TokenType::FOR, "for", line, column));
                for (int i=0; i<3; i++) {
                    advance();
                }

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
            if (src[position + 1] == '>') {
                tokens.push_back(Token(TokenType::ARROW, "->", line, column));
                advance();
                advance();

            } else {
                tokens.push_back(Token(TokenType::MINUS, std::string(1, src[position]), line, column));
                advance();
            }

        } else if (src[position] == '*') {
            tokens.push_back(Token(TokenType::MULTIPLY, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '/') {
            tokens.push_back(Token(TokenType::DIVIDE, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '=') {
            if (src[position + 1] == '=') {
                tokens.push_back(Token(TokenType::EQUAL, "==", line, column));
                advance();
                advance();
            } else {
                tokens.push_back(Token(TokenType::ASSIGN, std::string(1, src[position]), line, column));
                advance();
            }

        } else if (src[position] == ';') {
            tokens.push_back(Token(TokenType::SEMICOLON, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '<') {
            if (src[position + 1] == '=') {
                tokens.push_back(Token(TokenType::LESS_EQUAL, "<=", line, column));
                advance();
                advance();
            } else {
                tokens.push_back(Token(TokenType::LESS, std::string(1, src[position]), line, column));
                advance();
            }

        } else if (src[position] == '>') {
            if (src[position + 1] == '=') {
                tokens.push_back(Token(TokenType::GREATER_EQUAL, ">=", line, column));
                advance();
                advance();
            } else {
                tokens.push_back(Token(TokenType::GREATER, std::string(1, src[position]), line, column));
                advance();
            }

        } else if (src[position] == '(') {
            tokens.push_back(Token(TokenType::LEFT_BRACKET, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == ')') {
            tokens.push_back(Token(TokenType::RIGHT_BRACKET, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '{') {
            tokens.push_back(Token(TokenType::LEFT_BRACE, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '}') {
            tokens.push_back(Token(TokenType::RIGHT_BRACE, std::string(1, src[position]), line, column));
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
