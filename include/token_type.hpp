#pragma once

enum class TokenType {
    INT,
    STRING,
    FUNCTION,

    NUMBER,
    IDENTIFIER,
    RETURN,

    IF,
    WHILE,
    FOR,

    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    LESS,
    GREATER,

    ASSIGN,
    ARROW,

    SEMICOLON,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,

    END_OF_FILE,
    INVALID,
};
