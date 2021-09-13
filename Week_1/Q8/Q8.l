%{
%}

/* defining the identifier */
identifier [a-zA-Z][a-zA-Z0-9]*


/* Rules section */
%%
#.* { printf("%s is a preprocessor directive\n",yytext);}
";"|"("|")"|"{"|"}"|"["|"]"|[\t\n]+ {}
int|float|char|double|while|for|do|if|break|continue|void|switch|case|long|struct|const|typedef|return|else|goto {printf("%s is a keyword\n",yytext);}
"//".*\n {printf("%s is a single line comment\n",yytext);}
"/*"[^*/]*"*/" {printf("%s is a multiline comment\n",yytext);}
{identifier} {printf("%s is an identifier\n",yytext);}
[0-9]+ {printf("%s is a number\n",yytext);}
= {printf("%s is assignment operator\n",yytext);}
"<="|">="|"<"|"=="|">" {printf("%s is a relational operator",yytext);}
"+"|"-"|"/"|"*" {printf("%s is arithmetic operator",yytext);}

%%


int main(int argc,char **argv)
{
	yyin=fopen("input.txt","r");	
	yylex();
	printf("\n\n");
} 

