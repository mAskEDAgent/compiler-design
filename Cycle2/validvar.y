%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s);
%}

%token LETTER DIGIT

%%
var : LETTER
    | var LETTER
    | var DIGIT
    ;
%%

int main() {
    printf("Enter a variable: ");
    if (yyparse() == 0)
        printf("Valid variable\n");
    return 0;
}

void yyerror(const char *s) {
    printf("Invalid variable\n");
}
