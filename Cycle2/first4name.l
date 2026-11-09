%{
#include <stdio.h>
%}

%%
.*[aA][bB][hH][iI].*    { printf("%s -> REJECTED\n", yytext); }
[^\n]+                  { printf("%s -> ACCEPTED\n", yytext); }
\n                      ;
%%

int yywrap() { return 1; }

int main() {
    yylex();
    return 0;
}
