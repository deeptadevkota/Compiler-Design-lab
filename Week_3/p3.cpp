#include<iostream>
#include<math.h>
#include<string>
#include<vector>
#include<unordered_map>
#include<set>
#include<algorithm>
#include<iomanip>

using namespace std;

unordered_map<char, unordered_map<string,int>> grammar;

vector<set<pair<char,string>>> states;
set<set<pair<char,string>>> set_states;
unordered_map<char,set<pair<char,string>>> closures;
vector<unordered_map<char,int>> map_states;


//swap function to swap two charaters 
string swap(string str, int index)
{
    char c;
    c = str[index];
    str[index] = str[index+1];
    str[index+1] = c;
    return str;
}

int getPositionOfScannedProd(string str)
{
    int index;
    index = 0;
    while (str[index] != '.')
    {
        index++;
    }

    return index;
}

void compute_closures(char nt)
{

    if(closures.find(nt) == closures.end())
    {
        closures.insert(make_pair(nt,set<pair<char,string>>()));

        unordered_map<string,int> prod = grammar[nt];

        for(auto itr:prod)
        {
            string p = "." + itr.first;
            closures[nt].insert(make_pair(nt,p));

            if((p[1]>=65 && p[1]<=90) && p[1] != nt)
            {
                if(closures.find(p[1]) == closures.end())
                {
                    compute_closures(p[1]);
                }
                closures[nt].insert(closures[p[1]].begin(),closures[p[1]].end());
            }

        }

    }

    return;
}

void generateStates(char start_symbol, vector<char> terminals, vector<char> nonTerminals)
{

    int i, j;

    states.push_back(set<pair<char,string>>());
    string temp = ".";
    temp.push_back(start_symbol);

    states[0].insert(make_pair('&',temp));
    for(auto itr:closures[start_symbol])
    {
        states[0].insert(itr);
    }

    for(i=0;i<states.size();i++)
    {
        unordered_map<char,set<pair<char,string>>> goto_map;
        map_states.push_back(unordered_map<char,int>());

        for(auto itr:states[i])
        {
            int posi;
            string prod = itr.second;
            char nt = itr.first;

            posi = getPositionOfScannedProd(prod);

            if(posi == prod.length()-1)
                continue;
            
            prod = swap(prod,posi);

            if(goto_map.find(prod[posi]) == goto_map.end())
            {
                goto_map.insert(make_pair(prod[posi],set<pair<char,string>>()));
            }

            goto_map[prod[posi]].insert(make_pair(nt,prod));

            if( (posi+2 >= prod.length()) || (prod[posi+2]<65 || prod[posi+2]>90))
                continue;
            
            goto_map[prod[posi]].insert(closures[prod[posi+2]].begin(),closures[prod[posi+2]].end());
        }

        for(auto itr:goto_map)
        {
            int state_no;
            if(set_states.find(itr.second) == set_states.end())
            {
                set_states.insert(itr.second);
                states.push_back(itr.second);
                state_no = states.size()-1;
            }
            else{
                state_no = find(states.begin(), states.end(), itr.second) - states.begin();
            }
            map_states[i].insert(make_pair(itr.first,state_no));
        }

    }

    // cout << endl;
    // for(i=0;i<map_states.size();i++)
    // {
    //     cout << "State " << i << endl;
    //     for(auto itr:map_states[i])
    //     {
    //         cout << itr.first << ": " << itr.second << endl;
    //     }
    // }

}

void printStates(char start_symbol)
{
    int i;

    cout << "\nAll generated states are: " << endl;
    for(i=0;i<states.size();i++)
    {
        cout << "\nState " << i << ":" << endl;
        for(auto itr:states[i])
        {
            if(itr.first == '&')
                cout << start_symbol << "\'->" << itr.second << endl;
            else
                cout << itr.first << "->" << itr.second << endl;
        }
    }
}

void printParsingTable(int n_terminals, int n_nonTerminals,vector<char> terminals, vector<char> nonTerminals, vector<vector<string>> parsing_table)
{

    int i, j;

    cout << "\nParsing Table:" << endl;

    cout << " _____________";
    for(i=0;i<n_terminals+n_nonTerminals+1;i++)
        cout << "____________";
    cout << endl;

    cout << "| "<<left <<setw(12) <<"State No. ";
    for(i=0;i<n_terminals;i++)
        cout << "| " <<left <<setw(10) <<terminals[i];
    cout << "| " << left <<setw(10) << "$";
    for(i=0;i<n_nonTerminals;i++)
        cout << "| " <<left <<setw(10) <<nonTerminals[i];
    cout << "|";
    cout << endl;

    cout << "|-------------";
    for(i=0;i<n_terminals+n_nonTerminals+1;i++)
        cout << "|-----------";
    cout << "|";
    cout << endl;

    int n_states =states.size();

    for(i=0;i<n_states;i++)
    {
        cout << "| "<<left <<setw(12)  << i;
        for(j=0;j<n_terminals+n_nonTerminals+1;j++)
        {
            cout << "| " << left <<setw(10) << parsing_table[i][j];
        }
        cout << "|";
        cout << endl;
    }
    cout << "|_____________";
    for(i=0;i<n_terminals+n_nonTerminals+1;i++)
        cout << "|___________";
    cout << "|" << endl;

    cout << endl;

}


int main(void)
{

    int n_terminals, n_nonTerminals, i, j, k, n_prod;

    cout << "\nEnter the number of terminals: ";
    cin >> n_terminals;

    cout << "Enter the number of non-terminals: ";
    cin >> n_nonTerminals;

    vector<char> terminals(n_terminals);
    vector<char> nonTerminals(n_nonTerminals);

    cout << "\nEnter the terminals: "<<endl;
    for(i=0;i<n_terminals;i++)
    {
        fflush(stdin);
        cin >> terminals[i];
    }

    cout << "\nEnter the non-terminals: "<<endl;
    for(i=0;i<n_nonTerminals;i++)
    {
        fflush(stdin);
        cin >> nonTerminals[i];
    }

    char start_symbol;
    cout << "\nEnter the start symbol: ";
    fflush(stdin);
    cin >> start_symbol;

    cout << "\nEnter the number of productions: ";
    cin >> n_prod;

    set<pair<char,string>> productions;
    int count=0;

    cout << "Enter the grammar in the form \'S->ab|Ab|Bc\' and use \'^\' for epsilon: " << endl;
    for(i=0; i<n_nonTerminals ; i++)
    {
        fflush(stdin);
        string g;
        cin >> g;
        grammar.insert(make_pair(g[0],unordered_map<string,int>()));

        string temp;
        for(j=3; j< g.length(); j++)
        {
            if(g[j] == '|')
            {
                grammar[g[0]].insert(make_pair(temp,++count));
                temp = "";
            }else{
                temp += g[j];
            }
        }
        grammar[g[0]].insert(make_pair(temp,++count));
    }

    //augmenting grammar
    grammar.insert(make_pair('&',unordered_map<string,int>()));
    string temp;
    temp.push_back(start_symbol);
    grammar['&'].insert(make_pair(temp,++count));

    compute_closures(start_symbol);

    generateStates(start_symbol,terminals,nonTerminals);

    printStates(start_symbol);

    int n_states = states.size();

    vector<vector<string>> parsing_table(n_states,vector<string>(n_terminals + n_nonTerminals+1,""));

    for(i=0;i<n_states;i++)
    {

        string temp;
        temp.push_back(start_symbol);
        temp += ".";
        pair<char,string> p;
        p.first = '&';
        p.second = temp;
        if(states[i].find(p) != states[i].end())
        {
            parsing_table[i][n_terminals] = "accept";
            if(map_states[i].empty())
            {
                for(j=0;j<n_terminals+n_nonTerminals+1;j++)
                {
                    if(j==n_terminals)
                        continue;
                    parsing_table[i][j] = "-";
                }
            }
        }

        if(map_states[i].empty())
        {
            auto itr = states[i].begin();
            string temp = itr->second;
            char nt = itr->first;
            temp.pop_back();
            int p_no = grammar[nt][temp];
            for(j=0;j<n_terminals+1;j++)
            {
                parsing_table[i][j] = "r";
                parsing_table[i][j].push_back(48+p_no);
            }
            while(j<n_terminals+n_nonTerminals+1)
            {
                parsing_table[i][j] = "-";
                j++;
            }
            continue;
        }

        for(j=0;j<n_terminals;j++)
        {
            auto itr = map_states[i].find(terminals[j]);
            if(itr != map_states[i].end())
            {
                parsing_table[i][j] = "I";
                parsing_table[i][j].push_back(48+itr->second);
            }else{
                parsing_table[i][j] = "-";
            }
        }
        parsing_table[i][j] = "-";

        k=j+1;
        for(j=0;j<n_nonTerminals;j++,k++)
        {
            auto itr = map_states[i].find(nonTerminals[j]);
            
            if(itr != map_states[i].end())
            {
                parsing_table[i][k].push_back(48+itr->second);
            }else{
                parsing_table[i][k] = "-";
            }
        }

    }

    printParsingTable(n_terminals, n_nonTerminals, terminals, nonTerminals, parsing_table);
    
    return 0;
}



/*

Sample for input format:

Enter the number of terminals: 2
Enter the number of non-terminals: 2

Enter the terminals: 
a
b

Enter the non-terminals: 
S
A

Enter the start symbol: S

Enter the number of productions: 3
Enter the grammar in the form 'S->ab|Ab|Bc' and use '^' for epsilon: 
S->AA
A->aA|b


*/