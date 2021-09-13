

/*Defination section*/

%{
int no_lines;
int no_chars;
%}


/*Rules section*/
%%

\n {no_lines++; }
. { no_chars++;}

%%

int main()
{
    yylex();
    
    yyout=fopen("Output_file.txt","w");
    fprintf(yyout,"The number of lines is %d",no_lines);
    fprintf(yyout,"The number of characters is %d",no_chars);
    fclose(yyout);
    
    return 0;
}

