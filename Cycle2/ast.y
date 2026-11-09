%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

int yylex(void);
void yyerror(const char *s);
void printTree(node *t, int depth);
%}

%token NUM ID
%left '+' '-'
%left '*' '/'

%%
start : E     { printf("Abstract Syntax Tree:\n"); printTree($1, 0); }
      ;

E : E '+' E     { $$ = mknode("+", $1, $3); }
  | E '-' E     { $$ = mknode("-", $1, $3); }
  | E '*' E     { $$ = mknode("*", $1, $3); }
  | E '/' E     { $$ = mknode("/", $1, $3); }
  | '(' E ')'   { $$ = $2; }
  | NUM         { $$ = $1; }
  | ID          { $$ = $1; }
  ;
%%

node *mknode(char *label, node *l, node *r) {
    node *n = (node *)malloc(sizeof(node));
    n->label = strdup(label);
    n->left = l;
    n->right = r;
    return n;
}

void printTree(node *t, int depth) {
    if (t == NULL) return;
    for (int i = 0; i < depth; i++)
        printf("    ");
    printf("%s\n", t->label);
    printTree(t->left, depth + 1);
    printTree(t->right, depth + 1);
}

int main() {
    printf("Enter expression: ");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    printf("Invalid expression\n");
}
