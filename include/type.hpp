#pragma once

#define TYPES \
    X(INTEGER_LITERAL) \
    X(INT8) \
    X(INT16) \
    X(INT32) \
    X(INT64) \
    X(UINT8) \
    X(UINT16) \
    X(UINT32) \
    X(UINT64) \
    X(CHAR) \
    X(BOOL) \
    X(STRING) \
    X(FLOAT) \
    X(DOUBLE) \
    X(VOID)

enum class Type {
#define X(name) name,
    TYPES
#undef X
};
