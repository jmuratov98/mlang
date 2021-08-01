#pragma once

#include "mlang/token_type.h"

#include <stdio.h>

typedef struct {
    token_type_e type;
    char *value;
    int line, char_index;
} token_t;

// Debugging purposes
void print_token(token_t *token);