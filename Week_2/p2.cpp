#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<iomanip>

using namespace std;

unordered_map<char,vector<char>> first, follow;
unordered_map<char,bool> isEpsilon;
unordered_map<char,vector<string>> production_table;


void printFollow()
{
    int i;

    for(auto itr:follow)
    {
        vector<char> v = itr.second;
        char nt = itr.first;
        sort(v.begin(), v.end());
        vector<char>::iterator it = unique(v.begin(), v.end());
        v.resize(std::distance(v.begin(),it));
        follow[nt] = v;
    }

    cout << "\nFollow:" << endl;
    for(auto itr:follow)
    {
        cout << itr.first << " : ";
        for(i=0;i<itr.second.size();i++)
        {
            cout << itr.second[i] << " ";
        }
        cout << endl;
    }
}

void printFirst()
{
    int i;
    cout << "\nFirst:" << endl;
    for(auto itr:first)
    {
        cout << itr.first << " : ";
        for(i=0;i<itr.second.size();i++)
        {
            cout << itr.second[i] << " ";
        }
        cout << endl;
    }
}

void calculateFirst(char nonTerminal)
{
    int i,j;
    if(first[nonTerminal].size()>0)
        return;
    vector<string> productions;
    productions = production_table.find(nonTerminal)->second;
    vector<char> first_curr;

    for(i=0;i<productions.size();i++)
    {
        if(productions[i]=="^")
        {
            first_curr.push_back('^');
            continue;
        }
        j=0;
        if(productions[i][0] == nonTerminal)
            continue;
        while(j<productions[i].length())
        {
            if(productions[i][j] < 65 || productions[i][j] > 90)  //terminal
            {
                first_curr.push_back(productions[i][j]);
                break;
            }
            else{                                                //non-terminal
                if(first[productions[i][j]].size() == 0)
                    calculateFirst(productions[i][j]);
                for(int k=0;k<first[productions[i][j]].size();k++)
                {
                    if(first[productions[i][j]][k]=='^')
                    {
                        continue;
                    }else{
                        first_curr.push_back(first[productions[i][j]][k]);
                    }
                }
            }
            if(!isEpsilon[productions[i][j]]){
                break;
            }
            j++;
        }
    }
    
    sort(first_curr.begin(),first_curr.end());
    vector<char>::iterator it = unique(first_curr.begin(),first_curr.end());
    first_curr.resize(distance(first_curr.begin(),it));
    first[nonTerminal] = first_curr;

}

void calculateFollow()
{
    int j,l;
    vector<string> productions;
    vector<char> first_curr;
    char nonTerminal;
    for(auto i:production_table)
    {
        productions = i.second;
        nonTerminal = i.first;
        for(j=0;j<productions.size();j++)
        {
            if(productions[j] == "^")   
                continue;
            if(productions[j].length() == 1 && productions[j][0] <65 || productions[j][0] >90)
                continue;
            for(l=0;l<productions[j].length()-1;l++)
            {
                if(productions[j][l] < 65 || productions[j][l] > 90)
                {
                    continue;
                }
                else
                {
                    if(productions[j][l+1] < 65 || productions[j][l+1] > 90)
                        follow[productions[j][l]].push_back(productions[j][l+1]);
                    else if(!isEpsilon[productions[j][l+1]])
                        follow[productions[j][l]].insert(follow[productions[j][l]].end(),first[productions[j][l+1]].begin(),first[productions[j][l+1]].end());
                    else {
                        int k=l+1;
                        while(k<productions[j].length())
                        {
                            if(productions[j][k] < 65 || productions[j][k] > 90)
                            {
                                follow[productions[j][l]].push_back(productions[j][k]);
                                break;
                            }
                            else if(!isEpsilon[productions[j][k]])
                            {
                                follow[productions[j][l]].insert(follow[productions[j][l]].end(),first[productions[j][k]].begin(),first[productions[j][k]].end());
                                break;
                            }
                            follow[productions[j][l]].insert(follow[productions[j][l]].end(),first[productions[j][k]].begin(),first[productions[j][k]].end());
                            remove(follow[productions[j][l]].begin(), follow[productions[j][l]].end(), '^');
                            k++;
                        }
                        if(k==productions[j].length())
                        {
                            follow[productions[j][l]].insert(follow[productions[j][l]].end(),follow[nonTerminal].begin(),follow[nonTerminal].end());
                        }
                        remove(follow[productions[j][l]].begin(), follow[productions[j][l]].end(), '^');
                    }
                }
            }
            if(productions[j][l] >= 65 && productions[j][l] <= 90){
                follow[productions[j][l]].insert(follow[productions[j][l]].end(),follow[nonTerminal].begin(),follow[nonTerminal].end());
                remove(follow[productions[j][l]].begin(), follow[productions[j][l]].end(), '^');
            }

        }
    }
}

vector<char> getFirst(string str)
{
    int i, j;
    vector<char> v;
    for(i=0;i<str.length();i++)
    {
        if(str[i]<65 || str[i]>90)
        {
            v.push_back(str[i]);
            break;
        }
        int epsilon=0;
        vector<char> firsts = first[str[i]];
        for(j=0; j<firsts.size();j++)
        {
            if(firsts[i] == '^')
                epsilon=1;
            v.push_back(firsts[j]);
        }
        if(epsilon == 0)
            break;
    }
    return v;
}

void printParsingTable(vector<char> terminals, unordered_map<char,unordered_map<char,string>> parsing_table)
{
    int i;

    //print parsing table
    cout << "\nParsing Table:" << endl;
    cout << setw(4) << left << "NT";
    for(i=0;i<terminals.size();i++)
    {
        cout << setw(15) << terminals[i];
    }
    cout << endl;

    cout << setw(4) << left << "--";
    for(i=0;i<terminals.size();i++)
    {
        cout << setw(15) << "--------";
    }
    cout << endl;

    for(auto itrPT:parsing_table)
    {
        char nonTerminal = itrPT.first;
        cout << setw(4) << left << nonTerminal;
        for(i=0;i<terminals.size();i++)
        {
            string s = parsing_table[nonTerminal][terminals[i]];
            if(s.length()>0)
                cout << setw(1) << left << nonTerminal << "->" << setw(12) << left << s;
            else
                cout << setw(15) << left << "-";
        }
        cout << endl;
    }
}

int main(void)
{
    int n,i,j,n_terminals,n_nonTerminals;

    cout << "Enter number of terminals: ";
    cin >> n_terminals;
    cout << "Enter number of non-terminals: ";
    cin >> n_nonTerminals;

    //input all terminals
    unordered_map<char,string> terminal_map;
    vector<char> terminals(n_terminals+1);
    cout << "Enter the terminals: " << endl;
    for(i=0;i<n_terminals;i++)
    {
        fflush(stdin);
        cin >> terminals[i];
        terminal_map.insert(make_pair(terminals[i],""));
    }
    terminal_map.insert(make_pair('$',""));
    terminals[n_terminals]='$';

    //input grammar
    vector<string> grammar(n_nonTerminals);
    unordered_map<char,unordered_map<char,string>> parsing_table;
    vector<char> nonTerminals;

    cout << "Format of production rules corresponding to each non-terminal as: \"S->AB|a|c\" and use '^' for epsilon" <<endl;
    cout << "Enter the production rules :"<<endl;
    for(i=0;i<n_nonTerminals;i++)
    {
        fflush(stdin);
        cin >> grammar[i];
        parsing_table.insert(make_pair(grammar[i][0],terminal_map));
        production_table.insert(make_pair(grammar[i][0],vector<string>()));
        isEpsilon.insert(make_pair(grammar[i][0],false));
        
        string prod_rule;
        for(j=3;j<grammar[i].length();j++)
        {
            if(grammar[i][j]=='|')
            {
                production_table[grammar[i][0]].push_back(prod_rule);
                if(prod_rule=="^")
                    isEpsilon[grammar[i][0]] = true;
                prod_rule = "";
            }else{
                prod_rule+=grammar[i][j];
            }
        }
        if(prod_rule != "")
        {
            production_table[grammar[i][0]].push_back(prod_rule);
            if(prod_rule=="^")
                isEpsilon[grammar[i][0]] = true;
            prod_rule = "";
        }
        //initialize first and follow
        if(i>0)
        {
            follow.insert(make_pair(grammar[i][0],vector<char>()));
        }else{
            follow.insert(make_pair(grammar[i][0],vector<char>{'$'}));
        }
        first.insert(make_pair(grammar[i][0],vector<char>()));

    }

    for(auto itr:production_table)
    {
        calculateFirst(itr.first);
    }

    calculateFollow();
    printFirst();
    cout << endl;
    printFollow();

    int is_ll1 =1;
    char nonTerminal;
    vector<string> productions;
    
    //constructing parsing table
    for(auto itrPT:parsing_table)
    {
        nonTerminal = itrPT.first;
        productions = production_table[nonTerminal];
        for(i=0;i<productions.size();i++)
        {
            if(productions[i]=="^")
            {
                for(j=0;j<follow[nonTerminal].size();j++)
                {
                    if(parsing_table[nonTerminal][follow[nonTerminal][j]] != "")
                    {
                        is_ll1=0;
                        break;
                    }
                    parsing_table[nonTerminal][follow[nonTerminal][j]] = '^';
                }
                     
            }
            else if(productions[i][0]<65 || productions[i][0]>90)
            {
                if(parsing_table[nonTerminal][productions[i][0]] != "")
                {
                    is_ll1=0;
                    break;
                }
                parsing_table[nonTerminal][productions[i][0]] = productions[i];
            }else{
                vector<char> firsts;
                firsts = getFirst(productions[i]);
                for(j=0;j<firsts.size();j++)
                {
                    if(parsing_table[nonTerminal][firsts[j]] != "")
                    {
                        is_ll1=0;
                        break;
                    }
                    parsing_table[nonTerminal][firsts[j]] = productions[i];
                }
            }
            if(is_ll1==0)
                break;
        }
        if(is_ll1==0)
                break;
    }


    if(is_ll1 ==0)
        cout << "\nGiven grammar is not LL1" << endl;
    else
        printParsingTable(terminals,parsing_table);


    return 0;
}