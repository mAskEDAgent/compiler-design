%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s);
%}

%token A B

%%
S : A S B
  | A B
  ;
%%

int main(){
	printf("Enter string : ");
	if (yyparse() == 0){
		printf("Valid string\n");
	}
	return 0;
}

void yyerror(const char *s){
	printf("Invalid String\n");
}
