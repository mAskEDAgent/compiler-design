%{
#include <stdio.h>
%}

%%
[01]+         { printf("%s -> VALID\n", yytext); }
[^ \t\n]+     { printf("%s -> INVALID\n", yytext); }
[ \t\n]+      ;
%%

int yywrap() { return 1; }

int main() {
    yylex();
    return 0;
}
