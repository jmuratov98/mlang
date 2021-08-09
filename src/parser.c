#include "mlang/parser.h"

#include "mlang/containers/vector.h"

static void statement(parser_t *parser, token_t **tokens, token_t *token);
static void expression_statement(parser_t *parser, token_t **tokens, token_t *token);

void parser_init(parser_t *parser) {
    parser->ast = NULL;
}

void parser_destroy(parser_t *parser) {
    destroy_node(&(parser->ast));
}

void parse(parser_t *parser, token_t **tokens) {
    for(int i = 0; i < vector_size(tokens); i++) {
        statement(parser, tokens, tokens[i]);
    }
}

void statement(parser_t *parser, token_t **tokens, token_t *token) {
    expression_statement(parser, tokens, token);
}

void expression_statement(parser_t *parser, token_t **tokens, token_t *token) {
    if(strcmp(token->value), ";") return;
}