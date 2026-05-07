#include "type_checker.hpp"

TypeChecker::TypeChecker(SymbolTable& table) : table(table) {}

void TypeChecker::check(ProgramNode* node) {
    auto& functions = node->function_declarations;

    for (int i = 0; i < (int)functions.size(); i++) {
        auto& func = functions[i];
        current_function_return_type = func->return_type;
        current_function_name = func->identifier;

        checkFunction(func.get());
    }
}

void TypeChecker::checkFunction(FunctionDeclarationNode* node) {
    for (int i = 0; i < (int)node->body.size(); i++) {
        checkStatement(node->body[i].get());
    }
}

void TypeChecker::checkStatement(ASTNode* node) {
    if (auto p = dynamic_cast<VariableDeclarationNode*>(node)) {
        checkVariableDeclaration(p);
    }

    else if (auto p = dynamic_cast<VariableAssignmentNode*>(node)) {
        checkVariableAssignment(p);
    }

    else if (auto p = dynamic_cast<ReturnNode*>(node)) {
        checkReturn(p);
    }

    else if (auto p = dynamic_cast<IfStatementNode*>(node)) {
        checkIf(p);
    }

    else if (auto p = dynamic_cast<WhileStatementNode*>(node)) {
        checkWhile(p);
    }

    else if (auto p = dynamic_cast<ForStatementNode*>(node)) {
        checkFor(p);
    }
}

void TypeChecker::checkVariableDeclaration(VariableDeclarationNode* node) {
    Type left = node->type;

    if (node->value) {
        Type right = inferType(node->value.get());

        if (left == Type::VOID || right == Type::VOID) {
            return;
        }

        if (!isAssignable(left, node->value.get())) {
            error("cannot assign '" + typeToString(right) + "' to '" + typeToString(left) + "' variable '" + node->identifier + "'", node->line, node->column);
        }
    }
}

void TypeChecker::checkVariableAssignment(VariableAssignmentNode* node) {
    Symbol* symbol = lookupVariable(node->identifier);
    if (!symbol) return;
    Type left = symbol->type;
    Type right = inferType(node->value.get());

    if (left == Type::VOID || right == Type::VOID) {
        return;
    }

    if (!isAssignable(left, node->value.get())) {
        error("cannot assign '" + typeToString(right) + "' to '" + typeToString(left) + "' variable '" + node->identifier + "'", node->line, node->column);
    }
}

void TypeChecker::checkReturn(ReturnNode* node) {
    Type return_type = inferType(node->expression.get());

    if (!isAssignable(current_function_return_type, node->expression.get())) {
        error("function returns '" + typeToString(current_function_return_type) + "' but got '" + typeToString(return_type) + "'", node->line, node->column);
    }
}

void TypeChecker::checkIf(IfStatementNode* node) {
    if (inferType(node->expression.get()) != Type::BOOL) {
        error("'if' condition must be 'bool' but got '" + typeToString(inferType(node->expression.get())) + "'", node->line, node->column);
    }

    for (int i = 0; i < (int)node->body.size(); i++) {
        checkStatement(node->body[i].get());
    }

    if (node->else_statement) {
        for (int i = 0; i < (int)node->else_statement->body.size(); i++) {
            checkStatement(node->else_statement->body[i].get());
        }
    }
}

void TypeChecker::checkWhile(WhileStatementNode* node) {
    if (inferType(node->expression.get()) != Type::BOOL) {
        error("'while' condition must be 'bool' but got '" + typeToString(inferType(node->expression.get())) + "'", node->line, node->column);
    }

    for (int i = 0; i < (int)node->body.size(); i++) {
        checkStatement(node->body[i].get());
    }
}

void TypeChecker::checkFor(ForStatementNode* node) {
    if (node->init && node->init->type != Type::VOID) {
        Type left = node->init->type;
        Type right = inferType(node->init->expression.get());

        if (left == Type::VOID || right == Type::VOID) {
            return;
        }

        if (!isAssignable(left, node->init->expression.get())) {
            error("cannot assign '" + typeToString(right) + "' to '" + typeToString(left) + "' variable '" + node->init->identifier + "'", node->init->line, node->init->column);
        }
    }

    if (node->condition) {
        if (inferType(node->condition.get()) != Type::BOOL) {
            error("'for' condition must be 'bool' but got '" + typeToString(inferType(node->condition.get())) + "'", node->condition->line, node->condition->column);
        }
    }

    if (node->update) {
        checkVariableAssignment(dynamic_cast<VariableAssignmentNode*>(node->update.get()));
    }

    for (int i = 0; i < (int)node->body.size(); i++) {
        checkStatement(node->body[i].get());
    }
}

Type TypeChecker::inferType(ASTNode* node) {
    if (!node) return Type::VOID;

    if (dynamic_cast<NumberLiteralNode*>(node)) {
        return Type::INTEGER_LITERAL;
    }

    if (dynamic_cast<BooleanLiteralNode*>(node)) {
        return Type::BOOL;
    }

    if (dynamic_cast<StringLiteralNode*>(node)) {
        return Type::STRING;
    }

    if (auto* p = dynamic_cast<IdentifierNode*>(node)) {
        return inferIdentifier(p);
    }

    if (auto* p = dynamic_cast<BinaryOperationNode*>(node)) {
        return inferBinaryOp(p);
    }
    
    if (auto* p = dynamic_cast<UnaryOperationNode*>(node)) {
        return inferUnaryOp(p);
    }

    if (auto* p = dynamic_cast<FunctionCallNode*>(node)) {
        return inferFunctionCall(p);
    }

    if (auto* p = dynamic_cast<CastNode*>(node)) {
        return p->type;
    }

    return Type::VOID;
}

Type TypeChecker::inferIdentifier(IdentifierNode* node) {
    if (Symbol* symbol = lookupVariable(node->value)) {
        return symbol->type;
    }

    error("'" + node->value + "' is undefined", node->line, node->column);
    return Type::VOID;
}

Type TypeChecker::inferBinaryOp(BinaryOperationNode* node) {
    if (node->operation == "+" || node->operation == "-" || node->operation == "*" || node->operation == "/" || node->operation == "%") {
        Type left = inferType(node->left.get());
        Type right = inferType(node->right.get());

        if (left == Type::VOID || right == Type::VOID) {
            return Type::VOID;
        }
        
        if (!isAssignable(left, node->right.get())) {
            error("cannot apply '" + node->operation + "' to '" + typeToString(left) + "' and '" + typeToString(right) + "'", node->line, node->column);
            return Type::VOID;
        }

        return left;
    }

    if (node->operation == "==" || node->operation == "!=" || node->operation == "<" || node->operation == ">" || node->operation == "<=" || node->operation == ">=") {
        Type left = inferType(node->left.get());
        Type right = inferType(node->right.get());

        if (left == Type::VOID || right == Type::VOID) {
            return Type::VOID;
        }

        if (!isAssignable(left, node->right.get())) {
            error("cannot compare '" + typeToString(left) + "' and '" + typeToString(right) + "' with '" + node->operation + "'", node->line, node->column);
            return Type::VOID;
        }

        return Type::BOOL;
    }

    if (node->operation == "&&" || node->operation == "||") {
        Type left = inferType(node->left.get());
        Type right = inferType(node->right.get());

        if (left == Type::VOID || right == Type::VOID) {
            return Type::VOID;
            error("cannot apply '" + node->operation + "' to '" + typeToString(left) + "' and '" + typeToString(right) + "'", node->line, node->column);
        }

        if (!isAssignable(left, node->right.get())) {
            return Type::VOID;
        }

        if (left != Type::BOOL) {
            error("'" + node->operation + "' requires 'bool' operands but left side is '" + typeToString(left) + "'", node->line, node->column);
            return Type::VOID;
        }

        if (right != Type::BOOL) {
            error("'" + node->operation + "' requires 'bool' operands but right side is '" + typeToString(right) + "'", node->line, node->column);
            return Type::VOID;
        }

        return Type::BOOL;
    }

    return Type::VOID;
}

Type TypeChecker::inferUnaryOp(UnaryOperationNode* node) {
    if (node->operation == "-") {
        if (!isInt(inferType(node->operand.get()))) {
            error("cannot apply '-' to '" + typeToString(inferType(node->operand.get())) + "', expected 'int'", node->line, node->column);
            return Type::VOID;
        }

        return Type::INT32;
    }

    if (node->operation == "!") {
        if (inferType(node->operand.get()) != Type::BOOL) {
            error("cannot apply '!' to '" + typeToString(inferType(node->operand.get())) + "', expected 'bool'", node->line, node->column);
            return Type::VOID;
        }

        return Type::BOOL;
    }

    return Type::VOID;
}

Type TypeChecker::inferFunctionCall(FunctionCallNode* node) {
    Symbol* symbol = lookupVariable(node->identifier);
    if (!symbol) return Type::VOID;

    if (node->arguments.size() != symbol->parameter_types.size() && node->identifier != "syscall") {
        error("'" + node->identifier + "' expects " + std::to_string(symbol->parameter_types.size()) + " argument(s) but got " + std::to_string(node->arguments.size()), node->line, node->column);
        return Type::VOID;
    }

    for (int i = 0; i < (int)node->arguments.size(); i++) {
        if (inferType(node->arguments[i].get()) != symbol->parameter_types[i]) {
            error("'" + node->identifier + "' expects '" + typeToString(symbol->parameter_types[i]) + "' for argument " + std::to_string(i + 1) + " but got '" + typeToString(inferType(node->arguments[i].get())) + "'", node->arguments[i]->line, node->arguments[i]->column);
            return Type::VOID;
        }
    }

    return symbol->type;
}

void TypeChecker::error(const std::string& message, int line, int column) {
    std::string err;
    err += "ERROR: " + message + "\n";
    err += "       at line " + std::to_string(line) + ", column " + std::to_string(column) + "\n";
 
    errors.push_back(err);
}

std::string TypeChecker::typeToString(Type type) {
    switch (type) {
#define X(name) case Type::name: return #name;
        TYPES
#undef X
    default: return "unknown";
    }
}

std::vector<std::string> TypeChecker::getErrors() {
    return errors;
}

Symbol* TypeChecker::lookupVariable(const std::string& name) {
    Symbol* symbol = table.lookupQualified(current_function_name + "::" + name);
    if (symbol) return symbol;

    symbol = table.lookupQualified("global::" + name);
    if (symbol) return symbol;

    std::string suffix = "::" + name;
    std::string prefix = current_function_name + "::";

    for (auto& pair : table.symbols) {
        const std::string& key = pair.first;
        if (key.size() > suffix.size() && key.substr(0, prefix.size()) == prefix && key.substr(key.size() - suffix.size()) == suffix) {
            return &pair.second;
        }
    }

    return nullptr;
}

bool TypeChecker::isInt(Type t) {
    switch (t) {
    case Type::INT8:
    case Type::INT16:
    case Type::INT32:
    case Type::INT64:
    case Type::UINT8:
    case Type::UINT16:
    case Type::UINT32:
    case Type::UINT64:
        return true;
    default:
        return false;
    }
}

int TypeChecker::getBitWidth(Type t) {
    switch (t) {
    case Type::INT8: case Type::UINT8: return 8;
    case Type::INT16: case Type::UINT16: return 16;
    case Type::INT32: case Type::UINT32: return 32;
    case Type::INT64: case Type::UINT64: return 64;
    default: return 0;
    }
}

bool TypeChecker::isSigned(Type t) {
    return t == Type::INT8 || t == Type::INT16 || t == Type::INT32 || t == Type::INT64;
}

bool TypeChecker::isAssignable(Type target, ASTNode* node) {
    Type source = inferType(node);
    if (target == source) return true;
    
    // literal fits into any int type
    if (source == Type::INTEGER_LITERAL && isInt(target)) return true;
    
    // int widening
    if (isInt(target) && isInt(source)) {
        int targetBits = getBitWidth(target);
        int sourceBits = getBitWidth(source);
        if (sourceBits <= targetBits) return true;
    }
    return false;
}
