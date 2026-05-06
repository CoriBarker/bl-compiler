#include "symbol_table_generator.hpp"

#include <iostream>

SymbolTableGenerator::SymbolTableGenerator() : scope_counter(0) {}

SymbolTable SymbolTableGenerator::generate(ProgramNode* program) {
    table.enterScope("global");

    visitProgram(program);

    return std::move(table);
}

void SymbolTableGenerator::visitProgram(ProgramNode* node) {
    Symbol syscall_symbol;
    syscall_symbol.name = "syscall";
    syscall_symbol.qualified_name = "global::syscall";
    syscall_symbol.kind = SymbolKind::FUNCTION;
    syscall_symbol.type = Type::VOID;
    syscall_symbol.parameter_types = { Type::INT64 };
    syscall_symbol.line = 0;
    syscall_symbol.column = 0;
    table.define(syscall_symbol);

    Symbol strlen_symbol;
    strlen_symbol.name = "strlen";
    strlen_symbol.qualified_name = "global::strlen";
    strlen_symbol.kind = SymbolKind::FUNCTION;
    strlen_symbol.type = Type::INT32;
    strlen_symbol.parameter_types = { Type::STRING };
    strlen_symbol.line = 0;
    strlen_symbol.column = 0;
    table.define(strlen_symbol);

    for (int i = 0; i < (int)node->function_declarations.size(); i++) {
        auto& function = node->function_declarations[i];
        std::string name = function->identifier;
        std::string qualified_name = "global::" + name;
        
        Symbol symbol;
        symbol.name = name;
        symbol.qualified_name = qualified_name;
        symbol.kind = SymbolKind::FUNCTION;
        symbol.type = function->return_type;
        symbol.line = function->line;
        symbol.column = function->column;

        for (int j = 0; j < (int)function->parameters.size(); j++) {
            symbol.parameter_types.push_back(function->parameters[j]->type);
        }

        table.define(symbol);
    }

    for (int i = 0; i < (int)node->function_declarations.size(); i++) {
        visitFunctionDeclaration(node->function_declarations[i].get());
    }
}

void SymbolTableGenerator::visitFunctionDeclaration(FunctionDeclarationNode* node) {
    table.enterScope(node->identifier);

    for (int i = 0; i < (int)node->parameters.size(); i++) {
        auto& parameter = node->parameters[i];
        std::string name = parameter->identifier;
        std::string qualified_name = node->identifier + "::" + parameter->identifier;

        Symbol symbol;
        symbol.name = name;
        symbol.qualified_name = qualified_name;
        symbol.kind = SymbolKind::PARAMETER;
        symbol.type = parameter->type;
        symbol.line = parameter->line;
        symbol.column = parameter->column;

        table.define(symbol);
    }

    for (int i = 0; i < (int)node->body.size(); i++) {
        visitStatement(node->body[i].get());
    }

    table.exitScope();
}

void SymbolTableGenerator::visitStatement(ASTNode* node) {
    if (auto* p = dynamic_cast<VariableDeclarationNode*>(node)) {
        visitVariableDeclaration(p);
    }

    else if (auto* p = dynamic_cast<ArrayDeclarationNode*>(node)) {
        visitArrayDeclaration(p);
    }

    else if (auto* p = dynamic_cast<VariableAssignmentNode*>(node)) {
        visitVariableAssignment(p);
    }

    else if (auto* p = dynamic_cast<ArrayAssignmentNode*>(node)) {
        visitArrayAssignment(p);
    }

    else if (auto* p = dynamic_cast<IfStatementNode*>(node)) {
        visitIfStatement(p);
    }

    else if (auto* p = dynamic_cast<WhileStatementNode*>(node)) {
        visitWhileStatement(p);
    }
    
    else if (auto* p = dynamic_cast<ForStatementNode*>(node)) {
        visitForStatement(p);
    }

    else if (auto* p = dynamic_cast<ReturnNode*>(node)) {
        if (p->expression) {
            visitExpression(p->expression.get());
        }
    }
}

void SymbolTableGenerator::visitVariableDeclaration(VariableDeclarationNode* node) {
    if (table.lookupCurrent(node->identifier)) {
        error("'" + node->identifier + "' is already declared in this scope", node->line, node->column);
        return;
    }

    std::string qualified_name = table.scopes.back()->scope_name + "::" + node->identifier;

    Symbol symbol;
    symbol.name = node->identifier;
    symbol.qualified_name = qualified_name;
    symbol.kind = SymbolKind::VARIABLE;
    symbol.type = node->type;
    symbol.line = node->line;
    symbol.column = node->column;

    table.define(symbol);

    if (node->value) {
        visitExpression(node->value.get());
    }
}

void SymbolTableGenerator::visitArrayDeclaration(ArrayDeclarationNode* node) {
    if (table.lookupCurrent(node->identifier)) {
        error("'" + node->identifier + "' is already declared in this scope", node->line, node->column);
        return;
    }

    std::string qualified_name = table.scopes.back()->scope_name + "::" + node->identifier;

    Symbol symbol;
    symbol.name = node->identifier;
    symbol.qualified_name = qualified_name;
    symbol.kind = SymbolKind::ARRAY;
    symbol.type = node->element_type;
    symbol.line = node->line;
    symbol.column = node->column;

    if (auto* p = dynamic_cast<NumberLiteralNode*>(node->size_expr.get())) {
        symbol.array_size = p->value;
    }
    else if (node->elements) {
        symbol.array_size = (int)node->elements->value.size();
    }

    table.define(symbol);

    if (node->elements) {
        visitExpression(node->elements.get());
    }
}

void SymbolTableGenerator::visitVariableAssignment(VariableAssignmentNode* node) {
    if (!table.lookup(node->identifier)) {
        error("'" + node->identifier + "' is undefined", node->line, node->column);
        return;
    }

    visitExpression(node->value.get());
}

void SymbolTableGenerator::visitArrayAssignment(ArrayAssignmentNode* node) {
    if (!table.lookup(node->identifier)) {
        error("'" + node->identifier + "' is undefined", node->line, node->column);
        return;
    }

    visitExpression(node->value.get());
}

void SymbolTableGenerator::visitIfStatement(IfStatementNode* node) {
    visitExpression(node->expression.get());

    std::string scope_name = table.scopes.back()->scope_name + "::if_" + std::to_string(scope_counter++);
    table.enterScope(scope_name);
    
    for (int i = 0; i < (int)node->body.size(); i++) {
        visitStatement(node->body[i].get());
    }

    table.exitScope();

    if (node->else_statement) {
        std::string else_scope_name = table.scopes.back()->scope_name + "::if_else" + std::to_string(scope_counter++);
        table.enterScope(else_scope_name);

        for (int i = 0; i < (int)node->else_statement->body.size(); i++) {
            visitStatement(node->else_statement->body[i].get());
        }

        table.exitScope();
    }
}

void SymbolTableGenerator::visitWhileStatement(WhileStatementNode* node) {
    visitExpression(node->expression.get());

    std::string scope_name = table.scopes.back()->scope_name + "::while_" + std::to_string(scope_counter++);
    table.enterScope(scope_name);

    for (int i = 0; i < (int)node->body.size(); i++) {
        visitStatement(node->body[i].get());
    }

    table.exitScope();
}

void SymbolTableGenerator::visitForStatement(ForStatementNode* node) {
    std::string scope_name = table.scopes.back()->scope_name + "::for_" + std::to_string(scope_counter++);
    table.enterScope(scope_name);

    if (node->init) {
        if (!node->init->identifier.empty() && node->init->type != Type::VOID) {
            std::string name = node->init->identifier;
            std::string qualified_name = table.scopes.back()->scope_name + "::" + name;

            Symbol symbol;
            symbol.name = name;
            symbol.qualified_name = qualified_name;
            symbol.kind = SymbolKind::VARIABLE;
            symbol.type = node->init->type;
            symbol.line = node->init->line;
            symbol.column = node->init->column;

            table.define(symbol);

            if (node->init->expression) {
                visitExpression(node->init->expression.get());
            }
        }

        else {
            if (!table.lookup(node->init->identifier)) {
                error("'" + node->init->identifier + "' is undefined", node->init->line, node->init->column);
            }

            if (node->init->expression) {
                visitExpression(node->init->expression.get());
            }
        }
    }

    if (node->condition) {
        visitExpression(node->condition.get());
    }

    if (node->update) {
        visitStatement(node->update.get());
    }

    for (int i = 0; i < (int)node->body.size(); i++) {
        visitStatement(node->body[i].get());
    }

    table.exitScope();
}

void SymbolTableGenerator::visitExpression(ASTNode* node) {
    if (!node) return;
    
    if (auto p = dynamic_cast<IdentifierNode*>(node)) {
        if (!table.lookup(p->value)) {
            error("'" + p->value + "' is undefined", p->line, p->column);
        }
    }

    else if (auto p = dynamic_cast<FunctionCallNode*>(node)) {
        visitFunctionCall(p);
    }

    else if (auto p = dynamic_cast<BinaryOperationNode*>(node)) {
        visitExpression(p->left.get());
        visitExpression(p->right.get());
    }

    else if (auto p = dynamic_cast<UnaryOperationNode*>(node)) {
        visitExpression(p->operand.get());
    }

    else if (auto* p = dynamic_cast<ArrayLiteralNode*>(node)) {
        for (auto& it : p->value) {
            visitExpression(it.get());
        }
    }
}

void SymbolTableGenerator::visitFunctionCall(FunctionCallNode* node) {
    auto symbol = table.lookup(node->identifier);

    if (symbol == nullptr) {
        error("'" + node->identifier + "' is not defined", node->line, node->column);
        return;
    }

    if (symbol->kind != SymbolKind::FUNCTION) {
        error("'" + node->identifier + "' is not a function", node->line, node->column);
        return;
    }
    
    if (node->arguments.size() != symbol->parameter_types.size()) {
        error("'" + node->identifier + "' expects " + std::to_string(symbol->parameter_types.size()) + " arguments but got " + std::to_string(node->arguments.size()), node->line, node->column);
    }

    for (int i = 0; i < (int)node->arguments.size(); i++) {
        visitExpression(node->arguments[i].get());
    }
}

void SymbolTableGenerator::error(const std::string& message, int line, int column) {
    std::string err;
    err += "ERROR: " + message + "\n";
    err += "       at line " + std::to_string(line) + ", column " + std::to_string(column) + "\n";
 
    errors.push_back(err);
}
