#include "mlang/containers/ast.h"

#include "mlang/debug/mem.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

static void print_tree_rec(node_t *head, int height);
static void print_indent(int indent);

typedef struct node {
    const char *value;
    node_t *left;
    node_t *right;
} node_t;

node_t *create_node(const char *value) { 
    node_t *node = xmalloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->value = value;
    return node;
}

void destroy_node(node_t **rootptr) {
    node_t *root = *rootptr;
    if(root == NULL) return;

    if(root->left)
        destroy_node(&root->left);
    if(root->right)
        destroy_node(&root->right);

    xfree(root, sizeof(node_t));
    (*rootptr) = NULL;
}

bool insert_node(node_t **rootptr, const char *value) {
    node_t *root = *rootptr;
    if(root == NULL) { 
        *rootptr = create_node(value);
        return true;
    };

    // Temporary
    int status = strcmp(value, root->value);
    if(status == 0) {
        return false;
    } else if(status < 0) { 
        return insert_node(&(root->left), value);
    } else {
        return insert_node(&(root->right), value);
    }
}

void print_tree(node_t *head) {
    print_tree_rec(head, 0);
}

void print_tree_rec(node_t *head, int height) {
    if(head == NULL) {
        return;
    }

    if(head->left)
        print_tree_rec(head->left, height + 1);
    print_indent(height);
    printf("%s\n", head->value);
    if(head->right)
        print_tree_rec(head->right, height + 1);

}

void print_indent(int indent) {
    for(int i = 0; i < indent; i++) {
        printf("\t");
    }
}