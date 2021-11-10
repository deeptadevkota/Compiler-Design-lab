#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<algorithm>
#include<iomanip>

using namespace std;

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


int checkIfOperator(char op)
{
    if(op=='+' || op=='-' || op=='*' || op=='/' || op=='^')
        return 1;
    return 0;
}

int main(void)
{
    vector<pair<string,pair<string,pair<string,string>>>> tac;
    int i, j, n_tac;

    cout << "\nEnter number of three address codes: ";
    cin >> n_tac;

    string op, arg1, arg2, result, exp;
    cout << "Enter the three address code: " << endl;
    for(i=0;i<n_tac;i++)
    {
        // cout << "\nOperation: ";
        // cin >> op;

        // cout << "Argument1: ";
        // cin >> arg1;

        // cout << "Argument2: ";
        // cin >> arg2;

        // cout << "Result: ";
        // cin >> result;

        
        fflush(stdin);
        exp = result = op = arg1 = arg2 = "";
        cin >> exp;

        while(exp[0]!='=')
        {
            result.push_back(exp[0]);
            exp.erase(0,1);
        }

        exp.erase(0,1);

        while(!checkIfOperator(exp[0]))
        {
            arg1.push_back(exp[0]);
            exp.erase(0,1);
        }


        op.push_back(exp[0]);
        exp.erase(0,1);

        while(exp.length()>0)
        {
            arg2.push_back(exp[0]);
            exp.erase(0,1);
        }

        //cout << result << " " << op << " " << arg1 << " " << arg2 << endl;

        tac.push_back(make_pair(result, make_pair(op, make_pair(arg1, arg2))));


    }

    vector<pair<string,pair<string,string>>> asmCode;
    int rNo=0;

    for(i=0;i<tac.size();i++)
    {
        result = tac[i].first;
        op = tac[i].second.first;
        arg1 = tac[i].second.second.first;
        arg2 = tac[i].second.second.second;

        asmCode.push_back(make_pair("MOV",make_pair("R"+intToString(rNo),arg1)));   

        arg1 = "R" + intToString(rNo);

        rNo++;

        
        if(op == "+")
        {
            asmCode.push_back(make_pair("ADD",make_pair(arg2,arg1)));   
        }
        else if(op == "-")
        {
            asmCode.push_back(make_pair("SUB",make_pair(arg2,arg1)));   
        }
        else if(op == "*")
        {
            asmCode.push_back(make_pair("MUL",make_pair(arg2,arg1)));  
        }
        else if(op == "/")
        {
            asmCode.push_back(make_pair("DIV",make_pair(arg2,arg1)));    
        }

        asmCode.push_back(make_pair("MOV",make_pair("R"+intToString(rNo),result)));   

        rNo++;

    }

    cout << "\nAssembly code for given three address code is: " << endl;
    for(i=0;i<asmCode.size();i++)
    {
        cout << asmCode[i].first << " " << asmCode[i].second.first << "," << asmCode[i].second.second << endl;
    }

    cout << "\n";



    return 0;
}



/*

Sample Input:

1)
a=b*c
e=a*d

2)
t0=b*c
t1=t0+b


*/