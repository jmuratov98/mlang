#include "lexer.h"
#include "containers/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static array(token_t) tokens;
static uint64_t tokens_index_at;

static char peekn(lexer_t*, uint64_t);
static char peek(lexer_t*);
static void get_next_char(lexer_t*);

static void _skip_whitespace(lexer_t*);
static void _skip_comments(lexer_t*);
static bool _tokenize_string(lexer_t*, token_t*);
static bool _tokenize_number(lexer_t*, token_t*);
static bool _tokenize_keyword(lexer_t*, token_t*);
static bool _tokenize_operator(lexer_t*, token_t*);

bool lexer_init(lexer_t* lexer, const char* filecontent) {
    lexer->location.filename = NULL;
    lexer->location.line = 1;
    lexer->location.column = 1;
    
    lexer->filecontents = filecontent;
    lexer->filecontents_length = strlen(filecontent);

    lexer->current_index = 0;
    lexer->current_char = lexer->filecontents[lexer->current_index++];

    tokens = array_create(token_t);
    tokens_index_at = 0;

    return true;
}

void tokenize(lexer_t* lexer) {
    while(true) {
        token_t token;
        if(lexer->current_char == '\0') {
            token_init(&token, "", TK_END_OF_FILE, TC_UNKNOWN, NULL, 1, 1);
            array_push_back(tokens, token);
            return;
        }

        _skip_whitespace(lexer);
        _skip_comments(lexer);

        if(_tokenize_string(lexer, &token)) {
            array_push_back(tokens, token);
            continue;
        }

        if(_tokenize_number(lexer, &token)) {
            array_push_back(tokens, token);
            continue;
        }

        if(_tokenize_operator(lexer, &token)) {
            array_push_back(tokens, token);
            continue;
        }

        if(_tokenize_keyword(lexer, &token)) {
            array_push_back(tokens, token);
            continue;
        }
        
        token_init(&token, "", TK_UNKNOWN, TC_UNKNOWN, NULL, 1, 1);
        array_push_back(tokens, token);
        continue;
    }
}

void token_consume() {
    token_consumen(1);
}

void token_consumen(size_t n) {
    if(tokens_index_at + n >= array_size(tokens))
        tokens_index_at = array_size(tokens);
    tokens_index_at += n;
}

token_t token_peek() {
    return token_peekn(0);
}

token_t token_peekn(size_t n) {
    if(tokens_index_at + n >= array_size(tokens))
        return tokens[array_size(tokens)];
    return tokens[tokens_index_at + n];
}

array(token_t) lexer_get_tokens() {
    return tokens;
}

char peekn(lexer_t* lexer, uint64_t n) {
    if(lexer->current_index + (n - 1) >= lexer->filecontents_length)
        return '\0';
    return lexer->filecontents[lexer->current_index + (n - 1)];
}

char peek(lexer_t* lexer) {
    return peekn(lexer, 1);
}

void get_next_char(lexer_t* lexer) {
    lexer->current_char = lexer->filecontents[lexer->current_index++];
}

void _skip_whitespace(lexer_t* lexer) {
    while(isspace(lexer->current_char)) {
        get_next_char(lexer);
    }    
}

void _skip_comments(lexer_t* lexer) {
    if(lexer->current_char != '/')      return;

    char c = peek(lexer);
    if(c != '/' && c != '*')            return;

    get_next_char(lexer);
}

bool _tokenize_string(lexer_t* lexer, token_t* token) {
    char delim = lexer->current_char;
    if(delim != '"' && delim != '\'' && delim != '`')   return false;
    get_next_char(lexer);

    size_t cap = 10;
    size_t size = 0;
    char* string = calloc(cap, sizeof(char));

    while(lexer->current_char != delim) {
        if(size >= cap) {
            cap *= 2;
            string = realloc(string, cap * sizeof(char));
        }
        string[size++] = lexer->current_char;
        get_next_char(lexer);
    }
    string[size] = 0;

    get_next_char(lexer);

    token_init(token, string, TK_STRING_LITERAL, TC_LITERAL, NULL, 1, 1);

    free(string);

    return true;
}

typedef enum {
    BEGIN, END,
    INTEGER, FLOAT, HEX, OCTAL, BINARY
} number_state;

typedef int (*is_type_fn_t) (int);

static bool _tokenize_number_base(lexer_t* lexer, char* number, size_t cap, size_t size, is_type_fn_t istype) {
    while(istype(lexer->current_char)) {
        if(size >= cap) {
            cap *= 2;
            number = realloc(number, cap);
        }

        number[size++] = lexer->current_char;
        get_next_char(lexer);

    }
    number[size] = '\0';
    return true;        
}

static int isodigit(int c) {
    return (c >= '0') && (c <= '7');
}

static int isbit(int c) {
    return (c == '0') || (c == '1');
}

static float _convert_string_to_float(char* number) {
    return 0.0f;
}

static int64_t _convert_string_to_number(char* number, int base) {
    if(base == 10) {
        return atoi(number);
    }

    // TODO: Implement something for the rest of the bases
    return 0;
}

bool _tokenize_number(lexer_t* lexer, token_t* token) {
    if(!isxdigit(lexer->current_char) && lexer->current_char != '.')  return false;
    number_state state = BEGIN;

    size_t cap = 10;
    size_t size = 0;
    char* number = calloc(cap, sizeof(char));
    token_kind_e kind = TK_INT_LITERAL;
    int base = 10;
    bool is_done = false;

    do {
        switch (state)
        {
        case BEGIN: {
            if(lexer->current_char == '.') {
                state = FLOAT;
                number[size++] = '.';
                get_next_char(lexer);
                break;
            }
            char c = peek(lexer);
            if(size == 0) {
                if(lexer->current_char == '0' && c == 'x') {
                    state = HEX;
                    number[size++] = '0';
                    number[size++] = 'x';
                    get_next_char(lexer);
                    get_next_char(lexer);
                    break;
                }
                if(lexer->current_char == '0' && c == 'o') {
                    state = OCTAL;
                    number[size++] = '0';
                    number[size++] = 'o';
                    get_next_char(lexer);
                    get_next_char(lexer);
                    break;
                }
                if(lexer->current_char == '0' && c == 'b') {
                    state = BINARY;
                    number[size++] = '0';
                    number[size++] = 'b';
                    get_next_char(lexer);
                    get_next_char(lexer);
                    break;
                }
                if(c == '.') {
                    state = FLOAT;
                    number[size++] = lexer->current_char;
                    number[size++] = '.';
                    get_next_char(lexer);
                    get_next_char(lexer);
                    break;
                }
            }
            if(!isdigit(lexer->current_char)) {
                free(number);
                return false;
            }
            state = INTEGER;
            number[size++] = lexer->current_char;
            get_next_char(lexer);
        } break;

        case END: {
            if(kind == TK_FLOAT_LITERAL) {
                float value = _convert_string_to_float(number);
                token_init(token, &value, kind, TC_LITERAL, NULL, 1, 1);
            } else {
                int64_t value = _convert_string_to_number(number, base);
                token_init(token, &value, kind, TC_LITERAL, NULL, 1, 1);
            }
                
            free(number);
            return true;
        } break;

        case INTEGER:
            _tokenize_number_base(lexer, number, cap, size, isdigit);
            state = END;
            break;
        case HEX:
            base = 16;
            _tokenize_number_base(lexer, number, cap, size, isxdigit);
            state = END;
            break;
        case OCTAL:
            base = 8;
            _tokenize_number_base(lexer, number, cap, size, isodigit);
            state = END;
            break;
        case BINARY:
            base = 2;
            _tokenize_number_base(lexer, number, cap, size, isbit);
            state = END;
            break;
        case FLOAT:
            kind = TK_FLOAT_LITERAL;
            break;

        default:
            break;
        }


    } while(!is_done);
    return false;
}

bool _tokenize_operator(lexer_t* lexer, token_t* token) {
    if(!ispunct(lexer->current_char)) return false;
    
    size_t i = 0;
    char c = peekn(lexer, i++);
    switch (c)
    {
    case '+': {
        c = peekn(lexer, i);
        if(c == '+') {
            i++;
            token_init(token, "++", TK_INC, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        if(c == '=') {
            i++;
            token_init(token, "+=", TK_ADD_ASSIGN, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        token_init(token, "+", TK_ADD, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '-': {
        c = peekn(lexer, i);
        if(c == '>') {
            i++;
            token_init(token, "->", TK_SKINNY_ARROW, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        if(c == '-') {
            i++;
            token_init(token, "--", TK_DEC, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        if(c == '=') {
            i++;
            token_init(token, "-=", TK_SUB_ASSIGN, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        
        token_init(token, "-", TK_SUB, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '*': {
        c = peekn(lexer, i);
        if(c == '=') {
            i++;
            token_init(token, "*=", TK_MULT_ASSIGN, TC_OPERATOR, NULL, 1, 1);
            break;
        }

        token_init(token, "*", TK_MULT, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '/': {
        c = peekn(lexer, i);
        if(c == '=') {
            i++;
            token_init(token, "/=", TK_DIV_ASSIGN, TC_OPERATOR, NULL, 1, 1);
            break;
        }

        token_init(token, "/", TK_DIV, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '%': {
        c = peekn(lexer, i);
        if(c == '=') {
            i++;
            token_init(token, "%=", TK_MOD_ASSIGN, TC_OPERATOR, NULL, 1, 1);
            break;
        }

        token_init(token, "%", TK_MOD, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '^': {
        c = peekn(lexer, i);
        if(c == '=') {
            i++;
            token_init(token, "^=", TK_BIT_XOR_ASSIGN, TC_OPERATOR, NULL, 1, 1);
            break;
        }

        token_init(token, "^", TK_BIT_XOR, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '&': {
        c = peekn(lexer, i);
        if(c == '=') {
            i++;
            token_init(token, "&=", TK_BIT_AND_ASSIGN, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        if(c == '&') {
            i++;
            token_init(token, "&&", TK_LOGIC_AND, TC_OPERATOR, NULL, 1, 1);
            break;
        }

        token_init(token, "&", TK_BIT_AND, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '|': {
        c = peekn(lexer, i);
        if(c == '=') {
            i++;
            token_init(token, "|=", TK_BIT_OR_ASSIGN, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        if(c == '&') {
            i++;
            token_init(token, "||", TK_LOGIC_OR, TC_OPERATOR, NULL, 1, 1);
            break;
        }

        token_init(token, "|", TK_BIT_OR, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '>': {
        c = peekn(lexer, i);
        if(c == '>') {
            i++;
            c = peekn(lexer, i);
            if(c == '=') {
                i++;
                token_init(token, ">>=", TK_BIT_SHIFT_RIGHT_ASSIGN, TC_OPERATOR, NULL, 1, 1);
                break;
            }
            token_init(token, ">>", TK_BIT_SHIFT_RIGHT, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        if(c == '=') {
            i++;
            token_init(token, ">=", TK_GREATER_EQUAL, TC_OPERATOR, NULL, 1, 1);
            break;
        }

        token_init(token, ">", TK_GREATER, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '<': {
        c = peekn(lexer, i);
        if(c == '<') {
            i++;
            c = peekn(lexer, i);
            if(c == '=') {
                i++;
                token_init(token, "<<=", TK_BIT_SHIFT_LEFT_ASSIGN, TC_OPERATOR, NULL, 1, 1);
                break;
            }
            token_init(token, "<<", TK_BIT_SHIFT_LEFT, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        if(c == '=') {
            i++;
            token_init(token, "<=", TK_LESS_EQUAL, TC_OPERATOR, NULL, 1, 1);
            break;
        }

        token_init(token, "<", TK_LESS, TC_OPERATOR, NULL, 1, 1);
        break;
    }
    case '(':
        token_init(token, "(", TK_LEFT_PAREN, TC_OPERATOR, NULL, 1, 1);
        break;
    case ')':
        token_init(token, ")", TK_RIGHT_PAREN, TC_OPERATOR, NULL, 1, 1);
        break;
    case '[':
        token_init(token, "[", TK_LEFT_SQUARE_BRACKET, TC_OPERATOR, NULL, 1, 1);
        break;
    case ']':
        token_init(token, "]", TK_RIGHT_SQUARE_BRACKET, TC_OPERATOR, NULL, 1, 1);
        break;
    case '{':
        token_init(token, "{", TK_LEFT_CURLY_BRACKET, TC_OPERATOR, NULL, 1, 1);
        break;
    case '}':
        token_init(token, "}", TK_RIGHT_CURLY_BRACKET, TC_OPERATOR, NULL, 1, 1);
        break;
    case ':':
        token_init(token, ":", TK_COLON, TC_OPERATOR, NULL, 1, 1);
        break;
    case ';':
        token_init(token, ";", TK_SEMICOLON, TC_OPERATOR, NULL, 1, 1);
        break;
    case '.':
        token_init(token, ".", TK_DOT, TC_OPERATOR, NULL, 1, 1);
        break;
    case '?':
        token_init(token, "?", TK_QUESTION, TC_OPERATOR, NULL, 1, 1);
        break;
    case ',':
        token_init(token, ",", TK_COMMA, TC_OPERATOR, NULL, 1, 1);
        break;
    case '!':
        char c = peekn(lexer, i);
        if(c == '=') {
            i++;
            token_init(token, "!=", TK_NOT_EQUAL, TC_OPERATOR, NULL, 1, 1);
            break;
        }
        token_init(token, "!", TK_LOGIC_NOT, TC_OPERATOR, NULL, 1, 1);
        break;
    case '~':
        token_init(token, "~", TK_BIT_NOT, TC_OPERATOR, NULL, 1, 1);
        break;
    default:
        return false;
    }

    for(; i != 0; i--)
        get_next_char(lexer);

    return true;
}

#define compare_and_return(token, keyword, string, tok_kind)      if(strcmp((keyword), (string)) == 0)    { token_init(token, (string), (tok_kind), TC_KEYWORD, NULL, 1, 1); return true; }

static bool _search_and_append_token(token_t* token, char* keyword) {
    // Type Keywords
    compare_and_return(token, keyword, "char", TK_CHAR);
    compare_and_return(token, keyword, "int", TK_INT);
    compare_and_return(token, keyword, "float", TK_FLOAT);
    compare_and_return(token, keyword, "double", TK_BOOL);
    compare_and_return(token, keyword, "void", TK_VOID);
    compare_and_return(token, keyword, "string", TK_STRING);
    compare_and_return(token, keyword, "struct", TK_STRUCTURE);
    compare_and_return(token, keyword, "class", TK_CLASS);
    compare_and_return(token, keyword, "enum", TK_ENUM);    
    compare_and_return(token, keyword, "union", TK_UNION);    

    // Privacy
    compare_and_return(token, keyword, "public", TK_PUBLIC);    
    compare_and_return(token, keyword, "private", TK_PUBLIC);    
    compare_and_return(token, keyword, "public", TK_PROTECTED);    

    // Type / Function Modifiers
    compare_and_return(token, keyword, "static", TK_STATIC);
    compare_and_return(token, keyword, "inline", TK_INLINE);
    compare_and_return(token, keyword, "const", TK_CONST);
    compare_and_return(token, keyword, "readonly", TK_READONLY);

    // Size Modifier
    compare_and_return(token, keyword, "short", TK_SHORT);
    compare_and_return(token, keyword, "long", TK_LONG);

    compare_and_return(token, keyword, "signed", TK_SIGNED);
    compare_and_return(token, keyword, "unsigned", TK_UNSIGNED);

    compare_and_return(token, keyword, "true", TK_TRUE);
    compare_and_return(token, keyword, "false", TK_FLASE);

    // Module Keywords
    compare_and_return(token, keyword, "import", TK_IMPORT);
    compare_and_return(token, keyword, "export", TK_EXPORT);
    compare_and_return(token, keyword, "from", TK_FROM);

    // Conditional Keywords
    compare_and_return(token, keyword, "if", TK_IF);
    compare_and_return(token, keyword, "else", TK_ELSE);
    compare_and_return(token, keyword, "switch", TK_SWITKH);
    compare_and_return(token, keyword, "case", TK_CASE);
    compare_and_return(token, keyword, "default", TK_DEFAULT);
    compare_and_return(token, keyword, "break", TK_BREAK);
    compare_and_return(token, keyword, "continue", TK_CONTINUE);
    compare_and_return(token, keyword, "return", TK_RETURN);
    compare_and_return(token, keyword, "for", TK_FOR);
    compare_and_return(token, keyword, "while", TK_WHILE);
    compare_and_return(token, keyword, "do", TK_DO);
    compare_and_return(token, keyword, "goto", TK_GOTO);

    // Other Keywords
    compare_and_return(token, keyword, "type", TK_TYPE);

    return false;
}

bool _tokenize_keyword(lexer_t* lexer, token_t* token) {
    if(isalpha(lexer->current_char) == 0 && lexer->current_char != '_')      return false;

    bool status = true;
    size_t size = 0;
    size_t cap = 10;
    char* keyword = calloc(cap, sizeof(char));

    do {
        if(size >= cap) {
            cap *= 2;
            keyword = realloc(keyword, cap * sizeof(char));
        }
        keyword[size] = lexer->current_char;
        size++;
        
        get_next_char(lexer);
    } while(isalnum(lexer->current_char) || lexer->current_char == '_');
    keyword[size] = 0;

    if(_search_and_append_token(token, keyword)) {
        goto done;
    }

    token_init(token, keyword, TK_IDENTIFIER, TC_IDENTIFIER, NULL, 1, 1);
    
done:
    free(keyword);
    return status;
}

