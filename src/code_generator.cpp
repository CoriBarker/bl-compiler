#include "code_generator.hpp"

CodeGenerator::CodeGenerator(SymbolTable& table) : table(table) {}

void CodeGenerator::generate(ProgramNode* node, const std::string& filename) {
    output.open(filename);
    label_counter = 0;

    output << ".intel_syntax noprefix\n";

    for (auto& func : node->function_declarations) {
        output << ".global " << func->identifier << "\n";
    }

    output << ".global _start\n";

    output << ".text\n\n";

    for (auto& func : node->function_declarations) {
        generateFunction(func.get());
    }

    emitLabel("_start");
    emit("call main");
    emit("mov rdi, rax");
    emit("mov rax, 60");
    emit("syscall");

    output.close();
}

void CodeGenerator::emit(const std::string& line) {
    output << "    " << line << "\n";
}

void CodeGenerator::emitLabel(const std::string& label) {
    
    output << label << ":\n";
}

std::string CodeGenerator::newLabel(const std::string& prefix) {
    return "." + prefix + "_" + std::to_string(label_counter++);
}

void CodeGenerator::buildOffsetMap(const std::string& function_name) {
    offsets.clear();
    current_offset = 0;

    for (auto& pair : table.symbols) {
        const Symbol& symbol = pair.second;

        if (symbol.kind == SymbolKind::FUNCTION) continue;

        std::string prefix = function_name + "::";
        if (symbol.qualified_name.substr(0, prefix.size()) != prefix) continue;

        current_offset -= 8;
        offsets[symbol.name] = current_offset;
    }
}

int CodeGenerator::getOffset(const std::string& name) {
    auto it = offsets.find(name);

    if (it == offsets.end()) return 0;

    return it->second;
}

void CodeGenerator::generateFunction(FunctionDeclarationNode* node) {
    return_label = newLabel("return");
    current_function = node->identifier;

    buildOffsetMap(current_function);
    emitLabel(current_function);

    emit("push rbp");
    emit("mov rbp, rsp");
    int stack_size = ((-current_offset + 15) / 16) * 16;
    emit("sub rsp, " + std::to_string(stack_size));

    static const std::vector<std::string> arg_regs = {
        "rdi", "rsi", "rdx", "rcx", "r8", "r9"
    };

    for (size_t i = 0; i < node->parameters.size(); i++) {
        emit("mov QWORD PTR [rbp + " + std::to_string(getOffset(node->parameters[i]->identifier)) + "], " + arg_regs[i]);
    }

    for (int i = 0; i < (int)node->body.size(); i++) {
        generateStatement(node->body[i].get());
    }

    emitLabel(return_label);
    emit("mov rsp, rbp");
    emit("pop rbp");
    emit("ret");
}

void CodeGenerator::generateStatement(ASTNode* node) {
    if (auto* p = dynamic_cast<VariableDeclarationNode*>(node)) {
        generateVariableDeclaration(p);
    }

    else if (auto* p = dynamic_cast<VariableAssignmentNode*>(node)) {
        generateVariableAssignment(p);
    }

    else if (auto* p = dynamic_cast<IfStatementNode*>(node)) {
        generateIf(p);
    }

    else if (auto* p = dynamic_cast<WhileStatementNode*>(node)) {
        generateWhile(p);
    }

    else if (auto* p = dynamic_cast<ForStatementNode*>(node)) {
        generateFor(p);
    }

    else if (auto* p = dynamic_cast<ReturnNode*>(node)) {
        generateReturn(p);
    }

    else if (auto* p = dynamic_cast<ForInitNode*>(node)) {
        if (p->type != Type::VOID) {
            if (p->expression) {
                generateExpression(p->expression.get());
                int offset = getOffset(p->identifier);
                emit("mov QWORD PTR [rbp + " + std::to_string(offset) + "], rax");
            }
        }

        else {
            if (p->expression) {
                generateExpression(p->expression.get());
                int offset = getOffset(p->identifier);
                emit("mov QWORD PTR [rbp + " + std::to_string(offset) + "], rax"); 
            }
        }
    }
}

void CodeGenerator::generateVariableDeclaration(VariableDeclarationNode* node) {
    if (node->value) {
        generateExpression(node->value.get());
        int offset = getOffset(node->identifier);
        emit("mov QWORD PTR [rbp + " + std::to_string(offset) + "], rax");
    }
}

void CodeGenerator::generateVariableAssignment(VariableAssignmentNode* node) {
    generateExpression(node->value.get());
    int offset = getOffset(node->identifier);
    emit("mov QWORD PTR [rbp + " + std::to_string(offset) + "], rax");
}

void CodeGenerator::generateReturn(ReturnNode* node) {
    generateExpression(node->expression.get());
    emit("jmp " + return_label);
}

void CodeGenerator::generateIf(IfStatementNode* node) {
    std::string else_label = newLabel("else");
    std::string end_label = newLabel("endif");

    generateExpression(node->expression.get());
    emit("cmp rax, 0");

    if (node->else_statement) {
        emit("je " + else_label);
    }

    else {
        emit("je " + end_label);
    }

    for (auto& stmt : node->body) {
        generateStatement(stmt.get());
    }

    emit("jmp " + end_label);

    if (node->else_statement) {
        emitLabel(else_label);
        for (auto& stmt : node->else_statement->body) {
            generateStatement(stmt.get());
        }
    }

    emitLabel(end_label);
}

void CodeGenerator::generateWhile(WhileStatementNode* node) {
    std::string start = newLabel("while_start");
    std::string end = newLabel("while_end");

    emitLabel(start);
    generateExpression(node->expression.get());
    emit("cmp rax, 0");
    emit("je " + end);

    for (auto& stmt : node->body) {
        generateStatement(stmt.get());
    }

    emit("jmp " + start);
    emitLabel(end);
}

void CodeGenerator::generateFor(ForStatementNode* node) {
    std::string start = newLabel("for_start");
    std::string end = newLabel("for_end");

    if (node->init) generateStatement(node->init.get());

    emitLabel(start);

    if (node->condition) {
        generateExpression(node->condition.get());
        emit("cmp rax, 0");
        emit("je " + end);
    }

    for (auto& stmt : node->body) {
        generateStatement(stmt.get());
    }

    if (node->update) generateStatement(node->update.get());

    emit("jmp " + start);
    emitLabel(end);
}

void CodeGenerator::generateExpression(ASTNode* node) {
    if (auto* p = dynamic_cast<BinaryOperationNode*>(node)) {
        generateBinaryOp(p);
    }

    else if (auto* p = dynamic_cast<UnaryOperationNode*>(node)) {
        generateUnaryOp(p);
    }

    else if (auto* p = dynamic_cast<FunctionCallNode*>(node)) {
        generateFunctionCall(p);
    }

    else if (auto* p = dynamic_cast<IdentifierNode*>(node)) {
        int offset = getOffset(p->value);
        emit("mov rax, [rbp + " + std::to_string(offset) + "]");
    }

    else if (auto* p = dynamic_cast<NumberLiteralNode*>(node)) {
        emit("mov rax, " + std::to_string(p->value));
    }
}

void CodeGenerator::generateBinaryOp(BinaryOperationNode* node) {
    generateExpression(node->left.get());
    emit("push rax");

    generateExpression(node->right.get());
    emit("mov rcx, rax");

    emit("pop rax");

    if (node->operation == "+") {
        emit("add rax, rcx");
    }

    else if (node->operation == "-") {
        emit("sub rax, rcx");
    }

    else if (node->operation == "*") {
        emit("imul rax, rcx");
    }

    else if (node->operation == "/") {
        emit("cqo");
        emit("idiv rcx");
    }

    else if (node->operation == "%") {
        emit("cqo");
        emit("idiv rcx");
        emit("mov rax, rdx");
    }

    else if (node->operation == "==") {
        emit("cmp rax, rcx");
        emit("sete al");
        emit("movzx rax, al");
    }

    else if (node->operation == "<") {
        emit("cmp rax, rcx");
        emit("setl al");
        emit("movzx rax, al");
    }

    else if (node->operation == ">") {
        emit("cmp rax, rcx");
        emit("setg al");
        emit("movzx rax, al");
    }

    else if (node->operation == "<=") {
        emit("cmp rax, rcx");
        emit("setle al");
        emit("movzx rax, al");
    }

    else if (node->operation == ">=") {
        emit("cmp rax, rcx");
        emit("setge al");
        emit("movzx rax, al");
    }

    else if (node->operation == "&&") {
        std::string false_label = newLabel("and_false");
        std::string end_label = newLabel("and_end");
        
        emit("cmp rax, 0");
        emit("je " + false_label);
        
        emit("cmp rcx, 0");
        emit("je " + false_label);
        
        emit("mov rax, 1");
        emit("jmp " + end_label);
        
        emitLabel(false_label);
        emit("mov rax, 0");
        
        emitLabel(end_label);
    }

    else if (node->operation == "||") {
        emit("or rax, rcx");
        emit("setne al");
        emit("movzx rax, al");
    }
}

void CodeGenerator::generateUnaryOp(UnaryOperationNode* node) {
    generateExpression(node->operand.get());

    if (node->operation == "-") {
        emit("neg rax");
    }

    else if (node->operation == "!") {
        emit("cmp rax, 0");
        emit("sete al");
        emit("movzx rax, al");
    }
}

void CodeGenerator::generateFunctionCall(FunctionCallNode* node) {
    static const std::vector<std::string> arg_regs = {
        "rdi", "rsi", "rdx", "rcx", "r8", "r9"
    };

    for (size_t i = 0; i < node->arguments.size(); i++) {
        generateExpression(node->arguments[i].get());
        emit("mov " + arg_regs[i] + ", rax");
    }

    emit("sub rsp, 8");
    emit("call " + node->identifier);
    emit("add rsp, 8");
}
