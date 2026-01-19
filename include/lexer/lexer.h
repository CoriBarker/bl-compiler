#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <vector>
#include <string>

class Lexer {
public:
    explicit Lexer (const std::string& src);

    std::vector<Token> tokenise();

private:
    int line;
    int column;
    int position;
    std::string src;
    std::vector<Token> tokens;

    char advance();
    void skipWhitespace();
    void skipComment();
};

#endif
