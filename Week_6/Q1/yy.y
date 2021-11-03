%{
   #include <stdio.h>
%}
  
%token    ID
%left    '+' '-'
%left    '*' '/'
%left    UMINUS
  
/* Rule Section */
%%
  
S  :  E
E  :  E'+'{func1();}T{func2();}
   |  E'-'{func1();}T{func2();}
   |  T
   ;
T  :  T'*'{func1();}F{func2();}
   |  T'/'{func1();}F{func2();}
   |  F
   ;
F  :  '('E{func2();}')'
   |  '-'{func1();}F{func2();}
   |  ID{func3();}
   ;
  
%%
  
#include"lex.yy.c"
char stack[100];
int top=0;
  
void yyerror(char * s)
/* yacc error handler */
{  
 fprintf (stderr, "%s\n", s);
}

int main()
{
    printf("Enter infix expression:  "); 
    yyparse();
    printf("\n");
    return 0;
}
func1()
{
    stack[top++]=yytext[0];
}
  
func2()
{
    printf("%c",  stack[--top]);
}
  
func3()
{
    printf("%c", yytext[0]);
}