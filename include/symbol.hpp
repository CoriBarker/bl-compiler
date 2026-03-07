#pragma once

class Symbol {
public:
    explicit Symbol();

private:
    std::string identifier;
    Type type;
    std::variant<int, bool, std::string> value;
};

Symbol::Symbol(std::string identifier, Type type, std::variant<int, bool, std::string> value) : identifier(identifier), type(type), value(value) { } 
