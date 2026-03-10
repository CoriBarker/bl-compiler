#include "semantic_analyzer.hpp"

SemanticAnalyzer::SemanticAnalyzer() : symbol_table(), current_scope(nullptr), current_scope_level(-1) { }

SymbolTable SemanticAnalyzer::analyze(ASTNode* node) {
    for (auto function : node->function_delcarations) {
	analyzeFunction(function);
    }
}

ASTNode* SemanticAnalyzer::advance() {
    if (dynamic_cast<ProgramNode*>(node)) {
	
    }
}

ASTNode* SemanticAnalyzer::peek() { }

Symbol* SemanticAnalyzer::analyzeVariable(ASTNode* node) { }

Symbol* SemanticAnalyzer::analyzeFunctionDeclaration(ASTNode* node) {
    std::string identifier = node->identifier;
    symbol_type = SymbolType::FUNCTION_DECLARATION;
}
