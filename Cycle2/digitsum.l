%{
	#include <stdio.h>
	int sum = 0;
%}

%%
[0-9]	{ sum += atoi(yytext); }
%%

int yywrap(){
	return 1;
}

int main(){
	yylex();
	printf("The sum  of digits : %d", sum);
	return 1;
}
