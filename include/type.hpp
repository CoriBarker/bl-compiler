#pragma once

enum struct TypeKind {
    INT,
    CHAR,
    BOOL,
    STRING,
    FLOAT,
    DOUBLE,
    VOID,
    FUNCTION,
};

struct Type {
    TypeKind kind;

    Type* return_type = nullptr;
    std::vector<Type*> param_types;

    static Type makeInt() { return { TypeKind::INT }; }
    static Type makeChar() { return { TypeKind::CHAR }; }
    static Type makeBool() { return { TypeKind::BOOL }; }
    static Type makeString() { return { TypeKind::STRING }; }
    static Type makeFloat() { return { TypeKind::FLOAT }; }
    static Type makeDouble() { return { TypeKind::DOUBLE }; }
    static Type makeVoid() { return { TypeKind::VOID }; }

    static Type makeFunction(Type* return_type, std::vector<Type*> param_types) {
	Type t;
	t.kind = TypeKind::FUNCTION;
	t.return_type = return_type;
	t.param_types = std::move(param_types);
	return t;
    };
};
