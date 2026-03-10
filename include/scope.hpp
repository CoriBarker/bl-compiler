#pragma once

class Scope {
public:
    explicit Scope(int scope_level);

private:
    std::vector<Symbol> symbols;
    int scope_level;
};
