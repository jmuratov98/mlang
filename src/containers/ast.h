#pragma once

#include "common.h"
#include "containers/array.h"

typedef enum {
    AST_TAG_TRANSLATION_UNIT,

    AST_TAG_IMPORT_STATEMENT,
    AST_TAG_LABELED_STATEMENT,
    AST_TAG_EXPRESSION_STATEMENT,
    AST_TAG_COMPOUND_STATEMENT,
    AST_TAG_SELECTION_STATEMENT,
    AST_TAG_ITERATION_STATEMENT,
    AST_TAG_JUMP_STATEMENT,

    AST_TAG_VARIABLE_DECLARATION,
    AST_TAG_PARAM_VAR_DECLARATION,
    AST_TAG_FUNCTION_DEFINITION,
    AST_TAG_FUNCTION_DECLARATION,
    AST_TAG_FUNCTION_CALL,

    AST_TAG_UNARY_OPERATOR,
    AST_TAG_BINARY_OPERATOR,
    AST_TAG_TERNARY_OPERATOR,

    AST_TAG_KEYWORD,
    AST_TAG_IDENTIFIER,
    AST_TAG_CONSTANT_INT,
    AST_TAG_CONSTANT_FLOAT,
    AST_TAG_CONSTANT_CHAR,
    AST_TAG_CONSTANT_STRING,

    AST_TAG_UNKNOWN,
} ast_node_kind_e;

typedef struct ast_list ast_list_t;

typedef struct ast_node {
    ast_node_kind_e         tag;

    struct {
        const char*         operator;
        struct ast_node*    operand;
    } unexp;

    struct {
        const char*         operator;
        struct ast_node*    left;
        struct ast_node*    right;
    } biexp;

    struct {
        struct ast_node*    left;
        struct ast_node*    mid;
        struct ast_node*    right;
    } ternexp;

    struct {
        const char*                 name;
        array(struct ast_node*)     arguments;
        struct ast_node*            statement;
    } function;

    struct {
        struct ast_node*    condition;
        struct ast_node*    statement;
        struct ast_node*    else_statement;
    } ifcond;

    int64_t                 integer_constant;
    double                  decimal_constant;
    char                    character_constant;
    const char*             string_constant;
    const char*             identifier_name;
    const char*             keyword;

    // Anthing else lol....
    array(struct ast_node*)       children;
} ast_node_t;

ast_node_t* ast_create_translation_unit();
ast_node_t* ast_create_import_statement();

ast_node_t* ast_create_function_definition();

ast_node_t* ast_create_identifier(const char* identifier);
ast_node_t* ast_create_keyword(const char* keyword);
ast_node_t* ast_create_integer(int64_t integer);
ast_node_t* ast_create_decimal(double decimal);
ast_node_t* ast_create_character(char character);
ast_node_t* ast_create_string(const char* string);

ast_node_t* ast_create_unary(const char* operator);
ast_node_t* ast_create_binary(const char* operator);
ast_node_t* ast_create_ternary();

void ast_free(ast_node_t* root);

int ast_print(ast_node_t* root);
