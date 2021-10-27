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

vector<set<pair<char,pair<string,vector<char>>>>> states;              //store states, each state is a set of pair formed by the nonterminal and corresponding scanned string
set<set<pair<char,pair<string,vector<char>>>>> set_states;             //stores states, just to check if a state is already present or not

vector<pair<int,set<pair<char,pair<int,vector<char>>>>>> reduced_states;               //to check which states contain reduced items

vector<unordered_map<char,int>> map_states;        //map transition of each states on terminals and nonterminals to other states => GOTO(state,c)

unordered_map<char,vector<char>> first, follow;
unordered_map<char,bool> isEpsilon;

vector<vector<int>> mergeStates;

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

void computeClosureForStates(string prod, vector<char> look_aheads, set<pair<char,pair<string,vector<char>>>>* forming_state)
{
    int posi;
    posi = getPositionOfScannedProd(prod);

    // cout << "computing states function" << endl;
    cout << prod << " " << posi <<endl;

    if(posi==prod.length()-1)
        return;
    
    if(prod[posi+1] < 65 || prod[posi+1]>90)
        return;
    
    pair<char,pair<string,vector<char>>> item;
    
    unordered_map<string,int> productions = grammar[prod[posi+1]];

    char nt = prod[posi+1];

    for(auto itr:productions)
    {

        pair<char,pair<string,vector<char>>> item;
        item.first = nt;
        item.second.first = "." + itr.first;

        if(posi+2==prod.length())
        {
            item.second.second = look_aheads;
        }else if(prod[posi+2] < 65 || prod[posi+2]>90){
            item.second.second.push_back(prod[posi+2]);
        }else{
            item.second.second = first[prod[posi+2]];
        }

        // cout << item.first << " " << item.second.first << " , ";
        // for(int i=0;i<item.second.second.size();i++)
        //     cout << item.second.second[i] << " ";
        // cout << endl;

        if(forming_state->find(item) == forming_state->end())
        {
            forming_state->insert(item);
            computeClosureForStates(item.second.first, item.second.second, forming_state);
        }
        
    }
    return;

}

void checkForSameitems(int state_no)
{
    int i;
    set<pair<char,pair<string,vector<char>>>> p = states[state_no];
    set<pair<char,string>> lr0;
    for(auto itr:states[state_no])
    {
        lr0.insert(make_pair(itr.first,itr.second.first));
    }
    for(i=0;i<states.size();i++)
    {
        if(i==state_no)
            continue;
        set<pair<char,string>> lr0_check;
        for(auto itr:states[i])
        {
            lr0_check.insert(make_pair(itr.first,itr.second.first));
        }
        
        int flag=1;
        for(auto itr:lr0)
        {
            pair<char,string>  checkItem = itr;
            if(find(lr0_check.begin(),lr0_check.end(),checkItem) == lr0_check.end())
            {
                flag=0;
                break;
            }
        }

        if(flag==1)
        {
            mergeStates[state_no].push_back(i);
        }
    }

}

//function to generate all states or canonical items
void generateStates(char start_symbol, vector<char> terminals, vector<char> nonTerminals)
{

    int i, j;

    /*---initialze states but inserting state 0-----*/
    // cout << "Forming state 0" << endl;

    set<pair<char,pair<string,vector<char>>>> forming_state0;
    pair<char,pair<string,vector<char>>> item;
    item.first = '&';
    item.second.first = ".";
    item.second.first.push_back(start_symbol);
    item.second.second.push_back('$');
    forming_state0.insert(item);
    // cout << item.first << " " << item.second.first << endl;
    // cout << "forming...." << endl;
    computeClosureForStates(item.second.first,item.second.second, &forming_state0);
    states.push_back(forming_state0);
    reduced_states.push_back(make_pair(0,set<pair<char,pair<int,vector<char>>>>()));
    states[0] = forming_state0;
    /*-----------------*/

    // cout << "State 0 computed" << endl;

    /*--Iterate over states to compute transtions----*/
    for(i=0;i<states.size();i++)
    {
        vector<int> merge;
        mergeStates.push_back(merge);
        checkForSameitems(i);

        // cout << "state: " << i << endl;
        unordered_map<char,set<pair<char,pair<string,vector<char>>>>> goto_map;  //goto map to map transtion of state on terminal or non-terminal
        map_states.push_back(unordered_map<char,int>());

        int reduced_flag = 0;        //check for reduced item
        set<pair<char,pair<int,vector<char>>>> pairs_reduced;     //to get production no. and left-hand side of lr item in case state consists of reduced item

        for(auto itr:states[i])         //iterate over all items in state
        {
            int posi;
            string prod = itr.second.first;
            vector<char> lookaAheads = itr.second.second;
            char nt = itr.first;

            posi = getPositionOfScannedProd(prod);   //get the position until which string is scanned

            // cout << "posi: " << posi << endl;

            if(posi == prod.length()-1)   //string is completely scanned hence the state consists of reduced item
            {
                reduced_flag=1;
                string p = prod;
                p.pop_back();
                pairs_reduced.insert(make_pair(nt,make_pair(grammar[nt][p],lookaAheads)));
                continue;
            }
            
            prod = swap(prod,posi);         //swap '.' with adjacent element

            // cout << "prod: " << prod << endl;

            if(goto_map.find(prod[posi]) == goto_map.end())       //add the item to goto_map of current state
            {
                goto_map.insert(make_pair(prod[posi],set<pair<char,pair<string,vector<char>>>>()));
            }

            pair<char,pair<string,vector<char>>> item;
            item.first = nt;
            item.second.first = prod;

            for(j=0;j<lookaAheads.size();j++)
            {
                item.second.second.push_back(lookaAheads[j]);
            }
            
            if(goto_map[prod[posi]].find(item) == goto_map[prod[posi]].end())
            {
                goto_map[prod[posi]].insert(item);
            }
            
            
            if( (posi+2 >= prod.length()) || (prod[posi+2]<65 || prod[posi+2]>90))    //if goto was on terminal then continue
            {
                continue;
            }
                
            computeClosureForStates(item.second.first, item.second.second, &goto_map[prod[posi]]);


        }

        //assign flag and production number in reduced_state vector
        reduced_states[i].first = reduced_flag;
        reduced_states[i].second = pairs_reduced;

        //check if transition sets are already present in states or needs to be added
        for(auto itr:goto_map)
        {
            int state_no;
            if(set_states.find(itr.second) == set_states.end())
            {
                set_states.insert(itr.second);
                states.push_back(itr.second);
                reduced_states.push_back(make_pair(0,set<pair<char,pair<int,vector<char>>>>()));
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
    int i,j;

    cout << "\nAll generated states are: " << endl;
    for(i=0;i<states.size();i++)
    {
        cout << "\nState " << i << ":" << endl;
        for(auto itr:states[i])
        {
            if(itr.first == '&')
            {    
                cout << start_symbol << "\'->" << itr.second.first << " | ";
                for(j=0;j<itr.second.second.size();j++)
                {
                    cout << itr.second.second[j] << ", ";
                }
                cout << endl;
            }
            else
            {    
                cout << itr.first << "->" << itr.second.first << " | ";
                for(j=0;j<itr.second.second.size();j++)
                {
                    cout << itr.second.second[j] << ", ";
                }
                cout << endl;
            }
        }
    }
}


void printMergeStates()
{
    int i,j;
    for(i=0;i<mergeStates.size();i++)
    {
        cout << i << ": ";
        for(j=0;j<mergeStates[i].size();j++)
        {
            cout << mergeStates[i][j] << " ";
        }
        cout << endl;
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
void printParsingTable(int n_terminals, int n_nonTerminals,vector<char> terminals, vector<char> nonTerminals, vector<vector<pair<int,vector<int>>>> parsing_table)
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
                value = "I";
                for(int m=0;m<parsing_table[i][j].second.size();m++)
                {
                    value += (getStringFromNum(parsing_table[i][j].second[m])+",");
                }
                cout << "| " << left <<setw(10) << value;
            }else if(parsing_table[i][j].first == 1)
            {
                value = "r" + getStringFromNum(parsing_table[i][j].second[0]);
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
                value = getStringFromNum(parsing_table[i][j].second[0]);
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

    int n_terminals, n_nonTerminals, i, j, k, n_prod, is_LALR;


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
    // cout << "Computed closures" << endl;

    calculateFirst(start_symbol);
    calculateFollow();

    printFirst();
    printFollow();

    generateStates(start_symbol,terminals,nonTerminals);

    printStates(start_symbol);

    
    // printMergeStates();

    // cout << "states generated" << endl;

    /*--------------Computing Parsing table---------------------------*/

    is_LALR = 1;

    int n_states = states.size();

    terminals.push_back('$');

    vector<vector<pair<int,vector<int>>>> parsing_table(n_states,vector<pair<int,vector<int>>>(n_terminals + n_nonTerminals+1,pair<int,vector<int>>(-1,vector<int>())));
    
    for(i=0;i<n_states;i++)
    {
        // cout << "\nState: " << i << endl;


        /*----Check if current state corresponding to LR(1) item generated from augmented production--------------*/
        string temp;
        temp.push_back(start_symbol);
        temp += ".";
        pair<char,pair<string,vector<char>>> item;
        item.first = '&';
        item.second.first = temp;
        item.second.second = {'$'};
        if(states[i].find(item) != states[i].end())
        {
            parsing_table[i][n_terminals].first = -2;
            parsing_table[i][n_terminals].second.push_back(-2);
            continue;
        }
        /*-----------------------------------------------------------------*/
        // cout << "Augmentation checked" << endl;

        /*----------------Check if current state consists of reduced item----------------------*/
        if(reduced_states[i].first == 1)
        {
            set<pair<char,pair<int,vector<char>>>> p_reduced = reduced_states[i].second;

            for(auto prtr:p_reduced)
            {
                char nt = prtr.first;
                int p_no = prtr.second.first;
                vector<char> look_aheads = prtr.second.second;
                for(j=0;j<=n_terminals+1;j++)
                {
                    if(find(look_aheads.begin(),look_aheads.end(),terminals[j]) != look_aheads.end())
                    {
                        if(parsing_table[i][j].first ==-1)
                        {
                            parsing_table[i][j].first = 1;
                            parsing_table[i][j].second.push_back(p_no);
                        }else{
                            is_LALR=0;
                            break;
                        }
                        
                    }
                }

                if(is_LALR==0)
                    break;
                
            }
            
        }
        /*-------------------------------------------------------------------*/
        // cout << "Reduced items checked" << endl;

        if(is_LALR==0)
            break;

        /*--------------Check for transtions on terminals and nonterminals-----------------------*/
        for(j=0;j<=n_terminals;j++)
        {
            auto itr = map_states[i].find(terminals[j]);

            if(itr != map_states[i].end())
            {
                if(parsing_table[i][j].first == 1)
                {
                    is_LALR=0;
                    break;
                }
                parsing_table[i][j].first = 0;
                parsing_table[i][j].second.push_back(itr->second);
                for(int m=0;m<mergeStates[itr->second].size();m++)
                {
                    parsing_table[i][j].second.push_back(mergeStates[itr->second][m]);
                }
            }
        }

        // cout << "Terminals checked" << endl;

        if(is_LALR==0)
            break;

        k=j;
        for(j=0;j<n_nonTerminals;j++,k++)
        {
            auto itr = map_states[i].find(nonTerminals[j]);
            
            if(itr != map_states[i].end())
            {
                parsing_table[i][k].first = 0;
                parsing_table[i][k].second.push_back(itr->second);
            }
        }
        /*----------------------------------------------*/

        if(is_LALR==0)
            break;
        // cout << "Non-Terminals checked" << endl;
    }

    /*----------------------------------------------------------*/

    if(is_LALR==1)
    {    
        printParsingTable(n_terminals, n_nonTerminals, terminals, nonTerminals, parsing_table);
        string input;
        int x=1;

        // while(x)
        // {
        //     cout << "\nEnter input string: ";
        //     cin >> input;
        //     if(checkInput(input,parsing_table,terminals,nonTerminals))
        //     {
        //         cout << "Above string is ACCEPTED by the given grammar" << endl;
        //     }
        //     else{
        //         cout << "Above string is NOT ACCEPTED by the given grammar" << endl;
        //     }

        //     cout << "\nTo check more strings enter 1 else 0: " ;
        //     cin >> x;
        // }

        // cout << endl;

    }
    else
        cout << "\nGiven grammar is not LALR(1) grammar" << endl;

    
    
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




