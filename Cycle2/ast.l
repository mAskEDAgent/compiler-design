%{
#include <stdio.h>
#include "ast.h"
#include "y.tab.h"
%}

%%
[0-9]+(\.[0-9]+)?          { yylval = mknode(yytext, NULL, NULL); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*     { yylval = mknode(yytext, NULL, NULL); return ID; }
[ \t]                      ;
\n                         { return 0; }
.                          { return yytext[0]; }
%%

int yywrap() { return 1; }
