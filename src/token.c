#include "token.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool token_init(token_t* token, const void* lexeme, token_kind_e kind, token_category_e category, const char* filename, uint64_t line, uint64_t column) {
    if(kind == TK_STRING_LITERAL || kind == TK_IDENTIFIER)
        token->string_lexeme = _strdup((const char*)lexeme);
    else if(kind == TK_FLOAT_LITERAL) {
        token->decimal_lexeme = *(double*)lexeme;
    } else if(kind == TK_INT_LITERAL) {
        token->number_lexeme = *(int*)lexeme;
    } else {
        token->string_lexeme = (const char*)lexeme;
    }
    token->kind = kind;
    token->category = category;
    token->location.filename = filename;
    token->location.line = line;
    token->location.column = column;
    return true;
}

int token_print(token_t* token) {
    if(token->kind == TK_STRING_LITERAL)
        return printf("token: { lexeme: '%s', filename: %s, line: %zu, column: %zu }\n", token->string_lexeme, token->location.filename, token->location.line, token->location.column);
    else if(token->kind == TK_FLOAT_LITERAL) {
        return printf("token: { lexeme: '%f', filename: %s, line: %zu, column: %zu }\n", token->decimal_lexeme, token->location.filename, token->location.line, token->location.column);
    } else if(token->kind == TK_INT_LITERAL) {
        return printf("token: { lexeme: '%i', filename: %s, line: %zu, column: %zu }\n", token->number_lexeme, token->location.filename, token->location.line, token->location.column);
    } else if(token->kind == TK_CHAR_LITERAL) {
        return printf("token: { lexeme: '%c', filename: %s, line: %zu, column: %zu }\n", token->char_lexeme, token->location.filename, token->location.line, token->location.column);        
    } else {
        return printf("token: { lexeme: '%s', filename: %s, line: %zu, column: %zu }\n", token->string_lexeme, token->location.filename, token->location.line, token->location.column);
    }
}
