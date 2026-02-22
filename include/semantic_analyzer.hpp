#pragma once

class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer();
    bool analyze(ASTNode* ast);
    std::vector<Error> getErrors();
    std::vector<Error> getWarnings();
    bool hasErrors();

    // Main visitor method
    std::string visit(ASTNode* ast);

    // Visitor methods for program structure
    std::string visitProgram(ProgramNode* node);
    std::string visitFunctionDecl(FunctionDeclNode* node);
    std::string visitParameter(ParameterNode*);

    // Visitor methods for statements
    std::string visitVarDeclaration(VarDeclNode* node);
    std::string visitAssignment(AssignmentNode* node);
    std::string visitIfStatement(IfStatementNode* node);
    std::string visitWhileStatement(WhileStatementNode* node);
    std::string visitForStatement(ForStatementNode* node);
    std::string visitReturnStatement(ReturnStatementNode* node);
    std::String visitExpressionStatement(ExpressionStatementNode* node);
    
    // Visitor methods for expressions
    std::string visitBinaryExpression(BinaryExprNode* node);
    std::string visitUnaryExpression(UnaryExprNode* node);
    std::string visitFunctionCall(FunctionCallNode* node);
    std::string visitIdentifier(IdentifierNode* node);
    std::string visitLiteral(LiteralNode* node);
    
    // Type checking helper methods
    bool checkTypeCompatibility(std::string target, std::string source);
    std::string inferBinaryOpType(std::string op, std::string left, std::string right);

private:
    SymbolTable symbol_table;
    std::vector<Error> errors;
    std::vector<Error> warnings;
    FunctionDeclNode* current_function;
    std::string current_function_return_type;
    bool has_main_function;
};
