#pragma once

#include "mlang/token.h"

#include "mlang/containers/ast.h"

typedef struct {
    node_t *ast;
} parser_t;

void parser_init(parser_t *parser);
void parser_destroy(parser_t *parser);

void parse(parser_t *parser, token_t **tokens);