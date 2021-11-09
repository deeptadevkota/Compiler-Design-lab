#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<stack>
#include<algorithm>
#include<iomanip>

using namespace std;

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

string infixToPostfix(string expression)
{
    int i,l;
    stack<char> st;
    string postfixExp="", num;

    l= expression.length();

    for(i=0;i<l;i++)
    {
        if((expression[i]>=65 && expression[i]<=90) || (expression[i]>=97 && expression[i]<=122) || (expression[i]>=48 && expression[i]<=57))
        {
            num.push_back(expression[i]);
        }
        else
        {
            if(num.length()>0)
            {
                postfixExp+=(num+"_");
                num = "";
            }else{
                num.push_back(expression[i]);
                continue;
            }

            while(!st.empty() && precedence(st.top()) < precedence(expression[i]))
            {
                postfixExp.push_back(st.top());
                postfixExp+="_";
                st.pop();
            }

            st.push(expression[i]);
        }
    }

    if(num.length()>0)
    {
        postfixExp+=(num+"_");
        num = "";
    }

    while(!st.empty())
    {
        postfixExp.push_back(st.top());
        postfixExp+="_";
        st.pop();
    }

    return postfixExp;

}

int checkIfOperator(string op)
{
    if(op=="+" || op=="-" || op=="*" || op=="/" || op=="^")
        return 1;
    return 0;
}



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

        if(arg1[0] == '#')
        {
            arg1.erase(0,1);
            arg1 = "t" + arg1;
        }

        if(arg2[0] == '#')
        {
            arg2.erase(0,1);
            arg2 = "t" + arg1;
        }
        cout << left << setw(12) << i << "| " << left << setw(12) << tac[i].first << "| " << left << setw(12) << arg1 << "| " << left << setw(12) << arg2 << "| " << "t" << i <<endl;
    }
    arg1 = tac[i].second.first;
    arg2 = tac[i].second.second;
    if(arg1[0] == '#')
    {
        arg1.erase(0,1);
        arg1 = "t" + arg1;
    }
    cout << left << setw(12)  << i << "| " << left << setw(12) <<  "=" << "| " << left  << setw(12)  << arg1 << "| " << left  << setw(12)  << tac[i].second.second << "| " << lhs <<endl;
    cout << endl;

}

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

        if(arg1[0] == '#')
        {
            arg1.erase(0,1);
            arg1 = "(" + arg1 + ")";
        }

        if(arg2[0] == '#')
        {
            arg2.erase(0,1);
            arg2 = "(" + arg2 + ")";
        }
        
        cout << left << setw(12) << i << "| " << left << setw(12) << tac[i].first << "| " << left << setw(12) << arg1 << "| " << left << setw(12) << arg2 <<endl;

    }

    arg1 = tac[i].second.first;
    arg2 = tac[i].second.second;

    if(arg1[0] == '#')
    {
        arg1.erase(0,1);
        arg1 = "(" + arg1 + ")";
    }
    
    cout << left << setw(12)  << i << "| " << left << setw(12) <<  "=" << "| " << left  << setw(12)  << arg1 << "| " << left  << setw(12)  << lhs <<endl;
    cout << endl;

}

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

        if(arg1[0] == '#')
        {
            arg1.erase(0,1);
            arg1 = "(" + arg1 + ")";
        }

        if(arg2[0] == '#')
        {
            arg2.erase(0,1);
            arg2 = "(" + arg2 + ")";
        }
        
        cout << left << setw(12) << i << "| " << left << setw(12) << tac[i].first << "| " << left << setw(12) << arg1 << "| " << left << setw(12) << arg2 <<endl;

    }

    arg1 = tac[i].second.first;
    arg2 = tac[i].second.second;

    if(arg1[0] == '#')
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

void threeAddressCode(string expression, string lhs)
{
    stack<string> st;
    int eqNo, i, l;

    vector<pair<string,pair<string,string>>> eq;

    string op;

    for(i=0;i<expression.length();i++)
    {
        if(expression[i]=='_')
        {
            if(checkIfOperator(op))
            {
                string op1 = st.top();
                st.pop();
                string op2 = st.top();
                st.pop();

                int expNo;
                pair<string,pair<string,string>> exp = make_pair(op, make_pair(op2,op1));
                if(find(eq.begin(),eq.end(),exp) == eq.end())
                {
                    eq.push_back(exp);
                    expNo = eq.size()-1;
                }else{
                    auto itr = find(eq.begin(),eq.end(),exp);
                    expNo = itr - eq.begin();
                }
                
                string eval = "#";
                eval += intToString(expNo);
                
                st.push(eval);
            }
            else if(op[0]=='-' || op[0]=='+')
            {
                string op1 = op;
                op1.erase(0,1);

                int expNo;
                pair<string,pair<string,string>> exp = make_pair("uminus", make_pair(op1,""));
                if(find(eq.begin(),eq.end(),exp) == eq.end())
                {
                    eq.push_back(exp);
                    expNo = eq.size()-1;
                }else{
                    auto itr = find(eq.begin(),eq.end(),exp);
                    expNo = itr - eq.begin();
                }
                
                string eval = "#";
                eval += intToString(expNo);
                st.push(eval);

            }
            else
            {
                st.push(op);
            }
            op="";
        }else{
            op.push_back(expression[i]);
        }
    }

    eq.push_back(make_pair("",make_pair(st.top(),"")));
    st.pop();

    cout << endl;

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