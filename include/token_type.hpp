#pragma once

enum class TokenType {
    INT,
    STRING,

    NUMBER,
    IDENTIFIER,

    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,

    ASSIGN,

    SEMICOLON,

    END_OF_FILE,
    INVALID

};
