#pragma once

#include <string>
#include <vector>

#include "symbol_type.hpp"

class Symbol {
public:
    explicit Symbol(std::string name, SymbolType type, std::string data_type, int scope_level);
    std::string getName();
    SymbolType getSymbolType();
    std::string getDataType();
    int getScopeLevel();
    bool isInitialized();
    void setInitialized(bool init);
    bool isParameter();
    void setParameter(bool is_param);
    std::vector<std::string> getParameterTypes();
    void setParameterTypes(std::vector<std::string> types);
    std::string getReturnType();
    void setReturnType(std::string type);
    int getLineDeclared();
    void setLineDeclared(int line);
    std::string toString();

private:
    std::string symbol_name;
    SymbolType symbol_type;
    std::string data_type;
    int scope_level;
    bool is_initialized;
    bool is_parameter;
    std::vector<std::string> parameter_types;
    std::string return_type;
    int line_declared;
    int column_declared;
};
