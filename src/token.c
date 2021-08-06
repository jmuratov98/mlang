#include "mlang/token.h"

#include "mlang/containers/vector.h"

#include "mlang/debug/mem.h"

#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// TODO: Change this, make this into a map of sorts
static const char *token_string[] = {
    "INTEGER", "FLOAT", "BOOLEAN", "VOID", "CLASS", "STRUCT", "CHAR",
    "INT_LITERAL", "FLOAT_LITERAL", "STRING_LITERAL", "TRUE", "FALSE",
    "IMPORT", "EXPORT", "FROM",
    "IF", "ELSE", "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE", "RETURN",
    "ADD", "SUB", "MULT", "DIV", "MOD", "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "MULT_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "INC", "DEC",
    "BIT_AND", "BIT_OR", "BIT_XOR", "BIT_NOT",
    "LOGIC_AND", "LOGIC_OR", "LOGIC_NOT", "DOUBLE_EQUAL", "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL",
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET",
    "DOUBLE_QUOTE", "SINGLE_QUOTE", "BACK_TICK",
    "COLON", "SEMICOLON", "DOT", "SKINNY_ARROW", "QUESTION", "COMMA",
    "IDENTIFIER", "UNKNOWN", "END_OF_FILE"
};

static bool has_more_tokens(lexer_t *lexer);

static void advance_char(lexer_t *lexer);
static void advance_nchar(lexer_t *lexer, int i);
static char peek(lexer_t *lexer);
static char peek_nth(lexer_t *lexer, int i);

static char skip_whitespace(lexer_t *lexer);
static void tokenize_operator(lexer_t *lexer, char c);
static void tokenize_number(lexer_t *lexer, char c);
static void tokenize_identifier(lexer_t *lexer, char c);
static void tokenize_string(lexer_t *lexer, char c);

static void create_and_append_token(lexer_t *lexer, token_type_e type, char *value);

static bool is_whitespace(char c);
static bool is_operator(char c);

static token_type_e token_char_to_enum(char c);
static token_type_e get_token_from_str(const char *str);

token_t *token_create(token_type_e type, char *value)  {
    token_t *t = xmalloc(sizeof(token_t));
    t->type = type;
    t->value = xmalloc((strlen(value) + 1) * sizeof(char));
    memcpy(t->value, value, strlen(value));
    t->value[strlen(value)] = '\0';

    return t;
}

void token_destroy(token_t *token) {
    xfree(token->value, (strlen(token->value) + 1) * sizeof(char));
    xfree(token, sizeof(token_t));
}

// Should this return a heap allocated lexer_t ?
void lexer_init(lexer_t *lexer, char *string) {
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
    xfree(lexer->string, strlen(lexer->string));
}

void lex(lexer_t *lexer) {
    char c;
    do {
        advance_char(lexer);
        c = lexer->current_char;

        if(c == 0) {
            create_and_append_token(lexer, END_OF_FILE, "");
            return;
        }

        c = skip_whitespace(lexer);
        // TODO: Need to skip comments - // and /*  */

        // TODO: Refactor this
        if(is_operator(c)) {
            tokenize_operator(lexer, c);
        }
        else if(isdigit(c)) {
            tokenize_number(lexer, c);
        }
        else if(c == '"' || c == '`' || c == '\'') {
            tokenize_string(lexer, c);
        }
        else if(isalpha(c) || c == '_')
            tokenize_identifier(lexer, c);
        else {
            char value[2] = "";
            value[0] = c;
            create_and_append_token(lexer, UNKNOWN, value);
        }

    } while(c != '\0');

}


void print_token(token_t *token) {
    printf("< %s , %s >\n", token_string[token->type], token->value);
}

bool has_more_tokens(lexer_t *lexer) {
    return lexer->current_index < (int64)strlen(lexer->string);
}

void advance_char(lexer_t *lexer) {
    lexer->current_char = lexer->string[lexer->current_index];
    lexer->current_index++;
}

void advance_nchar(lexer_t *lexer, int i) {
    lexer->current_index += i;
}

char peek(lexer_t *lexer) {
    if(has_more_tokens(lexer)) 
        return lexer->string[lexer->current_index];
    return '\0';
}

char peek_nth(lexer_t *lexer, int i) {
    if( lexer->current_index + 1 < strlen(lexer->string))
        return lexer->string[lexer->current_index + i];
    return '\0';
}

char skip_whitespace(lexer_t *lexer) {
    char c = lexer->current_char;
    while(is_whitespace(c) == true) {
        advance_char(lexer);
        c = lexer->current_char;
    }
    return c;
}

void tokenize_operator(lexer_t *lexer, char c) {
    char val[3] = "\0";
    val[0] = c;
        
    token_type_e type = UNKNOWN;
    char next_char = peek(lexer);
    if(next_char == '=') {
        // +=, -=, *=, /=, %=
        val[1] = next_char;
        switch(c) {
            case '+': type = ADD_ASSIGN; break;
            case '-': type = SUB_ASSIGN; break;
            case '*': type = MULT_ASSIGN; break;
            case '/': type = DIV_ASSIGN; break;
            case '%': type = MOD_ASSIGN; break;
            case '=': type = DOUBLE_EQUAL; break;
            case '>': type = GREATER_EQUAL; break;
            case '<': type = LESS_EQUAL; break;
        }
        advance_char(lexer);
    } else if(c == '+' && next_char == '+') {
        // ++
        val[1] = next_char;
        type = INC;
        advance_char(lexer);
    } else if(c == '-' && next_char == '-') {
        // --
        val[1] = next_char;
        type = DEC;
        advance_char(lexer);
    } else {
        type = token_char_to_enum(c);
    }
    create_and_append_token(lexer, type, val);
}

void tokenize_number(lexer_t *lexer, char c) {
    char *decimal = vector_create(char);
    token_type_e type = INT_LITERAL;
    int i = 0;

    while(isdigit(c) || c == '.') {
        if(c == '.') type = FLOAT_LITERAL;
        vector_push_back(decimal, c);

        c = peek_nth(lexer, i);
        i++;
    }

    advance_nchar(lexer, i - 1);
    create_and_append_token(lexer, type, decimal);
    vector_destroy(decimal);
}

void tokenize_identifier(lexer_t *lexer, char c) {
    char *token = vector_create(char);
    token_type_e type = UNKNOWN;
    int i = 0;

    while(isalnum(c) || c == '_') {
        vector_push_back(token, c);
        c = peek_nth(lexer, i);
        i++;
    }

    vector_push_back(token, '\0');
    type = get_token_from_str(token);

    create_and_append_token(lexer, type, token);
    vector_destroy(token);
    advance_nchar(lexer, i - 1);
}

void tokenize_string(lexer_t *lexer, char c) {
    char delim = c;
    char *str = vector_create(char);
    token_type_e type = STRING_LITERAL;
    int i = 0;
    c = peek_nth(lexer, i++);

    while(c != delim) {
        vector_push_back(str, c);
        c = peek_nth(lexer, i++);
        if(i == 20) break;
    }

    vector_push_back(str, '\0');
    create_and_append_token(lexer, type, str);
    advance_nchar(lexer, i);

    vector_destroy(str);
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
            c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' ||
            c == ':' || c == ';' || c == '.' || c == '?' || c == ','   );
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

        case ':': return COLON;
        case ';': return SEMICOLON;
        case '.': return DOT;
        case '?': return QUESTION;
        case ',': return COMMA;

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
    if(strcmp("char", str) == 0)     return CHAR;
    
    // boolean
    if(strcmp("true", str) == 0)     return TRUE;
    if(strcmp("false", str) == 0)    return FLASE;
    
    // Module
    if(strcmp("import", str) == 0)   return IMPORT;
    if(strcmp("export", str) == 0)   return EXPORT;
    if(strcmp("from", str) == 0)    return FROM;
    
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
