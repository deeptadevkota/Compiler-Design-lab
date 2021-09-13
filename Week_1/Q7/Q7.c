#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//keywords in given language
char keywords[9][10] = {"print", "int", "if", "else", "then", "endif", "while", "do", "enddo"};

//check if current token is a keyword
int isKeywords(char *keyword)
{
    int i;
    for (i = 0; i < 9; i++)
    {
        if (!strcmp(keywords[i], keyword))
            return 1;
    }
    return 0;
}

//check if current token is identifier
int isIdentifier(char *identifier)
{
    int i, l;
    if (!isalpha(identifier[0]))
        return 0;
    l = strlen(identifier);
    for (i = 0; i < l; i++)
    {
        if (!isalnum(identifier[i]))
            return 0;
    }
    return 1;
}

//check if current token is constant
int isConstant(char *constant)
{
    int i, l;
    l = strlen(constant);
    for (i = 0; i < l; i++)
    {
        if (!isdigit(constant[i]))
            return 0;
    }
    return 1;
}

//check if current token is an operator
int isOperator(char op)
{
    switch (op)
    {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 1;
    case '/':
        return 1;
    case '>':
        return 1;
    case '<':
        return 1;
    }
    return 0;
}

//check if current token ia a relational operator
int isRelOperator(char *op)
{
    switch (op[0])
    {
    case '>':
        if (op[1] == '=')
            return 1;
        break;
    case '<':
        if (op[1] == '=')
            return 1;
        break;
    case '!':
        if (op[1] == '=')
            return 1;
        break;
    case '=':
        if (op[1] == '=')
            return 1;
        break;
    }
    return 0;
}

//check if current token is assignment operator
int isAssigment(char op)
{
    return op == '=' ? 1 : 0;
}

//fuction to check token type
void checkToken(char *token)
{
    if (isKeywords(token))
        printf("%s : is a keyword\n", token);
    else if (isIdentifier(token))
        printf("%s : is an identifier\n", token);
    else if (isConstant(token))
        printf("%s : is a constant\n", token);
    else
    {
        printf("hello1");
        printf("%s : is not a valid token\n", token);
    }
}

int main()
{
    char str[100000], op[2], curr[100], line[1000], num, c;
    long int i, comment, l, j;
    FILE *file;

    //open text file with c program
    file = fopen("sample.txt", "r");

    //check if error in opening file
    if (file == NULL)
    {
        printf("Error\n");
        exit(0);
    }

    //copy file contains to a string
    c = fgetc(file);
    i = j = 0;
    while (c != EOF)
    {
        str[i++] = c;
        c = fgetc(file);
    }

    i = 0;
    l = strlen(str); //length of string
    comment = 0;     //keep track of comments
    while (i < l)
    {
        if (comment == 1) //current token is part of comment
        {
            if (str[i] == '*' && i + 1 < l && str[i + 1] == '/')
            {
                i++;
                comment = 0;
            }
        }
        else if (str[i] == '/' && i + 1 < l && str[i + 1] == '*') //comment started
        {
            comment = 1;
            i++;
        }
        else if (str[i] == '{' || str[i] == '(' || str[i] == '\n' || str[i] == ' ')
        {
            if (strlen(curr) > 0)
            {
                curr[j] = '\0';
                checkToken(curr);
                j = 0;
                curr[j] = '\0';
            }
            i++;
            continue;
        }
        else if (str[i] == '[' || str[i] == ']' || str[i] == '}' || str[i] == ')' || str[i] == ' ' || str[i] == ';' || str[i] == ',')
        {
            if (strlen(curr) > 0)
            {
                curr[j] = '\0';
                checkToken(curr);
                j = 0;
                curr[j] = '\0';
            }
        }
        else if (isalnum(str[i]))
        {
            curr[j++] = str[i];
        }
        else if (isOperator(str[i]))
        {
            if (strlen(curr) > 0)
            {
                curr[j] = '\0';
                checkToken(curr);
                j = 0;
                curr[j] = '\0';
            }
            if (i < l - 1)
            {
                op[0] = str[i];
                op[1] = str[i + 1];
                if (isRelOperator(op))
                {
                    printf("%s : is an operator\n", op);
                    i++;
                }
                else
                    printf("%c : is an operator\n", op[0]);
            }
            else
                printf("%c : is an operator\n", str[i]);
        }
        else if (isAssigment(str[i]))
        {
            if (strlen(curr) > 0)
            {
                curr[j] = '\0';
                checkToken(curr);
                j = 0;
                curr[j] = '\0';
            }
            printf("%c : is an operator\n", str[i]);
        }
        else if (str[i] != '#' && str[i] != '\t' && str[i] != '\0' && str[i] != 0)
        {
            printf("%c : is not a valid operator\n", str[i]);
        }
        i++;
    }
    fclose(file);
    return 0;
}