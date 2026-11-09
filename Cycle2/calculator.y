%{
#include <stdio.h>
#define YYSTYPE double
int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER
%left '+' '-'
%left '*' '/'

%%
line : expr                   { printf("Result = %g\n", $1); }
     ;

expr : expr '+' expr          { $$ = $1 + $3; }
     | expr '-' expr          { $$ = $1 - $3; }
     | expr '*' expr          { $$ = $1 * $3; }
     | expr '/' expr          { if ($3 == 0) {
                                    printf("Error: division by zero\n");
                                    $$ = 0;
                                } else
                                    $$ = $1 / $3; }
     | '(' expr ')'           { $$ = $2; }
     | NUMBER                 { $$ = $1; }
     ;
%%

int main() {
    printf("Enter expression: ");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    printf("Error: invalid expression\n");
}
