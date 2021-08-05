#pragma once

#include "mlang/common.h"
#include "mlang/token_type.h"

#include <stdio.h>


// ======================== TOKENS ========================
typedef struct token_t {
    token_type_e type;
    char *value;
    int line, char_index;
} token_t;

/**
 * @brief Creates a token on the heap
 * 
 * @param[in] type the token type
 * @param[in] value the value of the token
 * @return void
 */ 

token_t *token_create(token_type_e type, char *value);
void token_destroy(token_t *token);

/**
 * @brief Prints a token
 * 
 * @param[in] token The token to print
 * @return void
 */ 
void print_token(token_t *token);
// ======================== LEXER ========================
typedef struct {
    // I'm not all that sure if the symbol tree (tokens) should be part of the lexer.
    // Also, it would probably be better if it was a linked list rather than an array.
    token_t **tokens; // Dynamically allocated array
    char *string;
    char current_char;
    int32 current_index;
} lexer_t;

/**
 * @brief Initializes the lexer
 * 
 * @param lexer The lexer
 * @return void
 */
void lexer_init(lexer_t *lexer, char *string);

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
