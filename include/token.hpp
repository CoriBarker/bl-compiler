#pragma once

#include "token_type.hpp"

#include <string>

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t, const std::string& val, int line, int col) : type{t}, value{val}, line{line}, column{col} {};
};
