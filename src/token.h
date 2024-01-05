#ifndef _MLANG_TOKEN_H_
#define _MLANG_TOKEN_H_

#include "common.h"

typedef enum token_kind token_kind_e;
typedef enum token_category token_category_e;

typedef struct token_t {
    union {
        int number_lexeme;
        const char* string_lexeme;
        double decimal_lexeme; 
        char char_lexeme;  
    };
    token_kind_e kind;
    token_category_e category;
    location_t location;
} token_t;

// token_t* token_create(const char*, token_kind_e, token_category_e, const char*, uint64_t, uint64_t);
// void token_free(token_t*);

bool token_init(token_t*, const void*, token_kind_e, token_category_e, const char*, uint64_t, uint64_t);

int token_print(token_t*);

enum token_kind {
    // Type Keywords
    TK_CHAR,
    TK_INT,
    TK_FLOAT,
    TK_BOOL,
    TK_VOID,
    TK_STRING,
    TK_STRUCTURE,
    TK_CLASS,
    TK_ENUM,
    TK_UNION,   

    // Privacy
    TK_PUBLIC,
    TK_PRIVATE,
    TK_PROTECTED,
    TK_READONLY,

    // Type / Function Modifiers
    TK_STATIC,
    TK_INLINE,
    TK_CONST,

    // Size Modifier
    TK_SHORT,
    TK_LONG,

    TK_SIGNED,
    TK_UNSIGNED,

    // Literal
    TK_INT_LITERAL,
    TK_FLOAT_LITERAL,
    TK_CHAR_LITERAL,
    TK_STRING_LITERAL,
    TK_TRUE,
    TK_FLASE,

    // Module Keywords
    TK_IMPORT,
    TK_EXPORT,
    TK_FROM,

    // Conditional Keywords
    TK_IF,
    TK_ELSE,
    TK_SWITKH,
    TK_CASE,
    TK_DEFAULT,
    TK_BREAK,
    TK_CONTINUE,
    TK_RETURN,
    TK_FOR,
    TK_WHILE,
    TK_DO,
    TK_GOTO,

    // Other Keywords
    TK_TYPE,

    // Math Operators
    TK_ADD,
    TK_SUB,
    TK_MULT,
    TK_DIV,
    TK_MOD,
    TK_ASSIGN,
    TK_ADD_ASSIGN,
    TK_SUB_ASSIGN,
    TK_MULT_ASSIGN,
    TK_DIV_ASSIGN,
    TK_MOD_ASSIGN,
    TK_INC,
    TK_DEC,

    // Bitwise
    TK_BIT_AND,
    TK_BIT_OR,
    TK_BIT_XOR,
    TK_BIT_NOT,
    TK_BIT_SHIFT_LEFT,
    TK_BIT_SHIFT_RIGHT,
    TK_BIT_AND_ASSIGN,
    TK_BIT_OR_ASSIGN,
    TK_BIT_XOR_ASSIGN,
    TK_BIT_SHIFT_LEFT_ASSIGN,
    TK_BIT_SHIFT_RIGHT_ASSIGN,

    // Logic
    TK_LOGIC_AND,
    TK_LOGIC_OR,
    TK_LOGIC_NOT,
    TK_DOUBLE_EQUAL,
    TK_GREATER,
    TK_GREATER_EQUAL,
    TK_LESS,
    TK_LESS_EQUAL,
    TK_NOT_EQUAL,

    // BRACES
    TK_LEFT_PAREN,
    TK_RIGHT_PAREN,
    TK_LEFT_SQUARE_BRACKET,
    TK_RIGHT_SQUARE_BRACKET,
    TK_LEFT_CURLY_BRACKET,
    TK_RIGHT_CURLY_BRACKET,

    // Quotes
    TK_DOUBLE_QUOTE,
    TK_SINGLE_QUOTE,
    TK_BACK_TICK,

    // Punctuation
    TK_COLON,
    TK_SEMICOLON,
    TK_DOT,
    TK_SKINNY_ARROW,
    TK_QUESTION,
    TK_COMMA,

    TK_IDENTIFIER,
    TK_END_OF_FILE,
    TK_UNKNOWN,
    TK_MAX_COUNT
};

enum token_category {
    TC_IDENTIFIER,
    TC_KEYWORD,
    TC_OPERATOR,
    TC_LITERAL,

    TC_UNKNOWN,
    TC_MAX_COUNT
};

#endif // _MLANG_TOKEN_H_