#pragma once

#include "symbol_table.hpp"
#include "ast_node.hpp"

#include <fstream>
#include <unordered_map>
#include <string>

struct RodataString {
    std::string label;
    std::string value;
};

struct StackSlot {
    int offset;
    int size;
    Type type;
};

class CodeGenerator {
public:
    CodeGenerator(SymbolTable& table);
    void generate(ProgramNode* node, const std::string& filename);
    
private:
    SymbolTable& table;
    std::ofstream output;
    int label_counter;
    std::string current_function;
    std::unordered_map<std::string, int> offsets;
    int current_offset;
    std::string return_label;
    std::vector<RodataString> rodata_strings;
    int string_counter = 0;
    bool lib_mode = false;

    void setLibraryMode(bool value);

    void emit(const std::string& line);
    void emitLabel(const std::string& label);
    std::string newLabel(const std::string& prefix);

    void buildOffsetMap(FunctionDeclarationNode* node);
    int getOffset(const std::string& name);

    void generateFunction(FunctionDeclarationNode* node);
    void generateStatement(ASTNode* node);
    void generateVariableDeclaration(VariableDeclarationNode* node);
    void generateArrayDeclaration(ArrayDeclarationNode* node);
    void generateVariableAssignment(VariableAssignmentNode* node);
    void generateArrayAssignment(ArrayAssignmentNode* node);
    void generateReturn(ReturnNode* node);
    void generateIf(IfStatementNode* node);
    void generateWhile(WhileStatementNode* node);
    void generateFor(ForStatementNode* node);
    void generateExpression(ASTNode* node);
    void generateBinaryOp(BinaryOperationNode* node);
    void generateUnaryOp(UnaryOperationNode* node);
    void generateFunctionCall(FunctionCallNode* node);
    void generateArrayAccess(ArrayAccessNode* node);
    void generateBuiltIns();

    std::string getSizeKeyword(Type t);
    std::string getRegister(Type t);
    int getSizeInBytes(Type t);
    void walk(ASTNode* n);
    void allocate(const std::string& name, int size);
    int alignDown(int offset, int alignment);
    int getAlignment(Type t);
};
