#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>

#include "lexer.hpp"
#include "parser.hpp"
#include "symbol_table_generator.hpp"
#include "type_checker.hpp"
#include "code_generator.hpp"

// ---------------- HELPERS ----------------
std::string toString(Type type) {
    switch (type) {
        case Type::INT:    return "int";
        case Type::CHAR:   return "char";
        case Type::BOOL:   return "bool";
        case Type::STRING: return "string";
        case Type::FLOAT:  return "float";
        case Type::DOUBLE: return "double";
        case Type::VOID:   return "void";
        default:           return "unknown";
    }
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INT:            return "INT";
        case TokenType::STRING:         return "STRING";
        case TokenType::BOOL:           return "BOOL";
        case TokenType::FUNCTION:       return "FUNCTION";
        case TokenType::IF:             return "IF";
        case TokenType::ELSE:           return "ELSE";
        case TokenType::ELSE_IF:        return "ELSE_IF";
        case TokenType::WHILE:          return "WHILE";
        case TokenType::FOR:            return "FOR";
        case TokenType::RETURN:         return "RETURN";
        case TokenType::NUMBER:         return "NUMBER";
        case TokenType::TRUE:           return "TRUE";
        case TokenType::FALSE:          return "FALSE";
        case TokenType::IDENTIFIER:     return "IDENTIFIER";
        case TokenType::PLUS:           return "PLUS";
        case TokenType::MINUS:          return "MINUS";
        case TokenType::MULTIPLY:       return "MULTIPLY";
        case TokenType::DIVIDE:         return "DIVIDE";
        case TokenType::MOD:            return "MOD";
        case TokenType::EQUAL:          return "EQUAL";
        case TokenType::NOT_EQUAL:      return "NOT_EQUAL";
        case TokenType::LESS:           return "LESS";
        case TokenType::LESS_EQUAL:     return "LESS_EQUAL";
        case TokenType::GREATER:        return "GREATER";
        case TokenType::GREATER_EQUAL:  return "GREATER_EQUAL";
        case TokenType::OR:             return "OR";
        case TokenType::AND:            return "AND";
        case TokenType::NOT:            return "NOT";
        case TokenType::ASSIGN:         return "ASSIGN";
        case TokenType::ARROW:          return "ARROW";
        case TokenType::SEMICOLON:      return "SEMICOLON";
        case TokenType::COMMA:          return "COMMA";
        case TokenType::LEFT_BRACKET:   return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET:  return "RIGHT_BRACKET";
        case TokenType::LEFT_BRACE:     return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:    return "RIGHT_BRACE";
        case TokenType::END_OF_FILE:    return "EOF";
        case TokenType::INVALID:        return "INVALID";
        default:                        return "UNKNOWN";
    }
}

// ---------------- VERBOSE: TOKENS ----------------
void printTokens(const std::vector<Token>& tokens) {
    std::cout << "\n--- Tokens ---\n";
    for (const auto& token : tokens) {
        if (token.type == TokenType::END_OF_FILE) {
            std::cout << "  EOF\n";
        } else {
            std::cout << "  " << tokenTypeToString(token.type)
                      << " '" << token.value << "'"
                      << "  (" << token.line << ":" << token.column << ")\n";
        }
    }
}

// ---------------- VERBOSE: AST ----------------
void printAST(ASTNode* node, int indent) {
    std::string pad(indent * 2, ' ');

    if (!node) return;

    if (auto* program = dynamic_cast<ProgramNode*>(node)) {
        std::cout << pad << "Program\n";
        for (const auto& func : program->function_declarations)
            printAST(func.get(), indent + 1);
    }
    else if (auto* func = dynamic_cast<FunctionDeclarationNode*>(node)) {
        std::cout << pad << "Function: " << func->identifier
                  << " -> " << toString(func->return_type) << "\n";
        if (!func->parameters.empty()) {
            std::cout << pad << "  Parameters:\n";
            for (const auto& param : func->parameters)
                std::cout << pad << "    " << param->identifier
                          << " : " << toString(param->type) << "\n";
        }
        std::cout << pad << "  Body:\n";
        for (const auto& stmt : func->body)
            printAST(stmt.get(), indent + 2);
    }
    else if (auto* call = dynamic_cast<FunctionCallNode*>(node)) {
        std::cout << pad << "Call: " << call->identifier << "()\n";
        for (const auto& arg : call->arguments)
            printAST(arg.get(), indent + 1);
    }
    else if (auto* var = dynamic_cast<VariableDeclarationNode*>(node)) {
        std::cout << pad << "Declare: " << toString(var->type)
                  << " " << var->identifier << "\n";
        if (var->value) printAST(var->value.get(), indent + 1);
    }
    else if (auto* assign = dynamic_cast<VariableAssignmentNode*>(node)) {
        std::cout << pad << "Assign: " << assign->identifier << "\n";
        printAST(assign->value.get(), indent + 1);
    }
    else if (auto* ret = dynamic_cast<ReturnNode*>(node)) {
        std::cout << pad << "Return\n";
        if (ret->expression) printAST(ret->expression.get(), indent + 1);
    }
    else if (auto* ifs = dynamic_cast<IfStatementNode*>(node)) {
        std::cout << pad << "If\n";
        printAST(ifs->expression.get(), indent + 1);
        std::cout << pad << "  Then:\n";
        for (const auto& stmt : ifs->body)
            printAST(stmt.get(), indent + 2);
        if (ifs->else_statement) {
            std::cout << pad << "  Else:\n";
            for (const auto& stmt : ifs->else_statement->body)
                printAST(stmt.get(), indent + 2);
        }
    }
    else if (auto* wh = dynamic_cast<WhileStatementNode*>(node)) {
        std::cout << pad << "While\n";
        std::cout << pad << "  Condition:\n";        // ← add label
        printAST(wh->expression.get(), indent + 2);  // ← indent+2 to be under label
        std::cout << pad << "  Body:\n";
        for (const auto& stmt : wh->body)
            printAST(stmt.get(), indent + 2);
    }
    else if (auto* f = dynamic_cast<ForStatementNode*>(node)) {
        std::cout << pad << "For\n";
        if (f->init) {
            std::cout << pad << "  Init: " << f->init->identifier;
            if (f->init->type != Type::VOID)
                std::cout << " : " << toString(f->init->type);
            std::cout << "\n";
            if (f->init->expression) printAST(f->init->expression.get(), indent + 2);
        }
        if (f->condition) {
            std::cout << pad << "  Condition:\n";
            printAST(f->condition.get(), indent + 2);
        }
        if (f->update) {
            std::cout << pad << "  Update:\n";
            printAST(f->update.get(), indent + 2);
        }
        std::cout << pad << "  Body:\n";
        for (const auto& stmt : f->body)
            printAST(stmt.get(), indent + 2);
    }
    else if (auto* bin = dynamic_cast<BinaryOperationNode*>(node)) {
        std::cout << pad << "BinaryOp: " << bin->operation << "\n";
        printAST(bin->left.get(), indent + 1);
        printAST(bin->right.get(), indent + 1);
    }
    else if (auto* un = dynamic_cast<UnaryOperationNode*>(node)) {
        std::cout << pad << "UnaryOp: " << un->operation << "\n";
        printAST(un->operand.get(), indent + 1);
    }
    else if (auto* id = dynamic_cast<IdentifierNode*>(node)) {
        std::cout << pad << "Identifier: " << id->value << "\n";
    }
    else if (auto* num = dynamic_cast<NumberLiteralNode*>(node)) {
        std::cout << pad << "Number: " << num->value << "\n";
    }
    else if (auto* b = dynamic_cast<BooleanLiteralNode*>(node)) {
        std::cout << pad << "Boolean: " << b->value << "\n";
    }
    else {
        std::cout << pad << "Unknown node\n";
    }
}

// ---------------- VERBOSE: SYMBOL TABLE ----------------
void printSymbolTable(const SymbolTable& table) {
    std::cout << "\n--- Symbol Table ---\n";

    // Print functions first, then variables/parameters
    std::cout << "  Functions:\n";
    for (const auto& pair : table.symbols) {
        const Symbol& sym = pair.second;
        if (sym.kind != SymbolKind::FUNCTION) continue;

        std::cout << "    " << sym.name << " -> " << toString(sym.type);
        if (!sym.parameter_types.empty()) {
            std::cout << "  (";
            for (int i = 0; i < (int)sym.parameter_types.size(); i++) {
                std::cout << toString(sym.parameter_types[i]);
                if (i < (int)sym.parameter_types.size() - 1) std::cout << ", ";
            }
            std::cout << ")";
        } else {
            std::cout << "  ()";
        }
        std::cout << "  [" << sym.line << ":" << sym.column << "]\n";
    }

    std::cout << "  Variables:\n";
    for (const auto& pair : table.symbols) {
        const Symbol& sym = pair.second;
        if (sym.kind == SymbolKind::FUNCTION) continue;

        std::string kind = sym.kind == SymbolKind::PARAMETER ? "param" : "var";
        std::cout << "    " << sym.qualified_name
                  << " : " << toString(sym.type)
                  << "  (" << kind << ")"
                  << "  [" << sym.line << ":" << sym.column << "]\n";
    }
}

// ---------------- MAIN ----------------
int main(int argc, char* argv[]) {
    std::string filename;
    bool verbose = false;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--verbose" || arg == "-v") {
            verbose = true;
        } else {
            filename = arg;
        }
    }

    if (filename.empty()) {
        std::cerr << "blc: no input file\n";
        std::cerr << "Usage: blc <file.bl> [--verbose]\n";
        return 1;
    }

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "blc: cannot open file '" << filename << "'\n";
        return 1;
    }

    std::ostringstream buffer;
    std::string line;
    while (std::getline(file, line)) {
        buffer << line << '\n';
    }
    std::string source = buffer.str();

    std::cout << "blc: compiling '" << filename << "'\n";

    // -------- LEXING --------
    Lexer lexer(source);
    std::vector<Token> tokens;

    try {
        tokens = lexer.tokenise();
    } catch (const std::exception& e) {
        std::cerr << "blc: lexer error: " << e.what() << "\n";
        return 1;
    }

    if (verbose) printTokens(tokens);

    // -------- PARSING --------
    Parser parser(tokens);
    std::unique_ptr<ProgramNode> ast;

    try {
        ast = parser.parseProgram();
    } catch (const std::exception& e) {
        std::cerr << "\nblc: " << e.what() << "\n";
        return 1;
    }

    if (verbose) {
        std::cout << "\n--- AST ---\n";
        printAST(ast.get(), 1);
    }

    // -------- SYMBOL TABLE --------
    SymbolTableGenerator generator;
    SymbolTable table = generator.generate(ast.get());

    if (verbose) printSymbolTable(table);

    auto errors = generator.getErrors();
    if (!errors.empty()) {
        std::cout << "\n";
        for (const auto& err : errors) {
            std::cerr << err;
        }
        std::cerr << "\nblc: " << errors.size() << " error(s) found. compilation stopped.\n";
        return 1;
    }

    // -------- TYPE CHECKING --------
    TypeChecker type_checker(table);
    type_checker.check(ast.get());
    
    auto type_errors = type_checker.getErrors();
    if (!type_errors.empty()) {
        std::cout << "\n";
        for (const auto& err : type_errors) {
            std::cerr << err;
        }
        std::cerr << "\nblc: " << type_errors.size() << " type error(s) found. compilation stopped.\n";
        return 1;
    }
    
    if (verbose) {
        std::cout << "\n--- Type Check ---\n";
        std::cout << "  No type errors found.\n";
    }

    // -------- CODE GEN --------
    CodeGenerator code_gen(table);
    std::filesystem::path p(filename);
    std::string stem = p.stem().string();
    std::string asm_file = stem + ".s";
    code_gen.generate(ast.get(), asm_file);

    if (verbose) {
        std::cout << "\n--- Code Gen ---\n";
        std::cout << "  Generated assembly: " << asm_file << "\n";
    }

    if (verbose) {
        std::cout << "  Assembling file: " << asm_file << "\n";
    }

    std::string cmd = "as -o " + stem + ".o " + asm_file;
    int res = system(cmd.c_str());
    if (res != 0) {
        std::cerr << "assembler failed\n";
        exit(1);
    }
    
    cmd = "gcc " + stem + ".o -o " + stem + " -no-pie";
    res = system(cmd.c_str());
    if (res != 0) {
        std::cerr << "linker failed\n";
        exit(1);
    }

    std::cout << "blc: compiled successfully\n";
    return 0;
}
