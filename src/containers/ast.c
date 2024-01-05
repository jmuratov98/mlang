#include "containers/ast.h"

#include <stdlib.h>
#include <stdio.h>

#define ASSERT(cond) {                                                                      \
    if((cond)) {}                                                                            \
    else {                                                                                  \
        printf("Assertion Failed (%s) at (%s %d)", #cond, __FILE__, __LINE__);             \
    }                                                                                       \
}

static ast_node_t* ast_create_statement(ast_node_kind_e kind);

ast_node_t* ast_create_translation_unit() {
    return ast_create_statement(AST_TAG_TRANSLATION_UNIT);
}

ast_node_t* ast_create_import_statement() {
    return ast_create_statement(AST_TAG_IMPORT_STATEMENT);
}

// TODO: Finish the rest of the statements

ast_node_t* ast_create_function_definition() {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_FUNCTION_DEFINITION;
    node->function.name = NULL;
    node->function.arguments = array_create(ast_node_t*);
    node->function.statement = NULL;
    
    return node;
}

ast_node_t* ast_create_identifier(const char* identifier) {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_IDENTIFIER;
    node->identifier_name = identifier;

    return node;
}

ast_node_t* ast_create_keyword(const char* keyword) {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_KEYWORD;
    node->keyword = keyword;

    return node;
}

ast_node_t* ast_create_integer(int64_t integer) {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_CONSTANT_INT;
    node->integer_constant = integer;

    return node;
}

ast_node_t* ast_create_decimal(double decimal) {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_CONSTANT_FLOAT;
    node->decimal_constant = decimal;

    return node;
}

ast_node_t* ast_create_character(char character) {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_CONSTANT_CHAR;
    node->character_constant = character;

    return node;
}

ast_node_t* ast_create_string(const char* string) {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_CONSTANT_STRING;
    node->string_constant = string;

    return node;
}

ast_node_t* ast_create_unary(const char* operator) {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_UNARY_OPERATOR;
    node->unexp.operator = operator;
    node->unexp.operand = NULL;

    return node;
}

ast_node_t* ast_create_binary(const char* operator) {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_BINARY_OPERATOR;
    node->biexp.operator = operator;
    node->biexp.left = NULL;
    node->biexp.right = NULL;

    return node;
}

ast_node_t* ast_create_ternary() {
    ast_node_t* node = malloc(sizeof(ast_node_t));
    ASSERT(node);

    node->tag = AST_TAG_BINARY_OPERATOR;
    node->ternexp.left = NULL;
    node->ternexp.mid = NULL;
    node->ternexp.right= NULL;

    return node;
}

void ast_free(ast_node_t* root) {
    if(!root) return;

    switch (root->tag)
    {
    case AST_TAG_TRANSLATION_UNIT:
    case AST_TAG_IMPORT_STATEMENT:
    case AST_TAG_UNKNOWN:
        for(size_t i = 0; i < array_size(root->children); i++) 
            ast_free(root->children[i]);
        array_free(root->children);
        break;

    case AST_TAG_UNARY_OPERATOR:
        ast_free(root->unexp.operand);
        break;

    case AST_TAG_BINARY_OPERATOR:
        ast_free(root->biexp.left);
        ast_free(root->biexp.right);
        break;

    case AST_TAG_TERNARY_OPERATOR:
        ast_free(root->ternexp.left);
        ast_free(root->ternexp.mid);
        ast_free(root->ternexp.right);
        break;

    case AST_TAG_FUNCTION_DEFINITION:
    case AST_TAG_FUNCTION_DECLARATION:
    case AST_TAG_FUNCTION_CALL:
        for(size_t i = 0; i < array_size(root->function.arguments); i++) 
            ast_free(root->function.arguments[i]);
        array_free(root->function.arguments);
        ast_free(root->function.statement);
        break;

    case AST_TAG_KEYWORD:
    case AST_TAG_IDENTIFIER:
    case AST_TAG_CONSTANT_INT:
    case AST_TAG_CONSTANT_FLOAT:
    case AST_TAG_CONSTANT_CHAR:
    case AST_TAG_CONSTANT_STRING:
        ast_free(root);
        return;
    
    default:
        printf("Unknown Tag recieved: %d\n", root->tag);
        break;
    }

    free(root);
}

int print_tab(int indent) {
    int result = 0;
    for(; indent != 0; indent--) {
        result += printf("\t");
    }
    return result;
}

// TODO: Temp .... Change this later
int ast_print_rec(ast_node_t* root, int indent) {
    if(!root) return 0;

    int result = 0;
    switch(root->tag) {
        case AST_TAG_TRANSLATION_UNIT:
            result += printf("TranslationUnit\n");
            for(size_t i = 0; i < array_size(root->children); i++)
                result += ast_print_rec(root->children[i], indent + 1);
            break;

        case AST_TAG_IMPORT_STATEMENT:
            result += print_tab(indent);
            result += printf("ImportStatement\n");
            for(size_t i = 0; i < array_size(root->children); i++)
                result += ast_print_rec(root->children[i], indent + 1);
            break;
        
        case AST_TAG_UNARY_OPERATOR:
            result += print_tab(indent);
            result += printf("UnaryExpression\n");
            result += ast_print_rec(root->unexp.operand, indent + 1);
            break;
        
        case AST_TAG_BINARY_OPERATOR:
            result += print_tab(indent);
            result += printf("BinaryExpression\n");
            result += ast_print_rec(root->biexp.left, indent + 1);
            result += ast_print_rec(root->biexp.right, indent + 1);
            break;
        
        case AST_TAG_TERNARY_OPERATOR:
            result += print_tab(indent);
            result += printf("TernaryExpression\n");
            result += ast_print_rec(root->ternexp.left, indent + 1);
            result += ast_print_rec(root->ternexp.mid, indent + 1);
            result += ast_print_rec(root->ternexp.right, indent + 1);
            break;

        case AST_TAG_FUNCTION_DEFINITION:
            const char* name = "FunctionDefinition";
            goto function_label;
        case AST_TAG_FUNCTION_DECLARATION:
            name = "FunctionDeclaration";
            goto function_label;
        case AST_TAG_FUNCTION_CALL:
            name = "FunctionCall";
function_label:
            result += print_tab(indent);
            result += printf("%s\n", name);
            for(size_t i = 0; i < array_size(root->function.arguments); i++)
                result += ast_print_rec(root->function.arguments[i], indent + 1);
            result += ast_print_rec(root->function.statement, indent + 1);

        case AST_TAG_KEYWORD:
            result += print_tab(indent);
            result += printf("Keyword\n");
            break;
        case AST_TAG_IDENTIFIER:
            result += print_tab(indent);
            result += printf("Identifier\n");
            break;
        case AST_TAG_CONSTANT_INT:
            result += print_tab(indent);
            result += printf("ConstantInteger\n");
            break;
        case AST_TAG_CONSTANT_FLOAT:
            result += print_tab(indent);
            result += printf("ConstantFloat\n");
            break;
        case AST_TAG_CONSTANT_CHAR:
            result += print_tab(indent);
            result += printf("ConstantChar\n");
            break;
        case AST_TAG_CONSTANT_STRING:
            result += print_tab(indent);
            result += printf("ConstantString\n");
            break;
    }

    return result;
}

int ast_print(ast_node_t* root) {
    return ast_print_rec(root, 0);
}




ast_node_t* ast_create_statement(ast_node_kind_e tag) {
    ast_node_t* statement = malloc(sizeof(ast_node_t));
    ASSERT(statement != NULL);

    statement->tag = tag;
    statement->children = array_create(ast_node_t*);

    return statement;
}
