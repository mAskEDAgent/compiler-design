#ifndef AST_H
#define AST_H

typedef struct node {
    char *label;
    struct node *left, *right;
} node;

node *mknode(char *label, node *l, node *r);

#define YYSTYPE node *

#endif
