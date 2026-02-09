#include <iostream>
#include <sstream>
#include <fstream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::PLUS:           return "PLUS";
        case TokenType::MINUS:          return "MINUS";
        case TokenType::MULTIPLY:       return "MULTIPLY";
        case TokenType::DIVIDE:         return "DIVIDE";
        case TokenType::ASSIGN:         return "ASSIGN";
        case TokenType::SEMICOLON:      return "SEMICOLON";
        case TokenType::INVALID:        return "INVALID";
        case TokenType::END_OF_FILE:    return "EOF";
        case TokenType::NUMBER:         return "NUMBER";
        case TokenType::IDENTIFIER:     return "IDENTIFIER";
        case TokenType::INT:            return "INT";
        default:                        return "UNKNOWN";
    }
}

void printAST(ASTNode* node, int indent) {
    std::string indentation(indent * 2, ' ');
    
    if (auto* program = dynamic_cast<Program*>(node)) {
        std::cout << indentation << "Program\n";
        for (const auto& decl : program->declarations) {
            printAST(decl.get(), indent + 1);
        }
    }

    else if (auto* decl = dynamic_cast<Declaration*>(node)) {
        std::cout << indentation << "Declaration\n";
        std::cout << indentation << "  Type: " << decl->type << "\n";
        std::cout << indentation << "  Name: " << decl->var_name << "\n";
        
        if (decl->value) {
            std::cout << indentation << "  Value:\n";
            printAST(decl->value.get(), indent + 2);
        } 
    }

    else if (auto* binop = dynamic_cast<BinaryOp*>(node)) {
        std::cout << indentation << "BinaryOp: " << binop->value << "\n";
        printAST(binop->left.get(), indent + 1);
        printAST(binop->right.get(), indent + 1);
    }

    else if (auto* id = dynamic_cast<Identifier*>(node)) {
        std::cout << indentation << "Identifier: " << id->name << "\n";
    }

    else if (auto* num = dynamic_cast<NumberLiteral*>(node)) {
        std::cout << indentation << "NumberLiteral: " << num->value << "\n";
    }

    else if (auto* str = dynamic_cast<StringLiteral*>(node)) {
        std::cout << indentation << "StringLiteral: " << str->value << "\n";
    }

    else if (auto* assign = dynamic_cast<Assignment*>(node)) {
        std::cout << indentation << "Assignment: " << assign->variable_name << "\n";
        printAST(assign->value.get(), indent + 1);
    }
}

int main() {
    std::ifstream file("test.bl");
    std::ostringstream buffer;
    std::string line;

    if (file) {
        while (std::getline(file, line)) {
            buffer << line << '\n';

        }
        file.close();

    } else {
        std::cerr << "Error opening file!" << std::endl;
        return 1;

    }

    std::string file_contents = buffer.str();

    std::cout << "File contents:\n" << file_contents << std::endl;

    Lexer lexer(file_contents);
    auto tokens = lexer.tokenise();

    for (const auto& token : tokens) {
        if (token.type == TokenType::END_OF_FILE) {
            std::cout << "EOF" << std::endl;

        } else {
            std::cout << tokenTypeToString(token.type)
              << " '" << token.value << "'"
              << " at line " << token.line
              << ", column " << token.column
              << std::endl;

        }
    }
    
    std::cout << "\n=== PARSING ===\n";
    Parser parser(tokens);
    auto ast = parser.parse();
    
    std::cout << "Parse successful!" << std::endl;
    std::cout << "Number of declarations: " << ast->declarations.size() << std::endl;

    std::cout << "\n=== AST ===\n";
    printAST(ast.get(), 0);

}
