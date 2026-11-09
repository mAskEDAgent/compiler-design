%{
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE double
#include "y.tab.h"
%}

%%
[0-9]+(\.[0-9]+)?    { yylval = atof(yytext); return NUMBER; }
[ \t]                ;
\n                   { return 0; }
.                    { return yytext[0]; }
%%

int yywrap() { return 1; }