#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<stack>
#include<algorithm>
#include<unordered_map>

using namespace std;

unordered_map<char,unordered_map<char,string>> parsing_table;

int main(void)
{
    int i,j,n_t,n_nt;
    vector<char> terminals, non_terminals;
    unordered_map<char,string> terminal_map;
    char start_symbol;

    //input
    cout << "Enter number of terminals: ";
    cin >> n_t;
    terminals.resize(n_t);

    cout << "Enter number of non-terminals: ";
    cin >> n_nt;
    non_terminals.resize(n_nt);

    cout << "Enter the terminals: "<< endl;
    for(i=0;i<n_t;i++)
    {    
        fflush(stdin);
        cin >>terminals[i];
        terminal_map.insert(make_pair(terminals[i],""));
    }

    terminal_map.insert(make_pair('$',""));

    cout << "Enter the non-terminals: "<< endl;
    for(i=0;i<n_nt;i++)
    {    
        fflush(stdin);
        cin >>non_terminals[i];
    }

    cout << "Enter the start symbol: ";
    cin >> start_symbol;

    //parsing table input
    string production;
    cout << "Enter the parsing table for each nonterminal and terminal, enter '-' in case of no production" << endl;
    for(i=0;i<n_nt;i++)
    {
        parsing_table.insert(make_pair(non_terminals[i],terminal_map));
        for(auto itr:terminal_map)
        {
            cout << non_terminals[i] << ", " << itr.first << " : ";
            fflush(stdin);
            cin >> production;
            parsing_table[non_terminals[i]][itr.first] = production;
        }
    }

    string input;
    cout << "Enter the input string: ";
    cin >> input;            //input string
    input += "$";

    stack<char> st;
    vector<string> productions;
    int flag;
    char top_stack;
    st.push('$');
    st.push(start_symbol);

    i=0;
    flag=1;
    //loop runs until stack is not empty or input is not exhausted
    while(!st.empty() && i<input.length())
    {
        top_stack = st.top();
        if(top_stack == input[i])    //top of stack and current input symbol match
        {
            st.pop();
            i++;
        }
        else if(top_stack<65 || top_stack>90)  //top of stack is non_terminal
        {
            flag = 0;
            break;
        }
        else if(parsing_table[top_stack][input[i]] != "-")  
        {
            production = parsing_table[top_stack][input[i]];
            st.pop();
            if(production != "^")     
            {
                for(j=production.length()-1; j>=0;j--)  //push corresponding production to stack in reversed manner
                {
                    st.push(production[j]);
                }
            }
            string str;
            str.push_back(top_stack);
            str += ("->"+production);
            productions.push_back(str);
        }
        else
        {
            flag = 0;
            break;
        }
    }

    if(flag == 0 || !st.empty())
    {
        cout << "String is not accepted by the parser" << endl;
    }else{
        cout << "String is accepted by the parser and the productions used are: " << endl;

        for(i=0;i<productions.size();i++)
        {
            cout << i+1 << " : " << productions[i] << endl;
        }
    }

    return 0;
}