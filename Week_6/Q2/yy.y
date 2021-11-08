%{
 #include <stdio.h>
 #include "lex.yy.c"
 int yylex(void);
 int yyerror(char *s);

 struct Node *root;
%}

%token INTEGER
%left '+' '-'
%left '*' '/'


%%

prog: |prog expr '\n';

expr: | additiveExpr {printf("Value is: %d\n\n", $1)};

additiveExpr: additiveExpr '+' multiplicativeExpr {$$ = $1+$3; printf("node: {op = +}\n");}
            | additiveExpr '-' multiplicativeExpr {$$ = $1-$3; printf("node: {op = -}\n");}
            | multiplicativeExpr;

multiplicativeExpr: multiplicativeExpr '*' primaryExpr {$$ = $1*$3; printf("node: {op = *}\n");}
                  | multiplicativeExpr '/' primaryExpr {$$ = $1/$3; printf("node: {op = /}\n");}
                  | primaryExpr;

primaryExpr: parenthesis
           | primary;

parenthesis: '(' additiveExpr ')' {$$ = $2; printf("leaf: {value = %d}\n",$2);};

primary: INTEGER {$$ = $1; printf("leaf: {value = %d}\n",$1);};


%%


int yyerror(char *s) {
 printf("Error oocurred\n");
 return 0;
}


int main(void) {
 yyparse();
 return 0;
} 