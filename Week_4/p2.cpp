#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<set>
#include<algorithm>
#include<stack>
#include<iomanip>

using namespace std;

unordered_map<char, unordered_map<string,int>> grammar;    //store grammar
vector<pair<char,string>> all_productions;

unordered_map<char,set<pair<char,string>>> closures; //stores closures for each non-terminal, each non-terminal corresponds to set of LR(0) items

vector<set<pair<char,string>>> states;              //store states, each state is a set of pair formed by the nonterminal and corresponding scanned string
set<set<pair<char,string>>> set_states;             //stores states, just to check if a state is already present or not

vector<pair<int,pair<char,int>>> reduced_states;               //to check which states contain reduced items

vector<unordered_map<char,int>> map_states;        //map transition of each states on terminals and nonterminals to other states => GOTO(state,c)

unordered_map<char,vector<char>> first, follow;
unordered_map<char,bool> isEpsilon;

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
    reduced_states.push_back(make_pair(0,pair<char,int>()));
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
        pair<char,int> pair_reduced;     //to get production no. and left-hand side of lr item in case state consists of reduced item
        pair_reduced.first = '-';
        pair_reduced.second = -1;

        for(auto itr:states[i])         //iterate over all items in state
        {
            int posi;
            string prod = itr.second;
            char nt = itr.first;

            posi = getPositionOfScannedProd(prod);   //get the position until which string is scanned

            if(posi == prod.length()-1)   //string is completely scanned hence the state consists of reduced item
            {
                if(reduced_flag == 0) 
                {    
                    reduced_flag = 1;
                    string p = prod;
                    p.pop_back();
                    pair_reduced.first = nt;
                    pair_reduced.second = grammar[nt][p];
                }
                else if(reduced_flag == 1)   //rr conflict
                    reduced_flag = -1;
                continue;
            }
            
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
        reduced_states[i].second = pair_reduced;

        //check if transition sets are already present in states or needs to be added
        for(auto itr:goto_map)
        {
            int state_no;
            if(set_states.find(itr.second) == set_states.end())
            {
                set_states.insert(itr.second);
                states.push_back(itr.second);
                reduced_states.push_back(make_pair(0,pair_reduced));
                state_no = states.size()-1;
            }
            else{
                state_no = find(states.begin(), states.end(), itr.second) - states.begin();
            }

            map_states[i].insert(make_pair(itr.first,state_no));   //map transition on curr element to corresponding state number
        }

    }

}

//function to calculate first
void calculateFirst(char nonTerminal)
{
    int i,j;
    if(first[nonTerminal].size()>0)
        return;
    unordered_map<string,int> productions;
    productions = grammar[nonTerminal];
    vector<char> first_curr;

    for(auto itr:productions)
    {
        if(itr.first=="^")
        {
            first_curr.push_back('^');
            continue;
        }
        j=0;
        if(itr.first[0] == nonTerminal)
            continue;
        while(j<itr.first.length())
        {
            if(itr.first[j] < 65 || itr.first[j] > 90)  //terminal
            {
                first_curr.push_back(itr.first[j]);
                break;
            }
            else{                                                //non-terminal
                if(first[itr.first[j]].size() == 0)
                    calculateFirst(itr.first[j]);
                for(int k=0;k<first[itr.first[j]].size();k++)
                {
                    if(first[itr.first[j]][k]=='^')
                    {
                        continue;
                    }else{
                        first_curr.push_back(first[itr.first[j]][k]);
                    }
                }
            }
            if(!isEpsilon[itr.first[j]]){
                break;
            }
            j++;
        }
    }
    
    vector<char>::iterator it = unique(first_curr.begin(),first_curr.end());
    first_curr.resize(distance(first_curr.begin(),it));
    first[nonTerminal] = first_curr;

}

//function to calculate follow
void calculateFollow()
{
    int j,l;
    unordered_map<string,int> productions;
    vector<char> first_curr;
    char nonTerminal;
    for(auto i:grammar)
    {
        productions = i.second;
        nonTerminal = i.first;
        for(auto itr:productions)
        {
            if(itr.first == "^")   
                continue;
            if(itr.first.length() == 1 && itr.first[0] <65 || itr.first[0] >90)
                continue;
            for(l=0;l<itr.first.length()-1;l++)
            {
                if(itr.first[l] < 65 || itr.first[l] > 90)
                {
                    continue;
                }
                else
                {
                    if(itr.first[l+1] < 65 || itr.first[l+1] > 90)
                        follow[itr.first[l]].push_back(itr.first[l+1]);
                    else if(!isEpsilon[itr.first[l+1]])
                        follow[itr.first[l]].insert(follow[itr.first[l]].end(),first[itr.first[l+1]].begin(),first[itr.first[l+1]].end());
                    else {
                        int k=l+1;
                        while(k<itr.first.length())
                        {
                            if(itr.first[k] < 65 || itr.first[k] > 90)
                            {
                                follow[itr.first[l]].push_back(itr.first[k]);
                                break;
                            }
                            else if(!isEpsilon[itr.first[k]])
                            {
                                follow[itr.first[l]].insert(follow[itr.first[l]].end(),first[itr.first[k]].begin(),first[itr.first[k]].end());
                                break;
                            }
                            follow[itr.first[l]].insert(follow[itr.first[l]].end(),first[itr.first[k]].begin(),first[itr.first[k]].end());
                            remove(follow[itr.first[l]].begin(), follow[itr.first[l]].end(), '^');
                            k++;
                        }
                        if(k==itr.first.length())
                        {
                            follow[itr.first[l]].insert(follow[itr.first[l]].end(),follow[nonTerminal].begin(),follow[nonTerminal].end());
                        }
                        remove(follow[itr.first[l]].begin(), follow[itr.first[l]].end(), '^');
                    }
                }
            }
            if(itr.first[l] >= 65 && itr.first[l] <= 90){
                follow[itr.first[l]].insert(follow[itr.first[l]].end(),follow[nonTerminal].begin(),follow[nonTerminal].end());
                remove(follow[itr.first[l]].begin(), follow[itr.first[l]].end(), '^');
            }
            
        }
    }
}

//function to print follows
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
        if(itr.first == '&')
            continue;
        cout << itr.first << " : ";
        for(i=0;i<itr.second.size();i++)
        {
            cout << itr.second[i] << " ";
        }
        cout << endl;
    }
}

//function to print firsts
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

//fucntion to convert number into string
string getStringFromNum(int n)
{
    string num;
    char digit;

    while(n>0)
    {
        digit = 48 + (n%10);
        num.push_back(digit);
        n/=10;
    }

    int l = num.length();
    for (int i = 0; i <l/2; i++)
    {
        digit = num[i];
        num[i] = num[l-i-1];
        num[l-i-1] = digit;
    }
    

    return num;

}

//function to print parsing table
void printParsingTable(int n_terminals, int n_nonTerminals,vector<char> terminals, vector<char> nonTerminals, vector<vector<pair<int,int>>> parsing_table)
{

    int i, j;
    string value;

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
        for(j=0;j<=n_terminals;j++)
        {

            if(parsing_table[i][j].first == 0)
            {
                value = "I" + getStringFromNum(parsing_table[i][j].second);
                cout << "| " << left <<setw(10) << value;
            }else if(parsing_table[i][j].first == 1)
            {
                value = "r" + getStringFromNum(parsing_table[i][j].second);
                cout << "| " << left <<setw(10) << value;
            }
            else if(parsing_table[i][j].first == -2)
            {    
                cout << "| " << left <<setw(10) << "accept";
            }
            else 
            {    
                cout << "| " << left <<setw(10) << " ";
            }
        }
        for(j=n_terminals+1;j<n_terminals+n_nonTerminals+1;j++)
        {
            if(parsing_table[i][j].first == -1)
            {    
                cout << "| " << left <<setw(10) << " ";
            }
            else
            {
                value = getStringFromNum(parsing_table[i][j].second);
                cout << "| " << left <<setw(10) << value;
            }
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

//function to check if grammar is acceptable by grammar
int checkInput(string input, vector<vector<pair<int,int>>> parsing_table, vector<char> terminals, vector<char> nonTerminals)
{
    stack<pair<int,int>> st;
    pair<int,int> s_top;
    int i, j;

    input.push_back('$');

    st.push(make_pair(0,0));

    i=0;
    while (!st.empty() && i < input.length())
    {

        s_top = st.top();
        j = s_top.second;

        auto itr = find(terminals.begin(),terminals.end(),input[i]);

        if(itr == terminals.end())
            return 0;

        int index = itr - terminals.begin();

        int next = parsing_table[j][index].second;

        if(next == -1)
            return 0;
        
        if(next == -2)
            return 1;

        if(parsing_table[j][index].first == 1)
        {
            int prod_no = parsing_table[j][index].second -1;
            string production = all_productions[prod_no].second;
            char nt = all_productions[prod_no].first;

            int l = production.length();
            int k=0;
            while(k<2*l)
            {
                if(st.empty())
                    return 0;
                st.pop();
                k++;
            }

            s_top = st.top();
            j = s_top.second;

            itr = find(nonTerminals.begin(),nonTerminals.end(),nt);
            index = itr - nonTerminals.begin() + terminals.size();
            next = parsing_table[j][index].second;

            if(next == -1)
                return 0;
            
            if(next == -2)
                return 1;
            
            st.push(make_pair(2,index));
            st.push(make_pair(0,next));

        }
        else{
            st.push(make_pair(1,index));
            st.push(make_pair(0,next));
            i++;
        }

    }
    
    return 0;
}


int main(void)
{

    int n_terminals, n_nonTerminals, i, j, k, n_prod, is_SLR1;


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
                all_productions.push_back(make_pair(g[0],temp));
                temp = "";
            }else{
                temp += g[j];
            }
        }
        grammar[g[0]].insert(make_pair(temp,++count));
        all_productions.push_back(make_pair(g[0],temp));
        if(i>0)
        {
            follow.insert(make_pair(g[0],vector<char>()));
        }else{
            follow.insert(make_pair(g[0],vector<char>{'$'}));
        }

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

    calculateFirst(start_symbol);
    calculateFollow();

    printFirst();
    printFollow();


    /*--------------Computing Parsing table---------------------------*/

    is_SLR1 = 1;

    int n_states = states.size();

    terminals.push_back('$');

    vector<vector<pair<int,int>>> parsing_table(n_states,vector<pair<int,int>>(n_terminals + n_nonTerminals+1,pair<int,int>(-1,-1)));
    
    for(i=0;i<n_states;i++)
    {
        
        if(reduced_states[i].first <= -1)    //RR conflict
        {
            is_SLR1=0;
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
            parsing_table[i][n_terminals].first = -2;
            parsing_table[i][n_terminals].second = -2;
            continue;
        }
        /*-----------------------------------------------------------------*/


        /*----------------Check if current state consists of reduced item----------------------*/
        if(reduced_states[i].first == 1)
        {
            pair<char,int> p_reduced = reduced_states[i].second;
            char nt = p_reduced.first;
            int p_no = p_reduced.second;
            vector<char> follow_nt = follow[nt];

            for(j=0;j<=n_terminals+1;j++)
            {
                if(find(follow_nt.begin(),follow_nt.end(),terminals[j]) != follow_nt.end())
                {
                    parsing_table[i][j].first = 1;
                    parsing_table[i][j].second = p_no;
                }
            }
        }
        /*-------------------------------------------------------------------*/


        /*--------------Check for transtions on terminals and nonterminals-----------------------*/
        for(j=0;j<=n_terminals;j++)
        {
            auto itr = map_states[i].find(terminals[j]);

            if(itr != map_states[i].end())
            {
                if(parsing_table[i][j].second != -1)
                {
                    is_SLR1 =0;
                    break;
                }
                parsing_table[i][j].first = 0;
                parsing_table[i][j].second = itr->second;
            }
        }

        k=j;
        for(j=0;j<n_nonTerminals;j++,k++)
        {
            auto itr = map_states[i].find(nonTerminals[j]);
            
            if(itr != map_states[i].end())
            {
                parsing_table[i][k].first = 0;
                parsing_table[i][k].second = itr->second;
            }
        }
        /*----------------------------------------------*/

        if(is_SLR1==0)
            break;
    }

    /*----------------------------------------------------------*/

    if(is_SLR1==1)
    {    
        printParsingTable(n_terminals, n_nonTerminals, terminals, nonTerminals, parsing_table);
        string input;
        int x=1;

        while(x)
        {
            cout << "\nEnter input string: ";
            cin >> input;
            if(checkInput(input,parsing_table,terminals,nonTerminals))
            {
                cout << "Above string is ACCEPTED by the given grammar" << endl;
            }
            else{
                cout << "Above string is NOT ACCEPTED by the given grammar" << endl;
            }

            cout << "\nTo check more strings enter 1 else 0: " ;
            cin >> x;
        }

        cout << endl;

    }
    else
        cout << "\nGiven grammar is not SLR(1) grammar" << endl;

    
    
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




