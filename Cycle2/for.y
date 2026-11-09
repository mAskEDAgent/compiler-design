%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s);
%}

%token FOR ID NUM REL INC DEC
%left '+' '-'
%left '*' '/'

%%
start   : forstmt
        ;

forstmt : FOR '(' init ';' cond ';' update ')' body
        ;

init    : ID '=' expr
        |
        ;

cond    : expr REL expr
        |
        ;

update  : ID INC
        | ID DEC
        | ID '=' expr
        |
        ;

body    : item
        | '{' list '}'
        ;

list    : list item
        |
        ;

item    : simple
        | forstmt
        ;

simple  : ID '=' expr ';'
        | ';'
        ;

expr    : expr '+' expr
        | expr '-' expr
        | expr '*' expr
        | expr '/' expr
        | ID
        | NUM
        ;
%%

int main() {
    printf("Enter a FOR statement (end with Ctrl+D):\n");
    if (yyparse() == 0)
        printf("Valid FOR statement\n");
    return 0;
}

void yyerror(const char *s) {
    printf("Invalid FOR statement\n");
}
