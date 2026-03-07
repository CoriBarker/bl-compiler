#include "semantic_analyzer.hpp"

bool SemanticAnalyzer::analyze(ASTNode* ast) {
    symbol_table.enterScope("global")
    visit(ast);
    validateMainFunction();

    if (has_main_function == false) {
        //error
        return false;
    }

    symbol_table.exitScope();
    return true;
}

std::vector<Error> SemanticAnalyzer::getErrors() {
    return errors;
}

std::vector<Error> SemanticAnalyzer::getWarnings() {
    return warnings;
}

bool SemanticAnalyzer::hasErrors() {
    return !errors.empty();
}

std::string SemanticAnalyzer::visit(ASTNode* node) {
    if (ProgramNode* prog = dynamic_cast<ProgramNode*>(node)) {
        return visitProgram(prog);
    }

    else if (FunctionDeclNode* func = dynamic_cast<FunctionDeclNode*>(node)) {
        return visitFunctionDecl(func);
    }

    else if (ParameterNode* param = dynamic_cast<ParameterNode*>(node)) {
        return visitParameter(param);
    }

    else if (VarDeclNode* var = dynamic_cast<VarDeclNode*>(node)) {
        return visitVarDeclaration(var);
    }

    else if (AssignmentNode as = dynamic_cast<AssignmentNode>(node)) {
        return visitAssignment(as);
    }

    else if (IfStatementNode is = dynamic_cast<IfStatementNode>(node)) {
        return visitIfStatement(is);
    }

    else if (WhileStatementNode ws = dynamic_cast<WhileStatementNode>(node)) {
        return visitWhileStatement(ws);
    }

    else if (ForStatementNode fs = dynamic_cast<ForStatementNode>(node)) {
        return visitForStatement(fs);
    }

    else if (ReturnStatementNode rs = dynamic_cast<ReturnStatementNode>(node)) {
        return visitReturnStatement(rs);
    }

    else if (ExpressionStatementNode es = dynamic_cast<ExpressionStatementNode>(node)) {
        return visitExpressionStatement(es);
    }

    else if (BinaryExprNode bin = dynamic_cast<BinaryExprNode>(node)) {
        return visitBinaryExpression(bin);
    }

    else if (UnaryExprNode un = dynamic_cast<UnaryExprNode>(node)) {
        return visitUnaryExpression(un);
    }

    else if (FunctionCallNode func_call = dynamic_cast<FunctionCallNode>(node)) {
        return visitFunctionCall(func_call);
    }

    else if (IdentifierNode ident = dynamic_cast<IdentifierNode>(node)) {
        return visitIdentifier(ident);
    }

    else if (LiteralNode lit = dynamic_cast<LiteralNode>(node)) {
        return visitLiteral(lit);
    }
}

std::string visitProgram(ProgramNode* node){
    // get list of functions
    // visit each function
    // return

    std::vector<FunctionDeclarationNode*> functions = node->function_declarations;
    for (auto element : functions) {
	visitFunctionDeclaration(element);
    };

    return;
}

std::string visitFunctionDeclaration(FunctionDeclarationNode* node){
    // check for dupe name
    // create function symbol and insert into global scope
    // check if main function
    // enter function scope
    // process parameters
    // process function body
    // verify non-void functions have return statements
    // exit function scope and clear context
    // return
    
    current_function = node;

}

std::string visitParameter(ParameterNode*){
    // get parameter info from node
    // check for dupes
    // create parameter symbol
    // insert into current scope
    // return parameter type

}

std::string visitVariableDeclaration(VariableDeclarationNode* node){
    // get variable information
    // check for dupes
    // if initializer exists, check type
    // create variable symbol
    // insert into current scope
    // return

}

std::string visitAssignment(AssignmentNode* node){
    // get left side info
    // look up target variable
    // verify variable
    // analyze right side info
    // check type compatability
    // make var as initialized
    // return

}

std::string visitIfStatement(IfStatementNode* node){
    // analyze condition
    // verify condition is bool
    // analyze then branch
    // analyze else branch
    // return

}

std::string visitWhileStatement(WhileStatementNode* node){
    // analyze condition
    // verify condition is bool
    // analyze loop body
    // return

}

std::string visitForStatement(ForStatementNode* node){
    // enter new scope for loop
    // analyze initialization
    // analyze condition
    // analyze update statement
    // analyze loop body
    // exit for loop scope
    // return 

}

std::string visitReturnStatement(ReturnStatementNode* node){
    // check if inside function
    // get return expression
    // analyze return expression
    // validate return type matches function
    // return
    
}

std::string visitExpressionStatement(ExpressionStatementNode* node){
    // get the expression
    // analyze expression
    // return

}

std::string visitBinaryExpression(BinaryExprNode* node){
    // get operator and operands
    // analyze operands
    // infer result type based on operator
    // return result type

}

std::string visitUnaryExpression(UnaryExprNode* node){
    // get operator and operand
    // analyze operand
    // infer result type
    // return result type

}

std::string visitFunctionCall(FunctionCallNode* node){
    // get function name and args
    // look up function
    // verify its a function
    // get expected parameter information
    // check argument count
    // check each argument type
    // return functions return type

}

std::string visitIdentifier(IdentifierNode* node){
    // get identifier name
    // look up in symbol table
    // check if initialized
    // return symbol type
    
}

std::string visitLiteral(LiteralNode* node){
    // determine literal type
    // return appropriate type

}

bool checkTypeCompatibility(std::string target, std::string source){
    // determines if a value of source type can be used where target type is expected

}

std::string inferBinaryOpType(std::string op, std::string left, std::string right){

}
