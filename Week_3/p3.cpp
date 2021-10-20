#include<iostream>
#include<math.h>
#include<string>
#include<vector>
#include<unordered_map>
#include<set>
#include<algorithm>
#include<iomanip>

using namespace std;

unordered_map<char, unordered_map<string,int>> grammar;    //store grammar

unordered_map<char,set<pair<char,string>>> closures; //stores closures for each non-terminal, each non-terminal corresponds to set of LR(0) items

vector<set<pair<char,string>>> states;              //store states, each state is a set of pair formed by the nonterminal and corresponding scanned string
set<set<pair<char,string>>> set_states;             //stores states, just to check if a state is already present or not

vector<pair<int,int>> reduced_states;               //to check which states contain reduced items, (0,-1) in case it doesn't else (1,production_no)

vector<unordered_map<char,int>> map_states;        //map transition of each states on terminals and nonterminals to other states => GOTO(state,c)


//swap function to swap two charaters at index and index+1
string swap(string str, int index)
{
    char c;
    c = str[index];
    str[index] = str[index+1];
    str[index+1] = c;
    return str;
}

//function to get position of "." in string
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

//function to compute closures
void compute_closures(char nt)
{

    if(closures.find(nt) == closures.end()) //compute only if closure is not computed
    {
        closures.insert(make_pair(nt,set<pair<char,string>>()));

        unordered_map<string,int> prod = grammar[nt];

        for(auto itr:prod)
        {
            string p = "." + itr.first;                //append "." to the production
            closures[nt].insert(make_pair(nt,p));

            if((p[1]>=65 && p[1]<=90) && p[1] != nt)     //if the first letter is non-terminal then add its closure 
            {
                if(closures.find(p[1]) == closures.end())    //if closure does not exist then compute
                {
                    compute_closures(p[1]);
                }
                closures[nt].insert(closures[p[1]].begin(),closures[p[1]].end());
            }

        }

    }

    return;
}


//function to generate all states or canonical items
void generateStates(char start_symbol, vector<char> terminals, vector<char> nonTerminals)
{

    int i, j;

    /*---initialze states but inserting state 0-----*/
    states.push_back(set<pair<char,string>>());
    reduced_states.push_back(make_pair(0,-1));
    string temp = ".";
    temp.push_back(start_symbol);

    states[0].insert(make_pair('&',temp));
    for(auto itr:closures[start_symbol])
    {
        states[0].insert(itr);
    }
    /*-----------------*/

    /*--Iterate over states to compute transtions----*/
    for(i=0;i<states.size();i++)
    {
        unordered_map<char,set<pair<char,string>>> goto_map;  //goto map to map transtion of state on terminal or non-terminal
        map_states.push_back(unordered_map<char,int>());

        int reduced_flag = 0;        //check for reduced item
        int g_no = -1;               //to get production no. in case state consists of reduced item

        for(auto itr:states[i])         //iterate over all items in state
        {
            int posi;
            string prod = itr.second;
            char nt = itr.first;

            posi = getPositionOfScannedProd(prod);   //get the position until which string is scanned

            if(posi == prod.length()-1)   //string is completely scanned hence the state consists of reduced item
            {
                if(reduced_flag == 0) 
                    reduced_flag = 1;
                else if(reduced_flag == 1)   //rr conflict
                    reduced_flag = -1;
                else                         //rr and sr conflict
                    reduced_flag = -3;
                string p = prod;
                p.pop_back();
                g_no = grammar[nt][p];
                continue;
            }

            if(reduced_flag == 1)      //sr conflict
                reduced_flag = -2;
            else if(reduced_flag == -1)   //rr and sr conflict
                reduced_flag = -3;
            
            prod = swap(prod,posi);         //swap '.' with adjacent element

            if(goto_map.find(prod[posi]) == goto_map.end())       //add the item to goto_map of current state
            {
                goto_map.insert(make_pair(prod[posi],set<pair<char,string>>()));
            }

            goto_map[prod[posi]].insert(make_pair(nt,prod));

            if( (posi+2 >= prod.length()) || (prod[posi+2]<65 || prod[posi+2]>90))    //if goto was on terminal then continue
                continue;
            
            //if goto was on non-terminal then add closures of non-terminal to the goto_map
            goto_map[prod[posi]].insert(closures[prod[posi+2]].begin(),closures[prod[posi+2]].end()); 
            


        }

        //assign flag and production number in reduced_state vector
        reduced_states[i].first = reduced_flag;
        reduced_states[i].second = g_no;

        //check if transition sets are already present in states or needs to be added
        for(auto itr:goto_map)
        {
            int state_no;
            if(set_states.find(itr.second) == set_states.end())
            {
                set_states.insert(itr.second);
                states.push_back(itr.second);
                reduced_states.push_back(make_pair(0,-1));
                state_no = states.size()-1;
            }
            else{
                state_no = find(states.begin(), states.end(), itr.second) - states.begin();
            }

            map_states[i].insert(make_pair(itr.first,state_no));   //map transition on curr element to corresponding state number
        }

    }

}


//function to print all states
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


//function to print parsing table
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

    int n_terminals, n_nonTerminals, i, j, k, n_prod, is_LR0;


    /*---------------Taking Input-------------------*/

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
        for(j=3; j< g.length(); j++)       //separate productions using '|' and add into grammar map
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

    /*-------------------------------------------*/


    /*-------augmenting grammar----*/
    grammar.insert(make_pair('&',unordered_map<string,int>()));
    string temp;
    temp.push_back(start_symbol);
    grammar['&'].insert(make_pair(temp,++count));
    /*---------------------------*/


    compute_closures(start_symbol);

    generateStates(start_symbol,terminals,nonTerminals);

    printStates(start_symbol);


    /*--------------Computing Parsing table---------------------------*/

    is_LR0 = 1;
    int rr=0,sr=0;

    int n_states = states.size();

    vector<vector<string>> parsing_table(n_states,vector<string>(n_terminals + n_nonTerminals+1,""));

    for(i=0;i<n_states;i++)
    {
        if(reduced_states[i].first <= -1)    //RR conflict
        {
            is_LR0=0;
            if(reduced_states[i].first == -1)
                rr = 1;
            else if(reduced_states[i].first == -2)
                sr = 1;
            else
            {
                rr=sr=1;
            }
            break;
        }

        /*----Check if current state corresponding to LR(0) item generated from augmented production--------------*/
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
            continue;
        }
        /*-----------------------------------------------------------------*/


        /*----------------Check if current state consists of reduced item----------------------*/
        if(reduced_states[i].first == 1)
        {
            int p_no = reduced_states[i].second;

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
        /*-------------------------------------------------------------------*/


        /*--------------Check for transtions on terminals and nonterminals-----------------------*/
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
        /*----------------------------------------------*/

    }

    /*----------------------------------------------------------*/

    if(is_LR0==1)
        printParsingTable(n_terminals, n_nonTerminals, terminals, nonTerminals, parsing_table);
    else
    {
        cout << "\nGiven grammar is not LR(0) grammar due to ";
        if(rr==1 && sr == 1)
            cout << "RR and SR conflict\n"<<endl;
        else if(rr==1)
            cout << "RR conflict\n"<<endl;
        else
            cout << "SR conflict\n"<<endl;
    }
    
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