#pragma once

enum class TokenType {
    INT,
    STRING,
    BOOL,
    FUNCTION,

    NUMBER,
    TRUE,
    FALSE,
    IDENTIFIER,

    IF,
    ELSE,
    ELSE_IF,
    WHILE,
    FOR,
    RETURN,

    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MOD,
    EQUAL,
    NOT_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    LESS,
    GREATER,

    OR,
    AND,
    NOT,

    ASSIGN,
    ARROW,

    SEMICOLON,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,

    END_OF_FILE,
    INVALID,
};
