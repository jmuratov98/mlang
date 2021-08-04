#include "mlang/token.h"

#include "mlang/containers/vector.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// TODO: Change this, make this into a map of sorts
static const char *token_string[] = {
    "INTEGER", "FLOAT", "BOOLEAN", "VOID", "CLASS", "STRUCT",
    "INT_LITERAL", "FLOAT_LITERAL", "STRING_LITERAL", "TRUE", "FALSE",
    "IMPORT", "EXPORT",
    "IF", "ELSE", "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE", "RETURN",
    "ADD", "SUB", "MULT", "DIV", "MOD", "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "MULT_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "INC", "DEC",
    "BIT_AND", "BIT_OR", "BIT_XOR", "BIT_NOT",
    "LOGIC_AND", "LOGIC_OR", "LOGIC_NOT", "DOUBLE_EQUAL", "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL",
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET",
    "DOUBLE_QUOTE", "SINGLE_QUOTE", "BACK_TICK",
    "COLON", "SEMICOLON", "DOT", "SKINNY_ARROW", "QUESTION",
    "IDENTIFIER", "UNKNOWN", "END_OF_FILE"
};

static bool has_more_tokens(lexer_t *lexer);

static void get_next_char(lexer_t *lexer);
static char peek(lexer_t *lexer);

static char skip_whitespace(lexer_t *lexer);
static void get_decimal(lexer_t *lexer);
static void get_reserved_tokens(lexer_t *lexer);

static void create_and_append_token(lexer_t *lexer, token_type_e type, char *value);

static bool is_whitespace(char c);
static bool is_operator(char c);

static token_type_e token_char_to_enum(char c);
static token_type_e get_token_from_str(const char *str);

token_t *token_create(token_type_e type, char *value)  {
    token_t *t = malloc(sizeof(token_t));
    t->type = type;
    t->value = malloc((strlen(value) + 1) * sizeof(char));
    memcpy(t->value, value, strlen(value));
    t->value[strlen(value)] = '\0';

    return t;
}

void token_destroy(token_t *token) {
    free(token->value);
    free(token);
}

void lexer_init(lexer_t *lexer, const char *string) {
    lexer->tokens = vector_create(token_t*);
    lexer->string = string;
    lexer->current_char = '\0';
    lexer->current_index = 0;
}

void lexer_destroy(lexer_t *lexer) {
    for(int i = 0; i < vector_size(lexer->tokens); i++) {
        token_destroy(lexer->tokens[i]);
    }
    vector_destroy(lexer->tokens);
}

void lex(lexer_t *lexer) {
    char c;
    do {
        get_next_char(lexer);
        c = lexer->current_char;

        if(c == 0) {
            create_and_append_token(lexer, END_OF_FILE, "");
            return;
        }

        c = skip_whitespace(lexer);
        // TODO: Need to skip comments - // and /*  */

        // "+", "-", "*", "/", "%", "=", "=="
        // "(", ")", "{", "}"
        // ">", ">=", "<", "<="
        // TODO: Refactor this
        if(is_operator(c)) {
            char val[3] = "\0";
            val[0] = c;

            token_type_e type = UNKNOWN;

            char next_char = peek(lexer);
            if(next_char == '=') {
                // +=, -=, *=, /=, %=
                val[1] = next_char;
                if(c == '+') type = ADD_ASSIGN;
                if(c == '-') type = SUB_ASSIGN;
                if(c == '*') type = MULT_ASSIGN;
                if(c == '/') type = DIV_ASSIGN;
                if(c == '%') type = MOD_ASSIGN;
                if(c == '=') type = DOUBLE_EQUAL;
                if(c == '>') type = GREATER_EQUAL;
                if(c == '<') type = LESS_EQUAL;
                get_next_char(lexer);

            } else if(c == '+' && next_char == '+') {
                // ++
                val[1] = next_char;
                type = INC;
                get_next_char(lexer);
            } else if(c == '-' && next_char == '-') {
                // --
                val[1] = next_char;
                type = DEC;
                get_next_char(lexer);
            } else {
                type = token_char_to_enum(c);
            }
            create_and_append_token(lexer, type, val);
        }

        // Integer or floating point literal
        // /\d+/g           - decimal
        // /0o[0-7]/        - octal
        // /0x\d+[a-fA-F]   - hex
        // TODO: Implement octal and hex literals
        else if(isdigit(c))
            get_decimal(lexer);

        else if(c == '"' || c == '`') {
            // String literal starts here
        }

        // All thats left to check is the type.
        else if(isalpha(c) || c == '_')
            get_reserved_tokens(lexer);

        else
            create_and_append_token(lexer, UNKNOWN, "");

    } while(c != '\0');

}


void print_token(token_t *token) {
    printf("< %s , %s >\n", token_string[token->type], token->value);
}

bool has_more_tokens(lexer_t *lexer) {
    return lexer->current_index < (int64)strlen(lexer->string);
}

void get_next_char(lexer_t *lexer) {
    lexer->current_char = lexer->string[lexer->current_index];
    lexer->current_index++;
}

char peek(lexer_t *lexer) {
    if(has_more_tokens(lexer)) 
        return lexer->string[lexer->current_index];
    return '\0';
}

char skip_whitespace(lexer_t *lexer) {
    char c = lexer->current_char;
    while(is_whitespace(c) == true) {
        get_next_char(lexer);
        c = lexer->current_char;
    }
    return c;
}

void get_decimal(lexer_t *lexer) {
    char *decimal = vector_create(char);
    char c = lexer->current_char;
    token_type_e type = INT_LITERAL;

    while(isdigit(c) || c == '.') {
        if(c == '.') type = FLOAT_LITERAL;
        vector_push_back(decimal, c);

        get_next_char(lexer);
        c = lexer->current_char;
    }

    vector_push_back(decimal, '\0');
    create_and_append_token(lexer, type, decimal);
    vector_destroy(decimal);
}

void get_reserved_tokens(lexer_t *lexer) {
    char *token = vector_create(char);
    char c = lexer->current_char;
    token_type_e type = UNKNOWN;
    
    while(isalnum(c) || c == '_') {
        vector_push_back(token, c);
        get_next_char(lexer);
        c = lexer->current_char;
    }

    vector_push_back(token, '\0');
    type = get_token_from_str(token);

    create_and_append_token(lexer, type, token);
    vector_destroy(token);
}

void create_and_append_token(lexer_t *lexer, token_type_e type, char *value) { 
    token_t *token = token_create(type, value);
    vector_push_back(lexer->tokens, token);
}

bool is_whitespace(char c) {
    return isspace(c);
}

bool is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' ||
            c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']'    );
}

token_type_e token_char_to_enum(char c) {
    switch(c) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MULT;
        case '/': return DIV;
        case '%': return MOD;
        case '=': return ASSIGN;

        case '(': return LEFT_PAREN;
        case ')': return RIGHT_PAREN;
        case '{': return LEFT_CURLY_BRACKET;
        case '}': return RIGHT_CURLY_BRACKET;
        case '[': return LEFT_SQUARE_BRACKET;
        case ']': return RIGHT_SQUARE_BRACKET;

        case '>': return GREATER;
        case '<': return LESS;

        default:  return UNKNOWN;
    }
    return UNKNOWN;
}

token_type_e get_token_from_str(const char *str) {
    // types
    if(strcmp("int", str) == 0)      return INT;
    if(strcmp("float", str) == 0)    return FLOAT;
    if(strcmp("bool", str) == 0)     return BOOL;
    if(strcmp("void", str) == 0)     return VOID;
    if(strcmp("class", str) == 0)    return CLASS;
    if(strcmp("struct", str) == 0)   return STRUCTURE;
    
    // boolean
    if(strcmp("true", str) == 0)     return TRUE;
    if(strcmp("false", str) == 0)    return FLASE;
    
    // Module
    if(strcmp("import", str) == 0)   return IMPORT;
    if(strcmp("export", str) == 0)   return EXPORT;
    
    // Conditional
    if(strcmp("if", str) == 0)       return IF;
    if(strcmp("else", str) == 0)     return ELSE;
    if(strcmp("switch", str) == 0)   return SWITCH;
    if(strcmp("case", str) == 0)     return CASE;
    if(strcmp("default", str) == 0)  return DEFAULT;
    if(strcmp("break", str) == 0)    return BREAK;
    if(strcmp("continue", str) == 0) return CONTINUE;
    if(strcmp("return", str) == 0)   return RETURN;

    return IDENTIFIER;
}
