#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "containers/ast.h"

#include <stdio.h>

static token_t accept();
static bool expect(token_kind_e kind);
static void skip(token_kind_e kind);

inline bool is_eof();
inline bool is_import();
inline bool is_identifier();
inline bool is_declaration_specifier();
inline bool is_storage_class_specifier();
inline bool is_type_specifier();
inline bool is_type_qualifier();
inline bool is_string_literal();
inline bool is_function_definition();

static ast_node_t* parse_import();
static ast_node_t* parse_function_definition();
static ast_node_t* parse_constant_expression();
static ast_node_t* parse_conditional_expression();
static ast_node_t* parse_logical_or_expression();
static ast_node_t* parse_logical_and_expression();
static ast_node_t* parse_inclusive_or_expression();
static ast_node_t* parse_exclusive_or_expression();
static ast_node_t* parse_and_expression();
static ast_node_t* parse_equality_expression();
static ast_node_t* parse_relational_expression();
static ast_node_t* parse_shift_expression();
static ast_node_t* parse_additive_expression();
static ast_node_t* parse_multiplicative_expression();
static ast_node_t* parse_cast_expression();
static ast_node_t* parse_unary_expression();
static ast_node_t* parse_postfix_expression();
static ast_node_t* parse_primary_expression();
static ast_node_t* parse_expression();

static bool parse_pointer();

static bool parse_declarator(ast_node_t* parent);
static bool parse_function_args(ast_node_t* parent);

/**
 *  translation_unit ::= {external_declaration}*
 *
 *  external_declaration ::= import-statement
 *                         | function-definition
 *                         | declaration
 *
 */
ast_node_t* parser_parse() {
    ast_node_t* translation_unit = ast_create_translation_unit();
    while(!is_eof()) {
        if(is_import()) {
            ast_node_t* import_node = parse_import();
            array_push_back(translation_unit->children, import_node);
        } else if(is_function_definition()) {
            ast_node_t* function_declarator_node = parse_function_definition();
            array_push_back(translation_unit->children, function_declarator_node);
        }
    }
    return translation_unit;
}

/**
 * import_statement ::= import printf from "stdio" ';'
 *                    | import { printf, scanf } from "stdio" ';'
 *                    | import("stdio") ';'
*/
ast_node_t* parse_import() {
    ast_node_t* import = ast_create_import_statement();
    // import
    // TODO: This should probably not be pushed into the tree ... because we know that it is an import based on the tag.
    if(expect(TK_IMPORT)) {
        ast_node_t* kw = ast_create_keyword("import");
        array_push_back(import->children, kw);
    } else {
        return import;
    }

    // import '(' string ')'
    if(expect(TK_LEFT_PAREN)) {
        token_t token = token_peek();
        
        // TODO: this could be something else like an identifer 
        ast_node_t* path = ast_create_string(token.string_lexeme);
        array_push_back(import->children, path);
        
        skip(TK_RIGHT_PAREN);

        return import;
    }

    // import { identifiers } | import identifier
    if(is_identifier()) {
        token_t token = token_peek();
        ast_node_t* identifier = ast_create_identifier(token.string_lexeme);
        array_push_back(import->children, identifier);
        accept();
    } else if(expect(TK_LEFT_CURLY_BRACKET)) {
        token_t token = token_peek();
        do {
            ast_node_t* identifier = ast_create_identifier(token.string_lexeme);
            array_push_back(import->children, identifier);
            token = accept();
        } while(token.kind != TK_RIGHT_CURLY_BRACKET);
        token = accept();
    } else {
        return import;
    }

    // from
    // TODO: This should probably not be pushed into the tree ... because we know that it is an import based on the tag.
    if(expect(TK_FROM)) {
        ast_node_t* kw = ast_create_keyword("from");
        array_push_back(import->children, kw);
    } else {
        return import;
    }

    // string
    if(is_string_literal()) {
        token_t token = token_peek();
        ast_node_t* path = ast_create_string(token.string_lexeme);
        array_push_back(import->children, path);
        accept();
    }

    // ';'
    if(!expect(TK_SEMICOLON)){ 
        // TODO: Error Checking
    }

    return import;
}

static token_t tokens[1024] = {};
static int32_t top = -1;
#define stack_push(token)           tokens[++top] = token
#define stack_pop()                 tokens[top--]
#define stack_get()                 tokens[top]

/**
*  function-definition ::= {declaration-specifier}+ declarator {declaration}* compound-statement
*/
ast_node_t* parse_function_definition() {
    ast_node_t* function_defintion_node = ast_create_function_definition();

    // Pushing all tokens except for the identifier to the stack
    token_t token = token_peek();
    while(token.kind != TK_IDENTIFIER) {
        stack_push(token);
        token = accept();
    }

    function_defintion_node->function.name = token.string_lexeme;
    token = accept();

    parse_declarator(function_defintion_node);

    if(token.kind == TK_SEMICOLON) {
        token = accept();
    } else if(token.kind == TK_LEFT_CURLY_BRACKET) {
        while(token.kind != TK_RIGHT_CURLY_BRACKET) {
            token = accept();
        }
        token = accept();
    }

    return function_defintion_node;
}


/**
 *
 * direct-declarator ::= identifier
 *                     | '(' declarator ')'
 *                     | direct-declarator '[' {constant-expression}? ']'
 *                     | direct-declarator '(' paramater-type-list ')'
 *                     | direct-declarator '(' {identifier}* ')'
 *
 * char * const * ( * next ) ( )              step 1     -- Find the first identifier
 * char * const * ( *      ) ( )              step 2 & 3 -- doesn't match '(' or '['
 * char * const * ( *      ) ( )              step 4     -- the token on the left doesnt match '('
 * char * const * ( *      ) ( )              step 5     -- the token kind matches an asterik
 * char * const * (        ) ( )              step 4     -- matches the "(" with ")"
 * char * const *            ( )              step 2     -- doesn't match '['
 * char * const *            ( )              step 3     -- matches ()
 * char * const *                             step 4     -- the token on the left matches "("
 * char * const *                             step 5     -- the token on the left matches "*"
 * char * const                               step 5     -- the token on the left matches "const"
 * char *                                     step 5     -- the token on the left matches "*"
 * char                                       step 6     -- the token on the left is a type
 *
 */
bool parse_declarator(ast_node_t* parent) {
    token_t token = token_peek();

    if(expect(TK_LEFT_SQUARE_BRACKET)) {
        ast_node_t* node = parse_constant_expression();
        array_push_back(parent->children, node);
        skip(TK_RIGHT_SQUARE_BRACKET);
    } else if(expect(TK_LEFT_PAREN)) {
        parse_function_args(parent);
    }

    // Parse pointers
    while(stack_get().kind == TK_MULT) {
        stack_pop();
        if(stack_get().kind == TK_CONST)
            stack_pop();
    }

    while(top >= 0) {
        if(token.kind == TK_LEFT_PAREN) {
            stack_pop();
            token_t token = accept();
            parse_declarator(parent);
        }
    }

    return true;
}

ast_node_t* parse_constant_expression() {
    return parse_conditional_expression();    
}

ast_node_t* parse_conditional_expression() {
    ast_node_t* log_or_exp = parse_logical_or_expression();

    if(!expect(TK_QUESTION))
        return log_or_exp;

    ast_node_t* cond_exp = ast_create_ternary();
    cond_exp->ternexp.left = log_or_exp;

    ast_node_t* exp = parse_expression();
    cond_exp->ternexp.mid = exp;

    skip(TK_COLON);

    ast_node_t* condexp = parse_conditional_expression();
    cond_exp->ternexp.right = condexp;

    return cond_exp;
}

ast_node_t* parse_logical_or_expression() {
    return NULL;
}

ast_node_t* parse_logical_and_expression() {
    return NULL;
}

ast_node_t* parse_inclusive_or_expression() {
    return NULL;
}

ast_node_t* parse_exclusive_or_expression() {
    return NULL;
}

ast_node_t* parse_and_expression() {
    return NULL;
}

ast_node_t* parse_equality_expression() {
    return NULL;
}

ast_node_t* parse_relational_expression() {
    return NULL;
}

ast_node_t* parse_shift_expression() {
    return NULL;
}

ast_node_t* parse_additive_expression() {
    return NULL;
}

ast_node_t* parse_multiplicative_expression() {
    return NULL;
}

ast_node_t* parse_cast_expression() {
    return NULL;
}

ast_node_t* parse_unary_expression() {
    return NULL;
}

ast_node_t* parse_postfix_expression() {
    return NULL;
}

ast_node_t* parse_primary_expression() {
    return NULL;
}

ast_node_t* parse_expression() {
    return NULL;
}

// TODO: Error Checking...
/**
*
*
*/
bool parse_function_args(ast_node_t* parent) {
    if(expect(TK_RIGHT_PAREN))
        return true;

    token_t token = token_peek();
    while(token.kind != TK_RIGHT_PAREN) {
        //TODO: Implement this...
        token = accept();
    }

    if(expect(TK_RIGHT_PAREN))
        return true;

    return false;
}

/**
*  pointer ::= '*' {type-qualifier}* {pointer}?
*/
bool parse_pointer() {
    return true;
}

token_t accept() {
    token_consume();
    return token_peek();
}

bool expect(token_kind_e kind) {
    token_t token = token_peek();
    if(token.kind == kind) {
        token_consume();
        return true;
    }
    return false;
}

void skip(token_kind_e kind) {
    token_t token = token_peek();
    if(token.kind == kind) {
        token_consume();
    }
}

bool is_eof() {
    token_t token = token_peek();
    return token.kind == TK_END_OF_FILE;
}

bool is_import() {
    token_t token = token_peek();
    return token.kind == TK_IMPORT;
}

bool is_identifier() {
    token_t token = token_peek();
    return token.kind == TK_IDENTIFIER;
}

bool is_declaration_specifier() {
    return is_storage_class_specifier() || is_type_specifier() || is_type_qualifier();
}

bool is_storage_class_specifier() {
    token_t token = token_peek();
    return  (token.kind == TK_STATIC) || (token.kind == TK_TYPE) || (token.kind == TK_EXPORT);
}

bool is_type_specifier() {
    token_t token = token_peek();
    return  (token.kind == TK_VOID) || (token.kind == TK_CHAR) || (token.kind == TK_SHORT) ||
            (token.kind == TK_INT) || (token.kind == TK_LONG) || (token.kind == TK_FLOAT) ||
            (token.kind == TK_SIGNED) || (token.kind == TK_UNSIGNED) || (token.kind == TK_STRUCTURE) ||
            (token.kind == TK_CLASS) || (token.kind == TK_UNION) || (token.kind == TK_ENUM);
}

bool is_type_qualifier() {
    token_t token = token_peek();
    return (token.kind == TK_CONST) || (token.kind == TK_READONLY);
}

bool is_string_literal() {
    token_t token = token_peek();
    return token.kind == TK_STRING_LITERAL;
}

bool is_function_definition() {
    token_t token;
    
    // Search for the the function_name
    size_t i = 0;
    do {
        token = token_peekn(i);
        i++;
    } while(token.kind != TK_IDENTIFIER && token.kind != TK_SEMICOLON && token.kind != TK_LEFT_CURLY_BRACKET);

    size_t last_index_of_left_paren = i;
    do {
        token = token_peekn(i);
        if(token.kind == TK_LEFT_PAREN) {
            last_index_of_left_paren = i;
        }
        i++;
    } while(token.kind != TK_SEMICOLON && token.kind != TK_LEFT_CURLY_BRACKET);

    if(i == last_index_of_left_paren)
        return false;

    if(!is_type_specifier()) {
        return false;
    }

    return true;
}
