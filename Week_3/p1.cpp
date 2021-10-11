#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<unordered_map>
#include <stack>
#include <set>
using namespace std;

unordered_map<char,vector<string>> productions;

void lr0ItemsCollection()
{
    int i, j, l;
    vector<string> prod;
    char non_terminal;
    string temp;

    for(auto itr:productions)
    {
        prod = itr.second;
        non_terminal = itr.first;
        for(i=0;i<prod.size();i++)
        {
            if(prod[i]=="^")
            {
                cout << non_terminal << "->.^" << endl;
                continue;
            }
            l = prod[i].length();
            j=0;
            while(j<l)
            {
                temp = prod[i];
                temp.insert(j,".");
                cout << non_terminal << "->" << temp << endl;
                j++;
            }
            cout << non_terminal << "->" << prod[i] << "." << endl;
        }
    }
}

int main(void)
{

    int n, i, j, n_terminals, n_nonTerminals;

    cout << "Enter number of terminals: ";
    cin >> n_terminals;
    cout << "Enter number of non-terminals: ";
    cin >> n_nonTerminals;

    //input all terminals
    vector<char> terminals(n_terminals);
    cout << "Enter the terminals: " << endl;
    for(i=0;i<n_terminals;i++)
    {
        fflush(stdin);
        cin >> terminals[i];
    }

    char start_symbol;
    cout << "Enter the start symbol: ";
    cin >> start_symbol;

    //input grammar
    string grammar;
    cout << "Format of production rules corresponding to each non-terminal as: \"S->AB|a|c\" and use '^' for epsilon" <<endl;
    cout << "Enter the production rules :"<<endl;
    for(i=0;i<n_nonTerminals;i++)
    {
        fflush(stdin);
        cin >> grammar;

        productions.insert(make_pair(grammar[0],vector<string>()));

        string prod;
        
        for(j=3;j<grammar.length();j++)
        {
            if(grammar[j]=='|')
            {
                productions[grammar[0]].push_back(prod);
                prod = "";
            }else{
                prod+=grammar[j];
            }
        }
        productions[grammar[0]].push_back(prod);
    }

    cout << "LR(0) items are: " << endl;
    
    cout << start_symbol << "\'->." << start_symbol<< endl;
    cout << start_symbol << "\'->" << start_symbol << "."<< endl;

    lr0ItemsCollection();
    
    return 0;
}