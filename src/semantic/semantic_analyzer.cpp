#include "semantic/semantic_analyzer.hpp"

SemanticAnalyzer::SemanticAnalyzer() : symobl_table(new SemanticTable), errors(std::vector<Error>), warnings(std::vector<Error>), current_function(nullptr), current_function_return_type(""), has_main_function(false) {}

bool SemanticAnalyzer::analyze(ASTNode* ast) {
    this->symbol_table.enterScope("global")
    this->visit(ast);
    this->validateMainFunction();

    if (has_main_function == false) {
        //error
        return false;
    }

    this->symbol_table.exitScope();
    return true;
}

std::vector<Error> SemanticAnalyzer::getErrors() {
    return this->errors;
}

std::vector<Error> SemanticAnalyzer::getWarnings() {
    return this->warnings;
}

bool SemanticAnalyzer::hasErrors() {
    return !this->errors.empty();
}

std::string SemanticAnalyzer::visit(ASTNode* ast) {

}
