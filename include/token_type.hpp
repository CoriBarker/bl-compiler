#pragma once

#define TOKEN_TYPES \
    X(INT) \
    X(INT8) \
    X(INT16) \
    X(INT32) \
    X(INT64) \
    X(UINT) \
    X(UINT8) \
    X(UINT16) \
    X(UINT32) \
    X(UINT64) \
    X(STRING) \
    X(BOOL) \
    X(FUNCTION) \
    X(NUMBER) \
    X(STRING_LITERAL) \
    X(TRUE) \
    X(FALSE) \
    X(IDENTIFIER) \
    X(IF) \
    X(ELSE) \
    X(ELSE_IF) \
    X(WHILE) \
    X(FOR) \
    X(RETURN) \
    X(PLUS) \
    X(MINUS) \
    X(MULTIPLY) \
    X(DIVIDE) \
    X(MOD) \
    X(EQUAL) \
    X(NOT_EQUAL) \
    X(LESS_EQUAL) \
    X(GREATER_EQUAL) \
    X(LESS) \
    X(GREATER) \
    X(OR) \
    X(AND) \
    X(NOT) \
    X(ASSIGN) \
    X(ARROW) \
    X(SEMICOLON) \
    X(COMMA) \
    X(LEFT_BRACKET) \
    X(RIGHT_BRACKET) \
    X(LEFT_BRACE) \
    X(RIGHT_BRACE) \
    X(LEFT_SQUARE) \
    X(RIGHT_SQUARE) \
    X(IMPORT) \
    X(END_OF_FILE) \
    X(INVALID)

enum class TokenType {
#define X(name) name,
    TOKEN_TYPES
#undef X
};
