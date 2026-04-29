#include "lexer.hpp"

Lexer::Lexer(const std::string& src) : src(src), position(0), line(1), column(1) {}

std::vector<Token> Lexer::tokenise() {
    while (position < src.size()) {
        skipComment();
        skipWhitespace();

        if (position >= src.size()) { break; }

        if (std::isdigit(src[position])) {
            std::string digit;
            int original_column = column;

            while (std::isdigit(src[position])) {
                digit.push_back(src[position]);
                advance();
            }

            tokens.push_back(Token(TokenType::NUMBER, digit, line, original_column));

        } else if (std::isalpha(src[position])) {
            if (src.substr(position, 3) == "int" && !std::isalnum(src[position+3]) && src[position+3] != '_') {
                tokens.push_back(Token(TokenType::INT, "int", line, column));
                for (int i=0; i<3; i++) {
                    advance();
                }

            } else if (src.substr(position, 6) == "string" && !std::isalnum(src[position+6]) && src[position+6] != '_') {
                tokens.push_back(Token(TokenType::STRING, "string", line, column));
                for (int i=0; i<6; i++) {
                    advance();
                }

            } else if (src.substr(position, 8) == "function" && !std::isalnum(src[position+8]) && src[position+8] != '_') {
                tokens.push_back(Token(TokenType::FUNCTION, "function", line, column));
                for (int i=0; i<8; i++) {
                    advance();
                }

            } else if (src.substr(position, 6) == "return" && !std::isalnum(src[position+6]) && src[position+6] != '_') {
                tokens.push_back(Token(TokenType::RETURN, "return", line, column));
                for (int i=0; i<6; i++) {
                    advance();
                }

            } else if (src.substr(position, 2) == "if" && !std::isalnum(src[position+2]) && src[position+2] != '_') {
              tokens.push_back(Token(TokenType::IF, "if", line, column));
                for (int i=0; i<2; i++) {
                    advance();
                }

            } else if (src.substr(position, 5) == "while" && !std::isalnum(src[position+5]) && src[position+5] != '_') {
              tokens.push_back(Token(TokenType::WHILE, "while", line, column));
                for (int i=0; i<5; i++) {
                    advance();
                }

            } else if (src.substr(position, 3) == "for" && !std::isalnum(src[position+3]) && src[position+3] != '_') {
              tokens.push_back(Token(TokenType::FOR, "for", line, column));
                for (int i=0; i<3; i++) {
                    advance();
                }

            } else if (src.substr(position, 7) == "else if" && !std::isalnum(src[position+7]) && src[position+7] != '_') {
                tokens.push_back(Token(TokenType::ELSE_IF, "else if", line, column));
                for (int i = 0; i < 7; i++) advance();

            } else if (src.substr(position, 4) == "else" && !std::isalnum(src[position+4]) && src[position+4] != '_') {
                tokens.push_back(Token(TokenType::ELSE, "else", line, column));
                for (int i = 0; i < 4; i++) advance();

            } else if (src.substr(position, 4) == "true" && !std::isalnum(src[position+4]) && src[position+4] != '_') {
                tokens.push_back(Token(TokenType::TRUE, "true", line, column));
                for (int i = 0; i < 4; i++) advance();

            } else if (src.substr(position, 5) == "false" && !std::isalnum(src[position+5]) && src[position+5] != '_') {
                tokens.push_back(Token(TokenType::FALSE, "false", line, column));
                for (int i = 0; i < 5; i++) advance();

            } else {
                std::string identifier;
                int original_column = column;

                while (std::isalnum(src[position]) || src[position] == '_') {
                    identifier.push_back(src[position]);
                    advance();
                }

                tokens.push_back(Token(TokenType::IDENTIFIER, identifier, line, original_column));
            }

        } else if (src[position] == '+') {
            tokens.push_back(Token(TokenType::PLUS, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '-') {
            if (position+1 < src.size() && src[position + 1] == '>') {
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
          if (position+1 < src.size() && src[position + 1] == '=') {
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
            if (position+1 < src.size() && src[position + 1] == '=') {
                tokens.push_back(Token(TokenType::LESS_EQUAL, "<=", line, column));
                advance();
                advance();
            } else {
                tokens.push_back(Token(TokenType::LESS, std::string(1, src[position]), line, column));
                advance();
            }

        } else if (src[position] == '>') {
            if (position+1 < src.size() && src[position + 1] == '=') {
                tokens.push_back(Token(TokenType::GREATER_EQUAL, ">=", line, column));
                advance();
                advance();
            } else {
                tokens.push_back(Token(TokenType::GREATER, std::string(1, src[position]), line, column));
                advance();
            }

        } else if (src[position] == '%') {
            tokens.push_back(Token(TokenType::MOD, std::string(1, src[position]), line, column));
            advance();

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

        } else if (src[position] == ',') {
            tokens.push_back(Token(TokenType::COMMA, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == '"') {
            int l = line;
            int c = column;
            advance();

            std::string s;

            while (position < src.size() && src[position] != '"') {
                if (src[position] == '\\' && position+1 < src.size() && src[position+1] == '"') {
                    s += '"';
                    advance();
                    advance();
                }

                else {
                    s += src[position];
                    advance();
                }
            }
            advance();

            tokens.push_back(Token(TokenType::STRING_LITERAL, s, l, c));

        } else if (src[position] == '[') {
            tokens.push_back(Token(TokenType::LEFT_SQUARE, std::string(1, src[position]), line, column));
            advance();

        } else if (src[position] == ']') {
            tokens.push_back(Token(TokenType::RIGHT_SQUARE, std::string(1, src[position]), line, column));
            advance();

        } else if (position+1 < src.size() && src[position] == '&' && src[position+1] == '&') {
            tokens.push_back(Token(TokenType::AND, "&&", line, column));
            advance();
            advance();
            
        } else if (position+1 < src.size() && src[position] == '|' && src[position+1] == '|') {
            tokens.push_back(Token(TokenType::OR, "||", line, column));
            advance();
            advance();

        } else if (src[position] == '!') {
            if (position+1 < src.size() && src[position+1] == '=') {
                tokens.push_back(Token(TokenType::NOT_EQUAL, "!=", line, column));
                advance();
                advance();

            } else {

            tokens.push_back(Token(TokenType::NOT, std::string(1, src[position]), line, column));
            advance();
            }

        } else {
            tokens.push_back(Token(TokenType::INVALID, std::string(1, src[position]), line, column));
            advance();

        }
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
        if (src[position] == '\t') {
            position++;
            column++;
        }
        
        else if (src[position] == ' ') {
            position++;
            column++;

        }

        else if (src[position] == '\n') {
            position++;
            column = 1;
            line++;
        }
    }
}

void Lexer::skipComment() {
    if (position+1 < src.size() && src[position] == '/' && src[position+1] == '/') {
        position += 2;
        column +=2;

        while (position < src.size() && src[position] != '\n') {
            position++;
            column++;
        }

        if (position < src.size()) {
            position++;
            line++;
            column = 1;
        }
    }
}
