#include "semantic/symbol.hpp"

Symbol::Symbol(std::string name, SymbolType type, std::string data_type, int scope) : symbol_name(name), symbol_type(type), data_type(data_type), scope_level(scope_level) {}

std::string Symbool::getName() {
    return name;
}

SymbolType Symbol::getSymbolType() {
    return symbol_type;
}

std::string Symbol::getDataType() {
    return symbol_data_type;
}

int Symbol::getSymbolLevel() {
    return scope_level;
}

bool Symbol::isInitialized() {
    return is_initialized;
}

void Symbol::setInitialized(bool init) {
    is_initialized = init;
}

bool Symbol::isParameter() {
    return is_parameter();
}

void Symbol::setParameter(bool is_param) {
    is_parameter = is_param;
}

std::vector<std::string> Symbol::getParameterTypes() {
    return paremeter_types;
}

void Symbol::setParameterTypes(std::vector<std::string> types) {
    parameter_types = types;
}

std::string Symbol::getReturnType() {
    return return_type;
}

void Symbol::setReturnType(std::string type) {
    return_type = type;
}

int Symbol::getLineDeclared() {
    return line_declared;
}

void Symbol::setLineDeclared(int line) {
    line_declared = line;
}

std::string Symbol::toString() const {
    std::string result = "";
    
    // Symbol name and basic info
    result += "Symbol: " + name + "\n";
    
    // Symbol type (VARIABLE, FUNCTION, PARAMETER)
    result += "  Type: ";
    switch (symbol_type) {
        case SymbolType::VARIABLE:
            result += "VARIABLE\n";
            break;
        case SymbolType::FUNCTION:
            result += "FUNCTION\n";
            break;
        case SymbolType::PARAMETER:
            result += "PARAMETER\n";
            break;
        default:
            result += "UNKNOWN\n";
            break;
    }
    
    // Data type
    result += "  Data Type: " + data_type + "\n";
    
    // Scope level
    result += "  Scope Level: " + std::to_string(scope_level) + "\n";
    
    // Initialization status (only relevant for variables)
    if (symbol_type == SymbolType::VARIABLE || symbol_type == SymbolType::PARAMETER) {
        result += "  Initialized: " + std::string(is_initialized ? "true" : "false") + "\n";
    }
    
    // Parameter flag
    if (is_parameter) {
        result += "  Is Parameter: true\n";
    }
    
    // Function-specific information
    if (symbol_type == SymbolType::FUNCTION) {
        result += "  Return Type: " + return_type + "\n";
        
        result += "  Parameters: [";
        for (size_t i = 0; i < parameter_types.size(); i++) {
            result += parameter_types[i];
            if (i < parameter_types.size() - 1) {
                result += ", ";
            }
        }
        result += "]\n";
        
        result += "  Parameter Count: " + std::to_string(parameter_types.size()) + "\n";
    }
    
    // Declaration location
    if (line_declared > 0) {
        result += "  Declared at: line " + std::to_string(line_declared);
        if (column_declared > 0) {
            result += ", column " + std::to_string(column_declared);
        }
        result += "\n";
    }
    
    return result;
}
