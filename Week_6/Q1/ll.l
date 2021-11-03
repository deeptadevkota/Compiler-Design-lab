letter [A-Za-z]
digit [0-9]
  
/* Rule Section */
%%
{letter}({letter}|{letter})*  return ID;
{digit}+                      {yylval=atoi(yytext); return ID;}
[\n\t]                        yyterminate();
.                             return yytext[0];
%%