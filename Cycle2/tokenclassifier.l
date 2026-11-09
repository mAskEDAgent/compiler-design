%{
	#include <stdio.h>
%}

%%
"int"|"float"|"if"|"else"|"while"	{ printf("%s --> KEYWORD\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*			{ printf("%s --> IDENTIFIER\n", yytext); }
[0-9]+					{ printf("%s --> INTEGER\n", yytext); }
[0-9]+(\.[0-9]+)			{ printf("%s --> FLOAT\n",yytext); }
"=="|"<="|">="|"!="|[+\-*/<>=]		{ printf("%s --> OPERATOR\n",yytext); }
[;,(){}]    				{ printf("%s --> SEPARATOR\n", yytext); }
[ \t\n]+				;
.    					{ printf("%s --> UNKNOWN\n", yytext); }
%%

int yywrap(){
	return 1;
}

int main(){
	yylex();
	return 0;
}

