#pragma once

#include "mlang/common.h"
#include "mlang/token_type.h"

#include <stdio.h>

typedef struct {
    token_type_e type;
    char *value;
    int line, char_index;
} token_t;

typedef struct {
    // I'm not all that sure if the symbol tree (tokens) should be part of the lexer.
    // Also, it would probably be better if it was a linked list rather than an array.
    token_t *tokens; // Dynamically allocated array
    const char *string;
    char current_char;
    int32 current_index;
} lexer_t;

/**
 * @brief Initializes the lexer
 * 
 * @param lexer The lexer
 * @return void
 */
void lexer_init(lexer_t *lexer, const char *string);

/**
 * @brief Frees all the memory allocated to the lexer
 * 
 * @param lexer The lexer
 * @return void
 */
void lexer_destroy(lexer_t *lexer);

/**
 * @brief Frees all the memory allocated to the lexer
 * 
 * @param lexer The lexer
 * @return void
 */
void lex(lexer_t *lexer);

// Debugging purposes
void print_token(token_t *token);