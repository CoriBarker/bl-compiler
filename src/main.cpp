#include <iostream>
#include <sstream>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"

// ---------------- TOKEN DEBUG ----------------
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        // Types
        case TokenType::INT:            return "INT";
        case TokenType::STRING:         return "STRING";
        case TokenType::BOOL:           return "BOOL";

        // Keywords
        case TokenType::FUNCTION:       return "FUNCTION";
        case TokenType::IF:             return "IF";
        case TokenType::ELSE:           return "ELSE";
        case TokenType::ELSE_IF:        return "ELSE_IF";
        case TokenType::WHILE:          return "WHILE";
        case TokenType::FOR:            return "FOR";
        case TokenType::RETURN:         return "RETURN";

        // Literals
        case TokenType::NUMBER:         return "NUMBER";
        case TokenType::TRUE:           return "TRUE";
        case TokenType::FALSE:          return "FALSE";
        case TokenType::IDENTIFIER:     return "IDENTIFIER";

        // Arithmetic operators
        case TokenType::PLUS:           return "PLUS";
        case TokenType::MINUS:          return "MINUS";
        case TokenType::MULTIPLY:       return "MULTIPLY";
        case TokenType::DIVIDE:         return "DIVIDE";
        case TokenType::MOD:            return "MOD";

        // Comparison operators
        case TokenType::EQUAL:          return "EQUAL";
        case TokenType::NOT_EQUAL:      return "NOT_EQUAL";
        case TokenType::LESS:           return "LESS";
        case TokenType::LESS_EQUAL:     return "LESS_EQUAL";
        case TokenType::GREATER:        return "GREATER";
        case TokenType::GREATER_EQUAL:  return "GREATER_EQUAL";

        // Logical operators
        case TokenType::OR:             return "OR";
        case TokenType::AND:            return "AND";
        case TokenType::NOT:            return "NOT";

        // Other operators
        case TokenType::ASSIGN:         return "ASSIGN";
        case TokenType::ARROW:          return "ARROW";

        // Punctuation
        case TokenType::SEMICOLON:      return "SEMICOLON";
        case TokenType::COMMA:          return "COMMA";
        case TokenType::LEFT_BRACKET:   return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET:  return "RIGHT_BRACKET";
        case TokenType::LEFT_BRACE:     return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:    return "RIGHT_BRACE";

        // Special
        case TokenType::END_OF_FILE:    return "EOF";
        case TokenType::INVALID:        return "INVALID";
    }
}

std::string toString(Type type) {
    switch (type) {
        case Type::INT: return "int";
        case Type::CHAR: return "char";
        case Type::BOOL: return "bool";
        case Type::STRING: return "string";
        case Type::FLOAT: return "float";
        case Type::DOUBLE: return "double";
        case Type::VOID: return "void";
        default: return "unknown";
    }
}

// ---------------- AST PRINT ----------------
void printAST(ASTNode* node, int indent) {
    std::string indentation(indent * 2, ' ');

    if (!node) {
        std::cout << indentation << "null\n";
        return;
    }

    // Program
    if (auto* program = dynamic_cast<ProgramNode*>(node)) {
        std::cout << indentation << "Program\n";
        for (const auto& func : program->function_declarations) {
            printAST(func.get(), indent + 1);
        }
    }

    // Function Declaration
    else if (auto* func = dynamic_cast<FunctionDeclarationNode*>(node)) {
        std::cout << indentation << "Function: " << func->identifier << "\n";
        std::cout << indentation << "  Return: " << toString(func->return_type) << "\n";

        if (!func->parameters.empty()) {
            std::cout << indentation << "  Params:\n";
            for (const auto& param : func->parameters) {
                std::cout << indentation << "    "
                          << param->identifier
                          << " : " << toString(param->type) << "\n";
            }
        }

        std::cout << indentation << "  Body:\n";
        for (const auto& stmt : func->body) {
            printAST(stmt.get(), indent + 2);
        }
    }

    // Function Call
    else if (auto* call = dynamic_cast<FunctionCallNode*>(node)) {
        std::cout << indentation << "FunctionCall: " << call->identifier << "\n";
        for (const auto& arg : call->arguments) {
            printAST(arg.get(), indent + 1);
        }
    }

    // Variable declaration
    else if (auto* var = dynamic_cast<VariableDeclarationNode*>(node)) {
        std::cout << indentation << "VarDecl: " << var->identifier
                  << " : " << toString(var->type) << "\n";

        if (var->value) {
            printAST(var->value.get(), indent + 1);
        }
    }

    // Assignment
    else if (auto* assign = dynamic_cast<VariableAssignmentNode*>(node)) {
        std::cout << indentation << "Assign: " << assign->identifier << "\n";
        printAST(assign->value.get(), indent + 1);
    }

    // Return
    else if (auto* ret = dynamic_cast<ReturnNode*>(node)) {
        std::cout << indentation << "Return\n";
        if (ret->expression) {
            printAST(ret->expression.get(), indent + 1);
        }
    }

    // If
    else if (auto* ifs = dynamic_cast<IfStatementNode*>(node)) {
        std::cout << indentation << "If\n";
        printAST(ifs->expression.get(), indent + 1);

        std::cout << indentation << "Then:\n";
        for (const auto& stmt : ifs->body) {
            printAST(stmt.get(), indent + 2);
        }

        if (ifs->else_statement) {
            std::cout << indentation << "Else:\n";
            for (const auto& stmt : ifs->else_statement->body) {
                printAST(stmt.get(), indent + 2);
            }
        }
    }

    // For
    else if (auto* f = dynamic_cast<ForStatementNode*>(node)) {
    std::cout << indentation << "For\n";

    if (f->init) {
        std::cout << indentation << "  Init:\n";
        if (!f->init->identifier.empty()) {
            std::cout << indentation << "    " << f->init->identifier;
            if (f->init->type == Type::INT) std::cout << " : int";  // extend for other types
            std::cout << "\n";
        }
        if (f->init->expression) {
            printAST(f->init->expression.get(), indent + 2);
        }
    }
    
    if (f->condition) {
        std::cout << indentation << "  Condition:\n";
        printAST(f->condition.get(), indent + 2);
    }

    if (f->update) {
        std::cout << indentation << "  Update:\n";
        printAST(f->update.get(), indent + 2);
    }

    std::cout << indentation << "  Body:\n";
    for (const auto& stmt : f->body) {
        printAST(stmt.get(), indent + 2);
    }
}

    // While
    else if (auto* wh = dynamic_cast<WhileStatementNode*>(node)) {
        std::cout << indentation << "While\n";
        printAST(wh->expression.get(), indent + 1);

        std::cout << indentation << "Body:\n";
        for (const auto& stmt : wh->body) {
            printAST(stmt.get(), indent + 2);
        }
    }

    // Binary operation
    else if (auto* bin = dynamic_cast<BinaryOperationNode*>(node)) {
        std::cout << indentation << "BinaryOp: " << bin->operation << "\n";
        printAST(bin->left.get(), indent + 1);
        printAST(bin->right.get(), indent + 1);
    }

    // Unary operation
    else if (auto* un = dynamic_cast<UnaryOperationNode*>(node)) {
        std::cout << indentation << "UnaryOp: " << un->operation << "\n";
        printAST(un->operand.get(), indent + 1);
    }

    // Identifier
    else if (auto* id = dynamic_cast<IdentifierNode*>(node)) {
        std::cout << indentation << "Identifier: " << id->value << "\n";
    }

    // Number
    else if (auto* num = dynamic_cast<NumberLiteralNode*>(node)) {
        std::cout << indentation << "Number: " << num->value << "\n";
    }

    // Boolean
    else if (auto* b = dynamic_cast<BooleanLiteralNode*>(node)) {
        std::cout << indentation << "Boolean: " << b->value << "\n";
    }

    else {
        std::cout << indentation << "Unknown node\n";
    }
}

// ---------------- MAIN ----------------
int main() {
    std::ifstream file("test.bl");
    std::ostringstream buffer;
    std::string line;

    if (!file) {
        std::cerr << "Error opening file!\n";
        return 1;
    }

    while (std::getline(file, line)) {
        buffer << line << '\n';
    }

    std::string file_contents = buffer.str();

    std::cout << "File contents:\n" << file_contents << "\n";

    // -------- LEXING --------
    Lexer lexer(file_contents);
    auto tokens = lexer.tokenise();

    for (const auto& token : tokens) {
        if (token.type == TokenType::END_OF_FILE) {
            std::cout << "EOF\n";
        } else {
            std::cout << tokenTypeToString(token.type)
                      << " '" << token.value << "'"
                      << " at line " << token.line
                      << ", column " << token.column
                      << "\n";
        }
    }

    // -------- PARSING --------
    std::cout << "\n=== PARSING ===\n";

    Parser parser(tokens);

    auto ast = parser.parseProgram();

    std::cout << "Parse successful!\n";
    std::cout << "Functions: " << ast->function_declarations.size() << "\n";

    // -------- AST --------
    std::cout << "\n=== AST ===\n";
    printAST(ast.get(), 0);

    return 0;
}
