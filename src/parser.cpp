#include <iostream>

#include "parser.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}

std::unique_ptr<ProgramNode> Parser::parseProgram() {
    auto program = std::make_unique<ProgramNode>();

    while (peek().type != TokenType::END_OF_FILE) {
        auto func = parseFunctionDeclaration();
        program->function_declarations.push_back(std::move(func));
    }

    return program;
}

std::unique_ptr<FunctionDeclarationNode> Parser::parseFunctionDeclaration() {
    auto func = std::make_unique<FunctionDeclarationNode>();
    func->line = peek().line;
    func->column = peek().column;

    expect(TokenType::FUNCTION, "at the start of a function");

    expect(TokenType::IDENTIFIER, "as function name");
    func->identifier = tokens[position-1].value;

    expect(TokenType::LEFT_BRACKET, "after function name");

    func->parameters = parseParameterList();

    expect(TokenType::RIGHT_BRACKET, "after parameters");

    expect(TokenType::ARROW, "after closing bracket");

    func->return_type = parseType();

    func->body = parseBlock();

    return func;
}

std::vector<std::unique_ptr<ParameterNode>> Parser::parseParameterList() {
    std::vector<std::unique_ptr<ParameterNode>> params;

    if (peek().type != TokenType::RIGHT_BRACKET) {
        params.push_back(std::move(parseParameter()));

        while (peek().type == TokenType::COMMA) {
            advance();
            params.push_back(std::move(parseParameter()));
        }
    }

    return params;
}

std::unique_ptr<ParameterNode> Parser::parseParameter() {
    auto param = std::make_unique<ParameterNode>();

    param->type = parseType();

    param->line = peek().line;
    param->column = peek().column;

    expect(TokenType::IDENTIFIER, "as name of parameter");
    param->identifier = tokens[position-1].value;
   
    return param;
}

Type Parser::parseType() {
    if (peek().type == TokenType::INT8) {
        advance();
        return Type::INT8;
    }

    if (peek().type == TokenType::INT16) {
        advance();
        return Type::INT16;
    }

    if (peek().type == TokenType::INT32 || peek().type == TokenType::INT) {
        advance();
        return Type::INT32;
    }

    if (peek().type == TokenType::INT64) {
        advance();
        return Type::INT64;
    }

    if (peek().type == TokenType::INT16) {
        advance();
        return Type::INT16;
    }

    if (peek().type == TokenType::INT32 || peek().type == TokenType::INT) {
        advance();
        return Type::INT32;
    }

    if (peek().type == TokenType::UINT8) {
        advance();
        return Type::UINT8;
    }

    if (peek().type == TokenType::UINT16) {
        advance();
        return Type::UINT16;
    }

    if (peek().type == TokenType::UINT32 || peek().type == TokenType::UINT) {
        advance();
        return Type::UINT32;
    }

    if (peek().type == TokenType::UINT64) {
        advance();
        return Type::UINT64;
    }

    if (peek().type == TokenType::STRING) {
        advance();
        return Type::STRING;
    }

    if (peek().type == TokenType::BOOL) {
        advance();
        return Type::BOOL;
    }

    error("Expected type 'int', 'string' or 'bool'");
    return Type::INT32;
}

std::vector<std::unique_ptr<ASTNode>> Parser::parseBlock() {
    expect(TokenType::LEFT_BRACE, "at start of block");

    std::vector<std::unique_ptr<ASTNode>> statements;

    while (peek().type != TokenType::RIGHT_BRACE && peek().type != TokenType::END_OF_FILE) {
        statements.push_back(std::move(parseStatement()));
    }

    expect(TokenType::RIGHT_BRACE, "at end of block");

    return statements;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    Token current = peek();

    switch (current.type) {
    case TokenType::INT:
    case TokenType::INT8:
    case TokenType::INT16:
    case TokenType::INT32:
    case TokenType::INT64:
    case TokenType::UINT:
    case TokenType::UINT8:
    case TokenType::UINT16:
    case TokenType::UINT32:
    case TokenType::UINT64:
    case TokenType::BOOL:
    case TokenType::STRING:
        if (tokens[position+1].type == TokenType::IDENTIFIER && tokens[position+2].type == TokenType::LEFT_SQUARE) {
            return parseArrayDeclaration();
        }
        return  parseVariableDeclaration();

    case TokenType::IDENTIFIER:
        switch (tokens[position+1].type) {
        case TokenType::ASSIGN:
            return parseVariableAssignment();

        case TokenType::LEFT_SQUARE:
            return parseArrayAssignment();

        default:
            return parseExpressionStatement();
        }

    case TokenType::NUMBER:
    case TokenType::TRUE:
    case TokenType::FALSE:
        return parseExpressionStatement();

    case TokenType::IF:
        return parseIfStatement();

    case TokenType::WHILE:
        return parseWhileStatement();

    case TokenType::FOR:
        return parseForStatement();

    case TokenType::RETURN:
        return parseReturnStatement();

    default:
        error("Expected a statement (variable declaration, if, while, for, return, or expression)");
        return nullptr;
    }
}

std::unique_ptr<VariableDeclarationNode> Parser::parseVariableDeclaration() {
    auto variable_declaration = std::make_unique<VariableDeclarationNode>();
    variable_declaration->line = peek().line;
    variable_declaration->column = peek().column;
    variable_declaration->type = parseType();

    expect(TokenType::IDENTIFIER, "as name of variable");
    variable_declaration->identifier = tokens[position-1].value;

    if (peek().type == TokenType::ASSIGN) {
        advance();
        variable_declaration->value = parseExpression();
    }
    expect(TokenType::SEMICOLON, "after variable declaration");
    return variable_declaration;
}

std::unique_ptr<ArrayDeclarationNode> Parser::parseArrayDeclaration() {
    auto arr = std::make_unique<ArrayDeclarationNode>();
    arr->line = peek().line;
    arr->column = peek().column;

    arr->element_type = parseType();

    expect(TokenType::IDENTIFIER, "as name of array");
    arr->identifier = tokens[position-1].value;

    expect(TokenType::LEFT_SQUARE, "after name of array");

    if (peek().type != TokenType::RIGHT_SQUARE) {
        arr->size_expr = parseExpression();
    }

    expect(TokenType::RIGHT_SQUARE, "after array size");

    if (peek().type == TokenType::ASSIGN) {
        advance();
        arr->elements = parseArrayLiteral();
    }

    expect(TokenType::SEMICOLON, "after array declaration");

    if ((arr->size_expr == nullptr) == (arr->elements == nullptr)) {
        error("array must have either a size or a literal value, not both or neither", arr->line, arr->column);
        return nullptr;
    }

    return arr;
}

std::unique_ptr<VariableAssignmentNode> Parser::parseVariableAssignment() {
    auto variable_assignment = std::make_unique<VariableAssignmentNode>();
    variable_assignment->line = peek().line;
    variable_assignment->column = peek().column;

    expect(TokenType::IDENTIFIER, "as variable name");
    variable_assignment->identifier = tokens[position-1].value;

    expect(TokenType::ASSIGN, "after variable name");

    variable_assignment->value = parseExpression();

    expect(TokenType::SEMICOLON, "after variable assignment");

    return variable_assignment;
}

std::unique_ptr<ArrayAssignmentNode> Parser::parseArrayAssignment() {
    auto arr = std::make_unique<ArrayAssignmentNode>();
    arr->line = peek().line;
    arr->column = peek().column;

    expect(TokenType::IDENTIFIER, "as array name");
    arr->identifier = tokens[position-1].value;

    expect(TokenType::LEFT_SQUARE, "after array name");

    if (peek().type != TokenType::RIGHT_SQUARE) {
        arr->index = parseExpression();
    }

    expect(TokenType::RIGHT_SQUARE, "after array index");

    expect(TokenType::ASSIGN, "after array");

    if (peek().type == TokenType::LEFT_SQUARE) {
        arr->value = parseArrayLiteral();
    }

    else {
        arr->value = parseExpression();
    }

    expect(TokenType::SEMICOLON, "after array assignment");

    return arr;
}

std::unique_ptr<ArrayAccessNode> Parser::parseArrayAccess() {
    auto arr = std::make_unique<ArrayAccessNode>();
    arr->line = peek().line;
    arr->column = peek().column;

    expect(TokenType::IDENTIFIER, "as name of array");
    arr->identifier = tokens[position-1].value;

    expect(TokenType::LEFT_SQUARE, "after name of array");

    arr->index = parseExpression();

    expect(TokenType::RIGHT_SQUARE, "after array index");
    return arr;
}

std::unique_ptr<ReturnNode> Parser::parseReturnStatement() {
    auto return_statement = std::make_unique<ReturnNode>();
    return_statement->line = peek().line;
    return_statement->column = peek().column;

    expect(TokenType::RETURN, "at start of return statement");

    if (isExpression()) {
        return_statement->expression = parseExpression();
    }

    expect(TokenType::SEMICOLON, "at end of return statement");
    
    return return_statement;
}

std::unique_ptr<ASTNode> Parser::parseExpressionStatement() {
    auto expression = parseExpression();

    expect(TokenType::SEMICOLON, "at end of statement");

    return expression;
}

std::unique_ptr<IfStatementNode> Parser::parseIfStatement() {
    auto if_statement = std::make_unique<IfStatementNode>();
    if_statement->line = peek().line;
    if_statement->column = peek().column;

    expect(TokenType::IF, "at start of if statement");

    expect(TokenType::LEFT_BRACKET, "after 'if'");

    if_statement->expression = parseExpression();

    expect(TokenType::RIGHT_BRACKET, "after expression");

    if_statement->body = parseBlock();

    if (peek().type == TokenType::ELSE) {
        if_statement->else_statement = parseElseStatement();
    }

    return if_statement;
}

std::unique_ptr<ElseStatementNode> Parser::parseElseStatement() {
    auto else_statement = std::make_unique<ElseStatementNode>();
    else_statement->line = peek().line;
    else_statement->column = peek().column;

    expect(TokenType::ELSE, "at start of else statement");

    else_statement->body = parseBlock();

    return else_statement;
}

std::unique_ptr<WhileStatementNode> Parser::parseWhileStatement() {
    auto while_statement = std::make_unique<WhileStatementNode>();
    while_statement->line = peek().line;
    while_statement->column = peek().column;

    expect(TokenType::WHILE, "at start of while statement");

    expect(TokenType::LEFT_BRACKET, "after 'while'");

    while_statement->expression = parseExpression();

    expect(TokenType::RIGHT_BRACKET, "after expression");

    while_statement->body = parseBlock();

    return while_statement;
}

std::unique_ptr<ForStatementNode> Parser::parseForStatement() {
    auto for_statement = std::make_unique<ForStatementNode>();
    for_statement->line = peek().line;
    for_statement->column = peek().column;

    expect(TokenType::FOR, "at start of for statement");

    expect(TokenType::LEFT_BRACKET, "after 'for'");

    if (peek().type != TokenType::SEMICOLON) {
        for_statement->init = parseForInit();
    }

    expect(TokenType::SEMICOLON, "after initialisation statement");

    if (isExpression()) {
        for_statement->condition = parseExpression();
    }

    expect(TokenType::SEMICOLON, "after conditional expression");

    if (isExpression()) {
        for_statement->update = parseForUpdate();
    }

    expect(TokenType::RIGHT_BRACKET, "after update expression");

    for_statement->body = parseBlock();

    return for_statement;
}

std::unique_ptr<ForInitNode> Parser::parseForInit() {
    auto for_init = std::make_unique<ForInitNode>();
    for_init->line = peek().line;
    for_init->column = peek().column;

    switch (peek().type) {
    case TokenType::INT:
    case TokenType::INT8:
    case TokenType::INT16:
    case TokenType::INT32:
    case TokenType::INT64:
    case TokenType::UINT:
    case TokenType::UINT8:
    case TokenType::UINT16:
    case TokenType::UINT32:
    case TokenType::UINT64:
    case TokenType::STRING:
    case TokenType::BOOL:
        for_init->type = parseType();

        for_init->line = peek().line;
        for_init->column = peek().column;

        expect(TokenType::IDENTIFIER, "as name of variable");
        for_init->identifier = tokens[position-1].value;

        if (peek().type == TokenType::ASSIGN) {
            advance();

            for_init->expression = parseExpression();
        }

        break;

    default:
        for_init->line = peek().line;
        for_init->column = peek().column;
        expect(TokenType::IDENTIFIER, "as name of variable");
        for_init->identifier = tokens[position-1].value;

        if (peek().type == TokenType::ASSIGN) {
            advance();

            for_init->expression = parseExpression();
        }
    }

    return for_init;
}

std::unique_ptr<VariableAssignmentNode> Parser::parseForUpdate() {
    auto update = std::make_unique<VariableAssignmentNode>();
    update->line = peek().line;
    update->column = peek().column;
    
    expect(TokenType::IDENTIFIER, "as variable name");
    update->identifier = tokens[position-1].value;

    expect(TokenType::ASSIGN, "after identifier");

    update->value = parseExpression();

    return update;
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseLogicalOr();
}

std::unique_ptr<ASTNode> Parser::parseLogicalOr() {
    auto left = parseLogicalAnd();

    while (peek().type == TokenType::OR) {
        std::string op = peek().value;
        int line = peek().line;
        int column = peek().column;
        advance();

        auto right = parseLogicalAnd();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
        left->line = line;
        left->column = column;
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseLogicalAnd() {
    auto left = parseEquality();

    while (peek().type == TokenType::AND) {
        std::string op = peek().value;
        int line = peek().line;
        int column = peek().column;
        advance();

        auto right = parseEquality();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
        left->line = line;
        left->column = column;
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseEquality() {
    auto left = parseComparison();

    while (peek().type == TokenType::EQUAL || peek().type == TokenType::NOT_EQUAL) {
        std::string op = peek().value;
        int line = peek().line;
        int column = peek().column;
        advance();

        auto right = parseComparison();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
        left->line = line;
        left->column = column;
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
    auto left = parseTerm();

    while (peek().type == TokenType::LESS || peek().type == TokenType::GREATER || peek().type == TokenType::LESS_EQUAL || peek().type == TokenType::GREATER_EQUAL) {
        std::string op = peek().value;
        int line = peek().line;
        int column = peek().column;
        advance();

        auto right = parseTerm();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
        left->line = line;
        left->column = column;
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto left = parseFactor();

    while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
        std::string op = peek().value;
        int line = peek().line;
        int column = peek().column;
        advance();

        auto right = parseFactor();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
        left->line = line;
        left->column = column;
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    auto left = parseUnary();

    while (peek().type == TokenType::MULTIPLY || peek().type == TokenType::DIVIDE || peek().type == TokenType::MOD) {
        std::string op = peek().value;
        int line = peek().line;
        int column = peek().column;
        advance();

        auto right = parseUnary();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
        left->line = line;
        left->column = column;
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
    if (peek().type == TokenType::NOT) {
        std::string op = peek().value;
        int line = peek().line;
        int column = peek().column;
        advance();

        auto right = parsePrimary();
        auto left = std::make_unique<UnaryOperationNode>(op, std::move(right));
        left->line = line;
        left->column = column;
        return left;
    }

    if (peek().type == TokenType::MINUS) {
        if (tokens[position+1].type == TokenType::NUMBER) {
            advance();
            std::string value = peek().value;
            int line = peek().line;
            int column = peek().column;
            advance();
            auto number = std::make_unique<NumberLiteralNode>(std::stoll(value) * -1);
            number->line = line;
            number->column = column;
            return number;
        }

        else {
            std::string op = peek().value;
            int line = peek().line;
            int column = peek().column;
            advance();
            auto right = parsePrimary();
            auto left = std::make_unique<UnaryOperationNode>(op, std::move(right));
            left->line = line;
            left->column = column;
            return left;
        }
    }

    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    std::string value;
    int line;
    int column;
    
    switch (peek().type) {

    case TokenType::NUMBER: {
        value = peek().value;
        line = peek().line;
        column = peek().column;
        advance();

        auto number = std::make_unique<NumberLiteralNode>(std::stoll(value));
        number->line = line;
        number->column = column;

        return number;
    }

    case TokenType::STRING_LITERAL: {
        value = peek().value;
        line = peek().line;
        column = peek().column;
        advance();

        auto s = std::make_unique<StringLiteralNode>(value);
        s->line = line;
        s->column = column;

        return s;
    }

    case TokenType::TRUE:
    case TokenType::FALSE: {
        value = peek().value;
        line = peek().line;
        column = peek().column;
        advance();

        auto boolean = std::make_unique<BooleanLiteralNode>(value);
        boolean->line = line;
        boolean->column = column;

        return boolean;
    }

    case TokenType::IDENTIFIER: {
        if (tokens[position + 1].type == TokenType::LEFT_BRACKET) {
            return parseFunctionCall();
        }

        if (tokens[position + 1].type == TokenType::LEFT_SQUARE) {
            return parseArrayAccess();
        }

        value = peek().value;
        line = peek().line;
        column = peek().column;
        advance();

        auto identifier = std::make_unique<IdentifierNode>(value);
        identifier->line = line;
        identifier->column = column;

        return identifier;
    }

    case TokenType::LEFT_BRACKET: {
        advance();
        auto expression = parseExpression();
        expression->line = peek().line;
        expression->column = peek().column;

        expect(TokenType::RIGHT_BRACKET, "after expression");

        return expression;
    }

    default: {
        return nullptr;
    }
    }
}

std::unique_ptr<ArrayLiteralNode> Parser::parseArrayLiteral() {
    int line = peek().line;
    int column = peek().column;
    std::vector<std::unique_ptr<ASTNode>> elements;

    expect(TokenType::LEFT_SQUARE, "at start of array literal");

    while (peek().type != TokenType::RIGHT_SQUARE) {
        elements.push_back(std::move(parseExpression()));
        if (peek().type == TokenType::RIGHT_SQUARE) {
            break;
        }
        expect(TokenType::COMMA, "between elements in array literal");
    }
    advance();

    auto arr_lit = std::make_unique<ArrayLiteralNode>(std::move(elements));
    arr_lit->line = line;
    arr_lit->column = column;

    return arr_lit;
}

std::unique_ptr<FunctionCallNode> Parser::parseFunctionCall() {
    auto function_call = std::make_unique<FunctionCallNode>();
    function_call->line = peek().line;
    function_call->column = peek().column;

    expect(TokenType::IDENTIFIER, "as name of function");
    function_call->identifier = tokens[position-1].value;

    expect(TokenType::LEFT_BRACKET, "after identifier");

    if (isExpression()) {
        function_call->arguments = parseArgumentList();
    }

    expect(TokenType::RIGHT_BRACKET, "after arguments");

    return function_call;
}

std::vector<std::unique_ptr<ASTNode>> Parser::parseArgumentList() {
    std::vector<std::unique_ptr<ASTNode>> argument_list;

    argument_list.push_back(std::move(parseExpression()));

    while (peek().type == TokenType::COMMA) {
        advance();
        argument_list.push_back(std::move(parseExpression()));
    }

    return argument_list;
}

Token Parser::advance() {
    return tokens[position++];
}

Token Parser::peek() {
    return tokens[position];
}

void Parser::error(const std::string& message, int line, int column) {
    std::cerr << "ERROR: " << message << "\n";
    std::cerr << "       at line " << line << ", column " << column << "\n\n";


    throw std::runtime_error("Parsing failed: " + message);
}

void Parser::error(const std::string& message) {
    int line = peek().line;
    int column = peek().column;

    error(message, line, column);
}

void Parser::expect(TokenType expected, const std::string& context) {
    if (peek().type != expected) {
        std::string msg = "Expected " + tokenTypeToString(expected) + " " + context + ", but found " + tokenTypeToString(peek().type);
        error(msg);
    }
    advance();
}

std::string Parser::tokenTypeToString(TokenType type) {
    switch(type) {
        case TokenType::INT: return "'int'";
        case TokenType::STRING: return "'string'";
        case TokenType::FUNCTION: return "'function'";
        case TokenType::NUMBER: return "number literal";
        case TokenType::IDENTIFIER: return "identifier (variable/function name)";
        case TokenType::RETURN: return "'return'";
        case TokenType::IF: return "'if'";
        case TokenType::WHILE: return "'while'";
        case TokenType::FOR: return "'for'";
        case TokenType::PLUS: return "'+'";
        case TokenType::MINUS: return "'-'";
        case TokenType::MULTIPLY: return "'*'";
        case TokenType::DIVIDE: return "'/'";
        case TokenType::EQUAL: return "'=='";
        case TokenType::LESS_EQUAL: return "'<='";
        case TokenType::GREATER_EQUAL: return "'=>'";
        case TokenType::LESS: return "'<'";
        case TokenType::GREATER: return "'>'";
        case TokenType::ASSIGN: return "'='";
        case TokenType::ARROW: return "'->'";
        case TokenType::SEMICOLON: return "';'";
        case TokenType::LEFT_BRACKET: return "'('";
        case TokenType::RIGHT_BRACKET: return "')'";
        case TokenType::LEFT_BRACE: return "'{'";
        case TokenType::RIGHT_BRACE: return "'}'";
        case TokenType::END_OF_FILE: return "EOF (End Of File)";
        case TokenType::INVALID: return "Invalid token";
        default:
            return "UNKNOWN TOKEN";
    }
}

bool Parser::isExpression() {
    TokenType current = peek().type;
    return current == TokenType::NUMBER || current == TokenType::TRUE || current == TokenType::FALSE || current == TokenType::IDENTIFIER || current == TokenType::LEFT_BRACKET || current == TokenType::MINUS || current == TokenType::NOT;
}
