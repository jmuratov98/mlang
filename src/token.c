#include "mlang/token.h"

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

void print_token(token_t *token) {
    printf("<%s, %s>\n", token->value, token_string[token->type]);
}