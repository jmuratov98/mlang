#include "mlang/token.h"

#include "mlang/containers/vector.h"

#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// TODO: Change this.
static const char *token_string[] = {
    "INT", "FLOAT", "BOOL", "VOID", "CLASS", "STRUCTURE",
    "IMPORT", "EXPORT",
    "IF", "ELSE", "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE", "RETURN",
    "ADD", "SUB", "MULT", "DIV", "MOD", "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "MULT_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "INC", "DEC",
    "BIT_AND", "BIT_OR", "BIT_XOR", "BIT_NOT",
    "LOGIC_AND", "LOGIC_OR", "LOGIC_NOT", 
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET", "LEFT_CURLY_BRACKET", "TIGHT_CURLY_BRACKET",
    "UNKNOWN", "END_OF_FILE"
};

#define APPEND_TOKEN(vector, tt, val) { \
    token_t token = { .type=tt, .value=val }; \
    vector_push_back(lexer->tokens, token); \
}

static bool has_more_tokens(lexer_t *lexer);

static void get_next_char(lexer_t *lexer);
static char peek(lexer_t *lexer);

static bool is_literal(lexer_t *lexer);

void lexer_init(lexer_t *lexer, const char *string) {
    lexer->tokens = vector_create(token_t);
    lexer->string = string;
    lexer->current_char = '\0';
    lexer->current_index = 0;
}

void lexer_destroy(lexer_t *lexer) {
    vector_destroy(lexer->tokens);
}

void lex(lexer_t *lexer) {
    char c;
    char *str;
    do {
        get_next_char(lexer);
        c = lexer->current_char;

        switch (c) {
        // Whitespace
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case '\v':
        case '\f':
            continue;
        // Operators
        case '+':
            APPEND_TOKEN(lexer->tokens, ADD, "+");
            break;
        case '-':
            APPEND_TOKEN(lexer->tokens, SUB, "-");
            break;
        case '*':
            APPEND_TOKEN(lexer->tokens, MULT, "*");
            break;
        case '/':
            APPEND_TOKEN(lexer->tokens, DIV, "/");
            break;
        case '%':
            APPEND_TOKEN(lexer->tokens, MOD, "%");
            break;
        case '(':
            APPEND_TOKEN(lexer->tokens, LEFT_PAREN, "(");
            break;
        case ')':
            APPEND_TOKEN(lexer->tokens, RIGHT_PAREN, ")");
            break;
        case '{':
            APPEND_TOKEN(lexer->tokens, LEFT_CURLY_BRACKET, "{");
            break;
        case '}':
            APPEND_TOKEN(lexer->tokens, RIGHT_CURLY_BRACKET, "}");
            break;
        case '[':
            APPEND_TOKEN(lexer->tokens, LEFT_SQUARE_BRACKET, "[");
            break;
        case ']':
            APPEND_TOKEN(lexer->tokens, RIGHT_SQUARE_BRACKET, "]");
            break;
        default:
            printf("default: %c\n", c);
            break; 
        }

    } while(peek(lexer) != '\0');
}

void print_token(token_t *token) {
    printf("<%s, %s>\n", token->value, token_string[token->type]);
}

bool has_more_tokens(lexer_t *lexer) {
    return lexer->current_index < (int64)strlen(lexer->string);
}

void get_next_char(lexer_t *lexer) {
    if(has_more_tokens(lexer)) 
        lexer->current_char = lexer->string[lexer->current_index++];
}

char peek(lexer_t *lexer) {
    if(has_more_tokens(lexer)) 
        return lexer->string[lexer->current_index + 1];
    return '\0';
}

bool is_literal(lexer_t *lexer) {

}
