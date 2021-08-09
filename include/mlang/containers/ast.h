#pragma once

#include "mlang/common.h"

#include <stdbool.h>

typedef struct node node_t;

node_t *create_node(const char *value);
void destroy_node(node_t **head);

bool insert_node(node_t **head, const char *value);
bool remove_node(node_t *head, const char *value);

void print_tree(node_t *head);