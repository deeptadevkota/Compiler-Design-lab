

/*Definition Section*/
%{
#include<stdio.h>
%}


/* Rules Section*/
%%

[\t" "]+ {fprintf(yyout," ");	}	
.|\n {fprintf(yyout,"%s",yytext);	}

%%

/*call the yywrap function*/
int yywrap()
{
return 1;
}

int main(void)
{
	yyin=fopen("input.txt","r");		
	yyout=fopen("Output_file.txt","w");
	yylex();
	return 0;
}
