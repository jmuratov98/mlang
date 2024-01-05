#ifndef _MLANG_LEXER_H_
#define _MLANG_LEXER_H_

#include "common.h"
#include "token.h"
#include "containers/array.h"

typedef struct {
    location_t location;
    const char* filecontents;
    size_t filecontents_length;
    char current_char;
    size_t current_index;
} lexer_t;

bool lexer_init(lexer_t* lexer, const char*);

void tokenize(lexer_t* lexer);

void token_consume();
void token_consumen(size_t n);

token_t token_peek();
token_t token_peekn(size_t n);

array(token_t) lexer_get_tokens();

#endif // _MLANG_LEXER_H_