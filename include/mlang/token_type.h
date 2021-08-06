#pragma once

typedef enum {
    // Type Keywords
    INT,
    FLOAT,
    BOOL,
    VOID,
    CLASS,
    STRUCTURE,
    CHAR,

    // Literal
    INT_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    TRUE,
    FLASE,

    // Module Keywords
    IMPORT,
    EXPORT,
    FROM,

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
    // TODO add more bitwise operators like shift left and right

    // Logic
    LOGIC_AND,
    LOGIC_OR,
    LOGIC_NOT,
    DOUBLE_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // BRACES
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_SQUARE_BRACKET,
    RIGHT_SQUARE_BRACKET,
    LEFT_CURLY_BRACKET,
    RIGHT_CURLY_BRACKET,

    // Quotes
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    BACK_TICK,

    // Punctuation
    COLON,
    SEMICOLON,
    DOT,
    SKINNY_ARROW,
    QUESTION,
    COMMA,

    // Other
    IDENTIFIER,
    UNKNOWN,
    END_OF_FILE,
} token_type_e;