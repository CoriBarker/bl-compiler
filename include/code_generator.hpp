#pragma once

#include "symbol_table.hpp"
#include "ast_node.hpp"

#include <fstream>
#include <unordered_map>
#include <string>

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

    void emit(const std::string& line);
    void emitLabel(const std::string& label);
    std::string newLabel(const std::string& prefix);

    void buildOffsetMap(const std::string& function_name);
    int getOffset(const std::string& name);

    void generateFunction(FunctionDeclarationNode* node);
    void generateStatement(ASTNode* node);
    void generateVariableDeclaration(VariableDeclarationNode* node);
    void generateVariableAssignment(VariableAssignmentNode* node);
    void generateReturn(ReturnNode* node);
    void generateIf(IfStatementNode* node);
    void generateWhile(WhileStatementNode* node);
    void generateFor(ForStatementNode* node);
    void generateExpression(ASTNode* node);
    void generateBinaryOp(BinaryOperationNode* node);
    void generateUnaryOp(UnaryOperationNode* node);
    void generateFunctionCall(FunctionCallNode* node);
};
