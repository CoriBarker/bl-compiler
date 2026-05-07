#include "code_generator.hpp"

#include <iostream>

CodeGenerator::CodeGenerator(SymbolTable& table) : table(table) {}

void CodeGenerator::generate(ProgramNode* node, const std::string& filename) {
    output.open(filename);
    label_counter = 0;

    output << ".intel_syntax noprefix\n"; // Use intel syntax

    output << ".section .data\n";
    output << "itoa_buffer:\n"; // Buffer for type conversion
    emit(".space 32");

    bool has_main = false;
    for (const auto& func : node->function_declarations) {
        if (func->identifier == "main") {    // Check for main function
            has_main = true;
            break;
        }
    }

    for (auto& func : node->function_declarations) {
        output << ".global " << func->identifier << "\n";
    }

    output << ".global _start\n";
    output << ".text\n\n";
    if (!has_main) {
        output << ".global main\n";
    }

    generateBuiltIns();
    output << "\n";

    for (auto& func : node->function_declarations) {
        generateFunction(func.get());
    }

    if (!has_main && !lib_mode) {
        emitLabel("main");
        emit("mov rax, 0");        // return 0
        emit("ret");
        output << "\n";
    }

    if (!lib_mode) {    // If compiling a library no _start label is necessary
        emitLabel("_start");
        emit("call main");
        emit("mov rdi, rax");
        emit("mov rax, 60");
        emit("syscall");
    }

    output << ".section .rodata\n";    // Read only data for strings
    output << ".int_fmt: .string \"%lld\\n\"\n";
    for (auto& s : rodata_strings) {
        output << s.label << ": .string \"" << s.value << "\"\n";
    }

    output.close();
}

void CodeGenerator::setLibraryMode(bool value) {
    lib_mode = value;
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

void CodeGenerator::buildOffsetMap(FunctionDeclarationNode* node) {
    offsets.clear();
    current_offset = 0;

    for (auto& param : node->parameters) {
        allocate(param->identifier, getSizeInBytes(param->type));
    }
    
    for (auto& stmt : node->body) {
        walk(stmt.get());
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

    buildOffsetMap(node);
    emitLabel(current_function);

    // Function prologue
    emit("push rbp");
    emit("mov rbp, rsp");
    int stack_size = ((-current_offset + 15) / 16) * 16;
    emit("sub rsp, " + std::to_string(stack_size));

    // Different registers for different argument sizes
    static const std::vector<std::string> arg_regs_64 = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };
    static const std::vector<std::string> arg_regs_32 = { "edi", "esi", "edx", "ecx", "r8d", "r9d" };
    static const std::vector<std::string> arg_regs_16 = { "di",  "si",  "dx",  "cx",  "r8w", "r9w" };
    static const std::vector<std::string> arg_regs_8  = { "dil", "sil", "dl",  "cl",  "r8b", "r9b" };
    
    // Get correct register for the argument size
    for (size_t i = 0; i < node->parameters.size(); i++) {
        Type t = node->parameters[i]->type;
        std::string keyword = getSizeKeyword(t);
        std::string reg;
        switch (getSizeInBytes(t)) {
        case 1: reg = arg_regs_8[i];  break;
        case 2: reg = arg_regs_16[i]; break;
        case 4: reg = arg_regs_32[i]; break;
        default: reg = arg_regs_64[i]; break;
        }
        emit("mov " + keyword + " PTR [rbp + " + std::to_string(getOffset(node->parameters[i]->identifier)) + "], " + reg);
    }

    for (int i = 0; i < (int)node->body.size(); i++) {
        generateStatement(node->body[i].get());
    }

    // Function epilogue
    emitLabel(return_label);
    emit("mov rsp, rbp");
    emit("pop rbp");
    emit("ret");
}

void CodeGenerator::generateStatement(ASTNode* node) {
    if (auto* p = dynamic_cast<VariableDeclarationNode*>(node)) {
        generateVariableDeclaration(p);
    }

    else if (auto* p = dynamic_cast<ArrayDeclarationNode*>(node)) {
        generateArrayDeclaration(p);
    }

    else if (auto* p = dynamic_cast<ArrayAssignmentNode*>(node)) {
        generateArrayAssignment(p);
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

    else if (auto* p = dynamic_cast<FunctionCallNode*>(node)) {
        generateFunctionCall(p);
    }

    else if (auto* p = dynamic_cast<ForInitNode*>(node)) {
        if (p->type != Type::VOID) {
            if (p->expression) {
                generateExpression(p->expression.get());
                int offset = getOffset(p->identifier);
                std::string keyword = getSizeKeyword(p->type);
                emit("mov " + keyword + " PTR [rbp + " + std::to_string(offset) + "], " + getRegister(p->type));
            }
        }

        else {
            if (p->expression) {
                generateExpression(p->expression.get());
                int offset = getOffset(p->identifier);
                Symbol* s = table.lookupName(p->identifier);
                std::string keyword = getSizeKeyword(s->type);
                emit("mov " + keyword + " PTR [rbp + " + std::to_string(offset) + "], " + getRegister(s->type)); 
            }
        }
    }
}

void CodeGenerator::generateVariableDeclaration(VariableDeclarationNode* node) {
    if (node->value) {
        generateExpression(node->value.get());
        int offset = getOffset(node->identifier);
        Type t = node->type;
        std::string size = getSizeKeyword(t);
        std::string reg = getRegister(t);

        emit("mov " + size + " PTR [rbp + " + std::to_string(offset) + "], " + reg); // Store value
    }
}

void CodeGenerator::generateArrayDeclaration(ArrayDeclarationNode* node) {
    int base = getOffset(node->identifier);
    if (node->elements) {
        auto* literal = dynamic_cast<ArrayLiteralNode*>(node->elements.get());
        for (int i = 0; i < (int)literal->value.size(); i++) {
            generateExpression(literal->value[i].get());
            int size = getSizeInBytes(node->element_type);
            std::string keyword = getSizeKeyword(node->element_type);
            emit("mov " + keyword + " PTR [rbp + " + std::to_string(base - i * size) + "], " + getRegister(node->element_type)); // Store values in each byte from the base pointer
        }
    }
    else {
        auto* size_literal = dynamic_cast<NumberLiteralNode*>(node->size_expr.get());
        for (int i = 0; i < size_literal->value; i++) {
            int size = getSizeInBytes(node->element_type);
            std::string keyword = getSizeKeyword(node->element_type);
            emit("mov " + keyword + " PTR [rbp + " + std::to_string(base - i * size) + "], 0"); // Zero initialise the array
        }
    }
}

void CodeGenerator::generateVariableAssignment(VariableAssignmentNode* node) {
    generateExpression(node->value.get());
    int offset = getOffset(node->identifier);
    Symbol* s = table.lookupName(node->identifier); // Lookup variable to find its type
    std::string keyword = getSizeKeyword(s->type);
    emit("mov " + keyword + " PTR [rbp + " + std::to_string(offset) + "], " + getRegister(s->type)); // Store value
}

void CodeGenerator::generateArrayAssignment(ArrayAssignmentNode* node) {
    int base = getOffset(node->identifier);
    Symbol* s = table.lookupName(node->identifier);

    std::string keyword = getSizeKeyword(s->type);
    int size = getSizeInBytes(s->type);

    std::string val_reg = getRegister(s->type);

    // value → rax
    generateExpression(node->value.get());
    emit("push rax");

    // index → rax
    generateExpression(node->index.get());
    emit("imul rax, rax, " + std::to_string(size));

    // restore value → rdx
    emit("pop rdx");

    emit("lea rcx, [rbp + " + std::to_string(base) + "]");
    emit("mov " + keyword + " PTR [rcx + rax], " + val_reg);
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
        Symbol* s = table.lookupName(p->value);
        Type t = s->type;

        switch (t) {
        case Type::INT8:
            emit("movsx rax, BYTE PTR [rbp + " + std::to_string(offset) + "]");
            break;
            
        case Type::UINT8:
            emit("movzx rax, BYTE PTR [rbp + " + std::to_string(offset) + "]");
            break;
            
        case Type::INT16:
            emit("movsx rax, WORD PTR [rbp + " + std::to_string(offset) + "]");
            break;
            
        case Type::UINT16:
            emit("movzx rax, WORD PTR [rbp + " + std::to_string(offset) + "]");
            break;
            
        case Type::INT32:
            emit("mov eax, DWORD PTR [rbp + " + std::to_string(offset) + "]");
            break;
            
        case Type::UINT32:
            emit("mov eax, DWORD PTR [rbp + " + std::to_string(offset) + "]");
            break;
            
        case Type::INT64:
        case Type::UINT64:
            emit("mov rax, QWORD PTR [rbp + " + std::to_string(offset) + "]");
            break;

        case Type::STRING:
            emit("mov rax, QWORD PTR [rbp + " + std::to_string(offset) + "]");
            break;
        }
        
    }

    else if (auto* p = dynamic_cast<NumberLiteralNode*>(node)) {
        emit("mov rax, " + std::to_string(p->value));
    }

    else if (auto* p = dynamic_cast<StringLiteralNode*>(node)) {
        std::string label = ".str" + std::to_string(string_counter++);
        rodata_strings.push_back({ label, p->value });
        emit("lea rax, [rip + " + label + "]");
    }

    else if (auto* p = dynamic_cast<ArrayAccessNode*>(node)) {
        generateArrayAccess(p);
    }

    else if (auto* p = dynamic_cast<CastNode*>(node)) {
        generateExpression(p->expression.get());
        if (p->type == Type::STRING) {
            emit("mov rdi, rax");
            emit("call itoa");
        }
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

    if (node->identifier == "syscall") {
        static const std::vector<std::string> syscall_regs = {
            "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9"
        };
        // push all args onto stack first
        for (size_t i = 0; i < node->arguments.size(); i++) {
            generateExpression(node->arguments[i].get());
            emit("push rax");
        }
        // pop in reverse into correct registers
        for (int i = node->arguments.size() - 1; i >= 0; i--) {
            emit("pop " + syscall_regs[i]);
        }
        emit("syscall");
        return;
    }

    if (node->identifier == "strlen") {
        for (size_t i = 0; i < node->arguments.size(); i++) {
            generateExpression(node->arguments[i].get());
            emit("mov " + arg_regs[i] + ", rax");
        }
        emit("call strlen");
        return;
    }

    for (size_t i = 0; i < node->arguments.size(); i++) {
        generateExpression(node->arguments[i].get());
        emit("mov " + arg_regs[i] + ", rax");
    }

    emit("sub rsp, 8");
    emit("call " + node->identifier);
    emit("add rsp, 8");
}

void CodeGenerator::generateArrayAccess(ArrayAccessNode* node) {
    int base = getOffset(node->identifier);
    Symbol* s = table.lookupName(node->identifier);
    int element_size = getSizeInBytes(s->type);

    generateExpression(node->index.get());
    emit("imul rax, rax, " + std::to_string(element_size));
    emit("lea rcx, [rbp + " + std::to_string(base) + "]");
    switch (s->type) {
    case Type::INT8:
        emit("movsx rax, BYTE PTR [rcx + rax]");
        break;
        
    case Type::UINT8:
        emit("movzx rax, BYTE PTR [rcx + rax]");
        break;
        
    case Type::INT16:
        emit("movsx rax, WORD PTR [rcx + rax]");
        break;
        
    case Type::UINT16:
        emit("movzx rax, WORD PTR [rcx + rax]");
        break;
        
    case Type::INT32:
    case Type::UINT32:
        emit("mov eax, DWORD PTR [rcx + rax]");
        break;
        
    case Type::INT64:
    case Type::UINT64:
    case Type::STRING:
        emit("mov rax, QWORD PTR [rcx + rax]");
        break;
    }
}

void CodeGenerator::generateBuiltIns() {
    emitLabel("strlen");
    std::string loop = newLabel("loop");
    std::string done = newLabel("done");
    emit("xor rax, rax");
    emitLabel(loop);
    emit("movzx rcx, BYTE PTR [rdi + rax]");
    emit("test rcx, rcx");
    emit("jz " + done);
    emit("inc rax");
    emit("jmp " + loop);
    emitLabel(done);
    emit("ret");

    emitLabel("itoa");
    std::string check_neg = newLabel("check_neg");
    std::string convert   = newLabel("convert");
    std::string loop_1    = newLabel("loop");
    std::string done_1    = newLabel("done");
    emit("push rbp");
    emit("mov rbp, rsp");
    emit("sub rsp, 32");
    emit("mov rax, rdi");
    emit("lea rsi, [rip + itoa_buffer + 31]");
    emit("mov BYTE PTR [rsi], 0");
    emit("mov rbx, 10");
    emit("xor rcx, rcx");
    emit("cmp rax, 0");
    emit("jne " + check_neg);
    emit("dec rsi");
    emit("mov BYTE PTR [rsi], '0'");
    emit("mov rax, rsi");
    emit("jmp " + done_1);
    emitLabel(check_neg);
    emit("jge " + convert);
    emit("neg rax");
    emit("mov rcx, 1");
    emitLabel(convert);
    emitLabel(loop_1);
    emit("xor rdx, rdx");
    emit("div rbx");
    emit("add dl, '0'");
    emit("dec rsi");
    emit("mov BYTE PTR [rsi], dl");
    emit("test rax, rax");
    emit("jnz " + loop_1);
    emit("cmp rcx, 0");
    emit("je " + done_1);
    emit("dec rsi");
    emit("mov BYTE PTR [rsi], '-'");
    emitLabel(done_1);
    emit("mov rax, rsi");
    emit("leave");
    emit("ret");
}

std::string CodeGenerator::getSizeKeyword(Type t) {
    switch (t) {
    case Type::INT8:
    case Type::UINT8:  return "BYTE";
    case Type::INT16:
    case Type::UINT16: return "WORD";
    case Type::INT32:
    case Type::UINT32: return "DWORD";
    case Type::INT64:
    case Type::UINT64: return "QWORD";
    default: return "QWORD";
    }
}

std::string CodeGenerator::getRegister(Type t) {
    switch (t) {
    case Type::INT8:
    case Type::UINT8:  return "al";
    case Type::INT16:
    case Type::UINT16: return "ax";
    case Type::INT32:
    case Type::UINT32: return "eax";
    case Type::INT64:
    case Type::UINT64: return "rax";
    default: return "rax";
    }
}

std::string CodeGenerator::getRegisterFromRDX(Type t) {
    switch (t) {
    case Type::INT8:
    case Type::UINT8:  return "dl";
    case Type::INT16:
    case Type::UINT16: return "dx";
    case Type::INT32:
    case Type::UINT32: return "edx";
    case Type::INT64:
    case Type::UINT64: return "rdx";
    default: return "rdx";
    }
}

int CodeGenerator::getSizeInBytes(Type t) {
    switch(t) {
    case Type::INT8: case Type::UINT8: return 1;
    case Type::INT16: case Type::UINT16: return 2;
    case Type::INT32: case Type::UINT32: return 4;
    case Type::INT64: case Type::UINT64: return 8;
    default: return 8;
    }
}


void CodeGenerator::walk(ASTNode* n) {
    if (auto* v = dynamic_cast<VariableDeclarationNode*>(n)) {
        allocate(v->identifier, getSizeInBytes(v->type));
    }
    
    else if (auto* a = dynamic_cast<ArrayDeclarationNode*>(n)) {
        int size = 1;
        if (auto* lit = dynamic_cast<NumberLiteralNode*>(a->size_expr.get())) {
            size = lit->value;
        }
        allocate(a->identifier, size * getSizeInBytes(a->element_type));
    }
    
    else if (auto* f = dynamic_cast<ForStatementNode*>(n)) {
        if (f->init) {
            walk(f->init.get());
        }
        for (auto& s : f->body) {
            walk(s.get());
        }
    }
    
    else if (auto* i = dynamic_cast<ForInitNode*>(n)) {
        if (!i->identifier.empty() && i->type != Type::VOID) {
            allocate(i->identifier, getSizeInBytes(i->type));
        }
    }
    
    else if (auto* b = dynamic_cast<IfStatementNode*>(n)) {
        for (auto& s : b->body) walk(s.get());
        if (b->else_statement) {
            for (auto& s : b->else_statement->body) walk(s.get());
        }
    }
    
    else if (auto* w = dynamic_cast<WhileStatementNode*>(n)) {
        for (auto& s : w->body) walk(s.get());
    }
}


void CodeGenerator::allocate(const std::string& name, int size) {
    StackSlot s;
    Symbol* sym = table.lookupName(name);
    s.size = size;
    s.type = sym->type;

    int alignment = getAlignment(s.type);
    current_offset = alignDown(current_offset, alignment);
    current_offset -= size;
    s.offset = current_offset;
    offsets[name] = s.offset;
}

int CodeGenerator::alignDown(int offset, int alignment) {
    int mod = -(offset) % alignment;
    if (mod == 0) return offset;
    return offset - (alignment - mod);
}

int CodeGenerator::getAlignment(Type t) {
    int size = getSizeInBytes(t);

    if (size >= 8) return 8;
    if (size >= 4) return 4;
    if (size >= 2) return 2;
    return 1;
}
