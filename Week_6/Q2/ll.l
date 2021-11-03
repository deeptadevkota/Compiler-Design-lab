%{

 #include <stdlib.h>
 int yyerror(char *);
 #include "yy.tab.h"


%}

number ([0-9])+
operator [()+-/*\n]

%%
 

{number}   {yylval = atoi(yytext); return (INTEGER);}


{operator}     { return (*yytext); }
 
. yyerror("invalid character");

%%


int yywrap(void) {
 return 1;
} 