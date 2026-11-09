%{
	#include <stdio.h>
	int vowels = 0;
%}

%%
[aeiouAEIOU]	{vowels++;}
%%

int yywrap(){
	return 1;
}

int main(){
	yylex();
	printf("NUmber of vowels = %d", vowels);
	return 0;
}
