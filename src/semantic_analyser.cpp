#include "semantic_analyser.hpp"

SemanticAnalyser::SemanticAnalyser(SymbolTable& table) : table(table) {}

void SemanticAnalyser::analyse(ProgramNode* node) {
    for (auto& it : node->function_declarations) {
        analyseFunction(it.get());
    }
}

void SemanticAnalyser::analyseFunction(FunctionDeclarationNode* node) {
    Context ctx;
    ctx.name = node->identifier;
    ctx.return_type = node->return_type;

    for (auto& param : node->parameters) {
        ctx.initialised_vars.insert(param->identifier);
    }

    for (auto& stmt : node->body) {
        analyseStatement(stmt.get(), ctx);
    }

    if (node->return_type != Type::VOID && !allPathsReturn(node->body)) {
        error("Function '" + node->identifier + "' does not return on all paths", node->line, node->column);
    }
}

void SemanticAnalyser::analyseStatement(ASTNode* node, Context& ctx) {
    if (auto* p = dynamic_cast<VariableDeclarationNode*>(node)) {
        analyseVariableDeclaration(p, ctx);
    }
    else if (auto* p = dynamic_cast<VariableAssignmentNode*>(node)) {
        analyseVariableAssignment(p, ctx);
    }
    else if (auto* p = dynamic_cast<ReturnNode*>(node)) {
        analyseReturn(p, ctx);
    }
    else if (auto* p = dynamic_cast<IfStatementNode*>(node)) {
        analyseIf(p, ctx);
    }
    else if (auto* p = dynamic_cast<WhileStatementNode*>(node)) {
        analyseWhile(p, ctx);
    }
    else if (auto* p = dynamic_cast<ForStatementNode*>(node)) {
        analyseFor(p, ctx);
    }
}

void SemanticAnalyser::analyseVariableDeclaration(VariableDeclarationNode* node, Context& ctx) {
    if (node->value) {
        ctx.initialised_vars.insert(node->identifier);
        analyseExpression(node->value.get(), ctx);
    }
}

void SemanticAnalyser::analyseArrayDeclaration(ArrayDeclarationNode* node, Context& ctx) {
    if (node->elements) {
        ctx.initialised_vars.insert(node->identifier);
        analyseExpression(node->elements.get(), ctx);
    }
}

void SemanticAnalyser::analyseVariableAssignment(VariableAssignmentNode* node, Context& ctx) {
    analyseExpression(node->value.get(), ctx);
    ctx.initialised_vars.insert(node->identifier);
}

void SemanticAnalyser::analyseArrayAssignment(ArrayAssignmentNode* node, Context& ctx) {
    Symbol* array = table.lookup(node->identifier);
    uint64_t array_size = array->array_size;
    
    if (auto* p = dynamic_cast<NumberLiteralNode*>(node->index.get())) {
        if (p->value >= array_size) {
            error("array index " + std::to_string(p->value) + " is out of bounds for array '" + node->identifier + "' of size " + std::to_string(array_size), node->line, node->column);
        }
        else if (p->value < 0) {
            error("array index " + std::to_string(p->value) + " is negative for array '" + node->identifier + "'", node->line, node->column);
        }
    }

    analyseExpression(node->value.get(), ctx);
    ctx.initialised_vars.insert(node->identifier);
}

void SemanticAnalyser::analyseReturn(ReturnNode* node, Context& ctx) {
    if (ctx.return_type == Type::VOID && node->expression) {
        error("void function '" + ctx.name + "' cannot return a value", node->line, node->column);
    }
    else if (ctx.return_type != Type::VOID && !node->expression) {
        error("non-void function '" + ctx.name + "' must return a value", node->line, node->column);
    }

    if (node->expression) {
        analyseExpression(node->expression.get(), ctx);
    }
}

void SemanticAnalyser::analyseIf(IfStatementNode* node, Context& ctx) {
    Context body_ctx = ctx;
    for (auto& it : node->body) {
        analyseStatement(it.get(), body_ctx);
    }

    if (node->else_statement) {
        Context else_ctx = ctx;
        for (auto& it : node->else_statement->body) {
            analyseStatement(it.get(), else_ctx);
        }
        
        const auto& smaller = (body_ctx.initialised_vars.size() < else_ctx.initialised_vars.size()) ? body_ctx.initialised_vars : else_ctx.initialised_vars;
        const auto& larger = (body_ctx.initialised_vars.size() < else_ctx.initialised_vars.size()) ? else_ctx.initialised_vars : body_ctx.initialised_vars;

        std::unordered_set<std::string> result;
        result.reserve(smaller.size());

        for (const auto& x : smaller) {
            if (larger.find(x) != larger.end()) {
                result.insert(x);
            }
        }
        ctx.initialised_vars.merge(result);
    }
}

void SemanticAnalyser::analyseWhile(WhileStatementNode* node, Context& ctx) {
    ctx.loop_depth++;
    Context while_ctx = ctx;

    for (auto& it : node->body) {
        analyseStatement(it.get(), while_ctx);
    }

    ctx.loop_depth--;
}

void SemanticAnalyser::analyseFor(ForStatementNode* node, Context& ctx) {
    ctx.loop_depth++;
    Context for_ctx = ctx;

    if (node->init) {
        if (for_ctx.initialised_vars.find(node->init->identifier) == for_ctx.initialised_vars.end() && node->init->expression) {
            for_ctx.initialised_vars.insert(node->init->identifier);
        }
    }

    if (node->condition) {
        analyseExpression(node->condition.get(), for_ctx);
    }

    if (node->update) {
        analyseExpression(node->update.get(), for_ctx);
    }

    for (auto& it : node->body) {
        analyseStatement(it.get(), for_ctx);
    }

    ctx.loop_depth--;
}

void SemanticAnalyser::analyseFunctionCall(FunctionCallNode* node, Context& ctx) {
    for (auto& it : node->arguments) {
        analyseExpression(it.get(), ctx);
    }
}

void SemanticAnalyser::analyseExpression(ASTNode* node, Context& ctx) {
    if (auto* p = dynamic_cast<IdentifierNode*>(node)) {
        analyseIdentifier(p, ctx);
    }
    else if (auto* p = dynamic_cast<FunctionCallNode*>(node)) {
        analyseFunctionCall(p, ctx);
    }
    else if (auto* p = dynamic_cast<BinaryOperationNode*>(node)) {
        analyseBinOp(p, ctx);
    }
    else if (auto* p = dynamic_cast<UnaryOperationNode*>(node)) {
        analyseUnaryOp(p, ctx);
    }
    else if (auto* p = dynamic_cast<ArrayAccessNode*>(node)) {
        analyseArrayAccess(p, ctx);
    }
    else if (auto* p = dynamic_cast<ArrayLiteralNode*>(node)) {
        analyseArrayLiteral(p, ctx);
    }
}

void SemanticAnalyser::analyseIdentifier(IdentifierNode* node, Context& ctx) {
    if (ctx.initialised_vars.find(node->value) == ctx.initialised_vars.end()) {
        error("variable '" + node->value + "' may be used before initialisation", node->line, node->column);
    }
}

void SemanticAnalyser::analyseBinOp(BinaryOperationNode* node, Context& ctx) {
    if (auto* p = dynamic_cast<NumberLiteralNode*>(node->right.get())) {
        if (node->operation == "/" && p->value == 0) {
            error("division by zero", node->line, node->column);
        }
    }

    analyseExpression(node->left.get(), ctx);
    analyseExpression(node->right.get(), ctx);
}

void SemanticAnalyser::analyseUnaryOp(UnaryOperationNode* node, Context& ctx) {
    analyseExpression(node->operand.get(), ctx);
}

void SemanticAnalyser::analyseArrayAccess(ArrayAccessNode* node, Context& ctx) {
    if (auto* p = dynamic_cast<NumberLiteralNode*>(node->index.get())) {
        Symbol* array = table.lookup(node->identifier);
        uint64_t array_size = array->array_size;
        if (p->value >= array_size) {
            error("array index " + std::to_string(p->value) + " is out of bounds for array '" + node->identifier + "' of size " + std::to_string(array_size), node->line, node->column);
        }

        else if (p->value < 0) {
            error("array index " + std::to_string(p->value) + " is negative for array '" + node->identifier + "'", node->line, node->column);
        }

        else if (ctx.initialised_vars.find(node->identifier) == ctx.initialised_vars.end()) {
            error("array '" + node->identifier + "' may be used before initialisation", node->line, node->column);
        }
    }

    analyseExpression(node->index.get(), ctx);
}

void SemanticAnalyser::analyseArrayLiteral(ArrayLiteralNode* node, Context& ctx) {
    for (auto& it : node->value) {
        analyseExpression(it.get(), ctx);
    }
}

bool SemanticAnalyser::allPathsReturn(std::vector<std::unique_ptr<ASTNode>>& nodes) {
    for (auto& it : nodes) {
        if (auto* p = dynamic_cast<ReturnNode*>(it.get())) {
            return true;
        }
        else if (auto* p = dynamic_cast<IfStatementNode*>(it.get())) {
            if (p->else_statement && allPathsReturn(p->else_statement->body) && allPathsReturn(p->body)) {
                return true;
            }
            else {
                continue;
            }
        }
        else if (auto* p = dynamic_cast<ElseStatementNode*>(it.get())) {
            if (allPathsReturn(p->body)) {
                return true;
            }
            continue;
        }
        else if (auto* p = dynamic_cast<WhileStatementNode*>(it.get())) {
            continue;
        }
        else if (auto* p = dynamic_cast<ForStatementNode*>(it.get())) {
            continue;
        }
        else {
            continue;
        }
    }
    return false;
}

void SemanticAnalyser::error(const std::string& message, int line, int column) {
    std::string err;
    err += "ERROR: " + message + "\n";
    err += "       at line " + std::to_string(line) + ", column " + std::to_string(column) + "\n";
 
    errors.push_back(err);
}

std::vector<std::string> SemanticAnalyser::getErrors() {
    return errors;
}
