#pragma once

typedef enum {
    // Type Keywords
    INT,
    FLOAT,
    BOOL,
    VOID,
    CLASS,
    STRUCTURE,

    // Module Keywords
    IMPORT,
    EXPORT,

    // Conditional Keywords
    IF,
    ELSE,
    SWITCH,
    CASE,
    DEFAULT,
    BREAK,
    CONTINUE,
    RETURN,

    // Math Operators
    ADD,
    SUB,
    MULT,
    DIV,
    MOD,
    ASSIGN,
    ADD_ASSIGN,
    SUB_ASSIGN,
    MULT_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    INC,
    DEC,

    // Bitwise
    BIT_AND,
    BIT_OR,
    BIT_XOR,
    BIT_NOT,

    // Logic
    LOGIC_AND,
    LOGIC_OR,
    LOGIC_NOT,

    // BRACES
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_SQUARE_BRACKET,
    RIGHT_SQUARE_BRACKET,
    LEFT_CURLY_BRACKET,
    TIGHT_CURLY_BRACKET,

    // Other
    UNKNOWN,
    END_OF_FILE,
} token_type_e;