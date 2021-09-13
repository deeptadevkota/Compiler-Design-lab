

/* Rules Section*/
%%
(.)* {printf("%s",yytext);}

%%

int main()
{
    yylex();

    
    return 0;
}
