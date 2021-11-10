#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<stack>
#include<algorithm>
#include<iomanip>

using namespace std;


//fucntion check precedence of operator
int precedence(char op)
{
    switch(op)
    {
        case '^' : return 1;
        case '*' : return 2;
        case '/' : return 2;
        case '+' : return 3;
        case '-' : return 3;
    }
    return -1;
}

//fucntion to convert infix to postfix
string infixToPostfix(string expression)
{
    int i,l;
    stack<char> st;
    string postfixExp="", num;

    l = expression.length();

    for(i=0;i<l;i++)
    {
        if((expression[i]>=65 && expression[i]<=90) || (expression[i]>=97 && expression[i]<=122) || (expression[i]>=48 && expression[i]<=57)) 
        {
            num.push_back(expression[i]);          //current symbol is part of operand
        }
        else                                       //current symbol is operator
        {
            if(num.length()>0)                    //append the operand along with '_' as delimitor to postfix expression
            {
                postfixExp+=(num+"_");
                num = "";
            }
            else                                  // current operator is unary opeartor
            {
                num.push_back(expression[i]);     // append the operator along with operand to postfix expression
                continue;
            }

            while(!st.empty() && precedence(st.top()) < precedence(expression[i]))  //pop the operators of higher preference than current operator from stack
            {
                postfixExp.push_back(st.top());
                postfixExp+="_";       //append the operator along with '_' as delimitor to postfix expression
                st.pop();
            }

            st.push(expression[i]);    //push the current operator to stack
        }
    }

    if(num.length()>0)        //push the last operand to postfix expression along with '_'
    {
        postfixExp+=(num+"_");
        num = "";
    }

    while(!st.empty())          //empty the stack by poping operators and appending to the postfix expression along with '_'
    {
        postfixExp.push_back(st.top());
        postfixExp+="_";
        st.pop();
    }

    return postfixExp;

}


//fucntion to check if symbol/string is operator or operand
int checkIfOperator(string op)
{
    if(op=="+" || op=="-" || op=="*" || op=="/" || op=="^")
        return 1;
    return 0;
}


//function to represent three address code in quadrapules
void quadrapuleRepresentation(vector<pair<string,pair<string,string>>> tac, string lhs)
{

    int i;
    vector<vector<string>> quadrapule;
    string arg1, arg2;

    cout << "\n\nQuadrapules representation:\n"<<endl;
    cout << left << setw(12) << "#" << "| " <<  left << setw(12) << "operation" << "| " << left << setw(12) << "arg1" << "| " << left << setw(12) << "arg2" << "| " << left << setw(12) <<"result" << endl;
    cout << "------------|" << "-------------|" << "-------------|"  << "-------------|"  <<"--------------" << endl;
    for(i=0;i<tac.size()-1;i++)
    {
        arg1 = tac[i].second.first;
        arg2 = tac[i].second.second;
        
    
        if(arg1[0] == '#')  //arg1 is temporary variable
        {
            arg1.erase(0,1);
            arg1 = "t" + arg1;
        }

        if(arg2[0] == '#')  //arg2 is temporary variable
        {
            arg2.erase(0,1);
            arg2 = "t" + arg1;
        }
        cout << left << setw(12) << i << "| " << left << setw(12) << tac[i].first << "| " << left << setw(12) << arg1 << "| " << left << setw(12) << arg2 << "| " << "t" << i <<endl;
    }
    arg1 = tac[i].second.first;
    arg2 = tac[i].second.second;
    if(arg1[0] == '#')    //arg1 is temporary variable
    {
        arg1.erase(0,1);
        arg1 = "t" + arg1;
    }
    cout << left << setw(12)  << i << "| " << left << setw(12) <<  "=" << "| " << left  << setw(12)  << arg1 << "| " << left  << setw(12)  << tac[i].second.second << "| " << lhs <<endl;
    cout << endl;

}

//function to represent three address code in triples
void triplesRepresentation(vector<pair<string,pair<string,string>>> tac, string lhs)
{
    int i;
    vector<vector<string>> quadrapule;
    string arg1, arg2;

    cout << "\n\nTriples representation:\n"<<endl;
    cout << left << setw(12) << "#" << "| " <<  left << setw(12) << "operation" << "| " << left << setw(12) << "arg1" << "| " << left << setw(12) << "arg2" << endl;
    cout << "------------|" << "-------------|" << "-------------|"  << "--------------"  << endl;
    for(i=0;i<tac.size()-1;i++)
    {
        
        arg1 = tac[i].second.first;
        arg2 = tac[i].second.second;

        if(arg1[0] == '#')     //arg1 is temporary variable
        {
            arg1.erase(0,1);
            arg1 = "(" + arg1 + ")";
        }

        if(arg2[0] == '#')    //arg2 is temporary variable
        {
            arg2.erase(0,1);
            arg2 = "(" + arg2 + ")";
        }
        
        cout << left << setw(12) << i << "| " << left << setw(12) << tac[i].first << "| " << left << setw(12) << arg1 << "| " << left << setw(12) << arg2 <<endl;

    }

    arg1 = tac[i].second.first;
    arg2 = tac[i].second.second;

    if(arg1[0] == '#')   //arg1 is temporary variable
    {
        arg1.erase(0,1);
        arg1 = "(" + arg1 + ")";
    }
    
    cout << left << setw(12)  << i << "| " << left << setw(12) <<  "=" << "| " << left  << setw(12)  << arg1 << "| " << left  << setw(12)  << lhs <<endl;
    cout << endl;

}

//function to represent three address code in indirect triples
void indirectTriplesRepresentation(vector<pair<string,pair<string,string>>> tac, string lhs)
{
    int i;
    vector<vector<string>> quadrapule;
    string arg1, arg2;

    vector<pair<string,pair<string,string>>*> addressTable;

    for(i=0;i<tac.size();i++)
    {
        addressTable.push_back(&tac[i]);
    }

    cout << "\n\nIndirect Triples representation:\n"<<endl;
    cout << left << setw(12) << "#" << "| " <<  left << setw(12) << "operation" << "| " << left << setw(12) << "arg1" << "| " << left << setw(12) << "arg2" << endl;
    cout << "------------|" << "-------------|" << "-------------|"  << "--------------"  << endl;
    for(i=0;i<tac.size()-1;i++)
    {
        
        arg1 = tac[i].second.first;
        arg2 = tac[i].second.second;
  
        if(arg1[0] == '#')       //arg1 is temporary variable
        {
            arg1.erase(0,1);
            arg1 = "(" + arg1 + ")";
        }

        if(arg2[0] == '#')     //arg2 is temporary variable
        {
            arg2.erase(0,1);
            arg2 = "(" + arg2 + ")";
        }
        
        cout << left << setw(12) << i << "| " << left << setw(12) << tac[i].first << "| " << left << setw(12) << arg1 << "| " << left << setw(12) << arg2 <<endl;

    }

    arg1 = tac[i].second.first;
    arg2 = tac[i].second.second;

    if(arg1[0] == '#')       //arg1 is temporary variable
    {
        arg1.erase(0,1);
        arg1 = "(" + arg1 + ")";
    }
    
    cout << left << setw(12)  << i << "| " << left << setw(12) <<  "=" << "| " << left  << setw(12)  << arg1 << "| " << left  << setw(12)  << lhs <<endl;
    cout << endl;

    cout << "Pointer table:"<<endl;
    cout << left << setw(10) << "Index" << "|" << left << setw(15) << "Address" << endl;
    cout << "----------|" <<  "---------------" << endl;
    for(i=0;i<addressTable.size();i++)
    {
        cout << left << setw(10) << i << "|" << left << setw(15) << addressTable[i] << endl;
    }
    cout << endl;

}

//fucntion to convert integer to corresponding string
string intToString(int num)
{
    int i,l;
    string s;
    if(num==0)
        return "0";
    while(num>0)
    {
        s.push_back(48+num%10);
        num/=10;
    }
    l=s.length();
    char temp;
    for(i=0;i<l/2;i++)
    {
        temp = s[i];
        s[i] = s[l-i-1];
        s[l-i-1] = temp;
    }
    return s;
}

//fucntion to generate three address code for given expression
void threeAddressCode(string expression, string lhs)
{
    stack<string> st;
    int eqNo, i, l;

    vector<pair<string,pair<string,string>>> eq;

    string op;

    for(i=0;i<expression.length();i++)
    {
        if(expression[i]=='_')       // current synbol is delimiter
        {
            if(checkIfOperator(op))     //check if current symbol is operand or operator
            {

                //pop the top two operators from stack
                string op1 = st.top();   
                st.pop();
                string op2 = st.top();
                st.pop();

                int expNo;
                pair<string,pair<string,string>> exp = make_pair(op, make_pair(op2,op1));
                if(find(eq.begin(),eq.end(),exp) == eq.end())    //check if the expression is already present in the three address code
                {
                    eq.push_back(exp);
                    expNo = eq.size()-1;
                }else{
                    auto itr = find(eq.begin(),eq.end(),exp);
                    expNo = itr - eq.begin();
                }
                
                string eval = "#";      //to indicate that operator is temporary vaiable
                eval += intToString(expNo);
                
                st.push(eval);
            }
            else if(op[0]=='-' || op[0]=='+')    //unary operator with operand
            {
                string op1 = op;
                op1.erase(0,1);

                int expNo;
                pair<string,pair<string,string>> exp = make_pair("uminus", make_pair(op1,""));
                if(find(eq.begin(),eq.end(),exp) == eq.end())   //check if already present in three address code
                {
                    eq.push_back(exp);
                    expNo = eq.size()-1;
                }else{
                    auto itr = find(eq.begin(),eq.end(),exp);
                    expNo = itr - eq.begin();
                }
                
                string eval = "#";      //to indicate that operator is temporary vaiable
                eval += intToString(expNo);
                st.push(eval);

            }
            else        //in case of operator push to stack
            {
                st.push(op);
            }
            op="";
        }
        else
        {
            op.push_back(expression[i]);
        }
    }
    

    eq.push_back(make_pair("",make_pair(st.top(),"")));
    st.pop();

    cout << endl;

    //printing the three address code
    cout << "Three Address Code : " << endl;
    string arg1, arg2;
    for(i=0;i<eq.size()-1;i++)
    {
        arg1 = eq[i].second.first;
        arg2 = eq[i].second.second;

        if(arg1[0] == '#')
        {
            arg1.erase(0,1);
            arg1 = "t" + arg1;
        }

        if(arg2[0] == '#')
        {
            arg2.erase(0,1);
            arg2 = "t" + arg2;
        }

        if(eq[i].first == "uminus")
        {
            cout << "t" << i << "=" << "-" << arg1 << endl;
        }
        else
        {
            cout << "t" << i << "=" << arg1 << eq[i].first << arg2 << endl;
        }

    }

    quadrapuleRepresentation(eq,lhs);

    triplesRepresentation(eq,lhs);

    indirectTriplesRepresentation(eq,lhs);

}


int main(void)
{
    int i, j;
    string input;

    cout << "\nEnter input expression: ";
    cin >> input;

    string expression = input;
    string lhs;
    while(expression[0] != '=')
    {
        lhs.push_back(expression[0]);
        expression.erase(0,1);
    }

    expression.erase(0,1);

    string postFix = infixToPostfix(expression);

    threeAddressCode(postFix,lhs);

    return 0;
}



/*

Sample Input:
 1. a=b*c+d
 2. a=-b*c+d
 3. a=b*c+b*c
 4. a=b*–c+b*–c

*/