#pragma once

class Scope {
public:
    explicit Scope(int scope_level);

    add symbol
private:
    std::vector<Symbol> symbols;
    int scope_level;
};
