#include "semantic_analyzer.hpp"

SemanticAnalyzer::SemanticAnalyzer() : symbol_table(SymbolTable();), current_scope(nullptr), current_scope_level(-1) { }

SymbolTable SemanticAnalyzer::analyze(ASTNode* node) {
    Scope* global_scope = new Scope();

    for (auto function : node->function_declarations) {
	global_scope->addSymbol(analyzeFunction(function));
    }
}

Symbol* SemanticAnalyzer::analyzeVariable(ASTNode* node) {
    std::string identifier = node->identifier;
    Type data_type = node->type;
    int line = node->line;
    int column = node->column;

    return new Symbol(identifier, data_type, line, column);
}

Symbol* SemanticAnalyzer::analyzeFunction(ASTNode* node) {
    std::string identifier = node->identifier;
    Type return_type = node->return_type;
    std::vector<std::string> parameter_identifiers;
    std::vector<Type> parameter_types;
    int line = node->line;
    int column = node->column;

    for (auto parameter : node->parameters) {
	parameter_identifiers.push_back(parameter->identifier);
	parameter_types.push_back(parameter->type);
    }
    
    return new Symbol(identifer, return_type, parameter_identifiers, parameter_types, line, column);
}
