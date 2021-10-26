/*
    Forming a parsing table for LALR grammar
    1)Build the CLR(1) states 
    2)Merge the states to form LALR(1)


    changing the data structure to store the states from

    unordered_map<char, set<string>>> &st 
    to
    unordered_map<char, set<pair<string,string>>>> &st
    --> the second element in the pair corresponds to the lookaheads
    again we would pass the given state and all other states to the compute_states() function

    the change that would make a difference should be in the compute_closure part

    now instead of precomputing items what can be done is, we need to send request each time


*/

#include <bits/stdc++.h>

using namespace std;

void calculateFirst(char c, unordered_map<char, set<string>> &m, vector<string> &first, vector<bool> &vis, vector<bool> &epsilon)
{
    string first_c = "";
    for (string i : m[c])
    {
        if (i == "#")
        {
            epsilon[c] = true;
            continue;
        }
        if (vis[i[0]] == 0 && c != i[0])
            calculateFirst(i[0], m, first, vis, epsilon);

        bool is_epsilon = epsilon[i[0]];
        first_c += first[i[0]];
        int ptr = 1;

        if (is_epsilon && ptr == i.size())
            epsilon[c] = true;

        while (is_epsilon == true && ptr < i.size())
        {
            if (vis[i[ptr]] == 0)
                calculateFirst(i[ptr], m, first, vis, epsilon);
            is_epsilon = epsilon[i[ptr]];
            first_c += first[i[ptr]];
            ptr++;

            if (is_epsilon && ptr == i.size())
                epsilon[c] = true;
        }
        if (first_c == "")
            epsilon[c] = true;
    }
    if (first_c == "")
        epsilon[c] = true;
    vis[c] = 1;
    first[c] = first_c;

    return;
}

string firstForString(string str, vector<bool> &epsilon, vector<string> &first)
{
    int i = 0;
    string req = "";

    while (i < str.size())
    {
        req += first[str[i]];
        if (epsilon[str[i]] == true)
        {
            i++;
        }
        else
            break;
    }

    return req;
}

//implement this function for today
void compute_closer(char c, string lookup, unordered_map<char, set<string>> &m, unordered_set<char> &nter, unordered_map<char, unordered_map<string, unordered_map<char, set<pair<string, string>>>>> &items, unordered_map<char, unordered_map<string, bool>> &isComputed, vector<bool> &epsilon, vector<string> &first)
{
    //cout << "Debug: " << c << " " << lookup << endl;
    set<pair<string, string>> temp;
    //set up the variable temp
    for (auto i : m[c])
    {
        temp.insert({i, lookup});
    }

    items[c][lookup][c] = temp;

    for (auto j : m[c])
    {

        //j[1] can be further closured
        if (nter.find(j[1]) != nter.end())
        {

            string new_lookup = "";
            string new_str = "";
            if (j.size() >= 3)
                new_str = j.substr(2, j.size());

            for (int i : lookup)
            {
                string temp = new_str;
                temp.push_back(i);
                new_lookup += firstForString(temp, epsilon, first);
            }

            if (isComputed[j[1]][new_lookup] == false && (c != j[1] || new_lookup != lookup))
            {
                //for the subsequent recursive call the lookup variable would change
                compute_closer(j[1], new_lookup, m, nter, items, isComputed, epsilon, first);
            }
            for (auto k : items[j[1]][new_lookup])
            {
                items[c][lookup][k.first] = k.second;
            }
        }
    }

    isComputed[c][lookup] = true;
}

bool check(string s, char c)
{
    int n = s.size();
    for (int i = 0; i < n; i++)
    {
        if (s[i] == '.' && (i + 1) < n && s[i + 1] == c)
        {
            return true;
        }
    }
    return false;
}

int find_index(string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '.')
            return i;
    }
    return 1;
}

void compute_states(int &ptr, int &states, char i, unordered_map<int, unordered_map<char, set<pair<string, string>>>> &st, unordered_map<char, unordered_map<string, unordered_map<char, set<pair<string, string>>>>> &items, unordered_map<char, set<string>> &m, unordered_set<char> &nter, unordered_map<char, unordered_map<string, bool>> &isComputed, vector<bool> &epsilon, vector<string> &first)
{
    unordered_map<char, set<pair<string, string>>> temp;
    for (auto j : st[ptr])
    {
        for (auto k : j.second)
        {
            string handle = k.first;
            string lookup = k.second;

            if (check(handle, i))
            {
                int ind = find_index(handle);

                if ((ind + 2) < handle.size())
                {
                    if (isComputed[handle[i + 2]][lookup] == false)
                        compute_closer(handle[ind + 2], lookup, m, nter, items, isComputed, epsilon, first);

                    for (auto l : items[handle[ind + 2]][lookup])
                    {

                        for (auto loop : l.second)
                        {
                            temp[l.first].insert(loop);
                        }
                    }
                }
                if ((ind + 1) < handle.size())
                {
                    swap(handle[ind], handle[ind + 1]);
                    temp[j.first].insert({handle, lookup});
                }
                // temp[j.first].insert(k);
            }
        }
    }
    if (temp.empty())
        return;
    int flag = 0;
    for (int itr = 0; itr <= states; itr++)
    {
        if (st[itr] == temp)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        states++;
        st[states] = temp;
        cout << "State: " << states << endl;
        for (auto i : temp)
        {
            for (auto j : i.second)
            {
                cout << i.first << " --> " << j.first << " , ";
                for (int k = 0; k < j.second.size() - 1; k++)
                {
                    cout << j.second[k] << "|";
                }
                cout << j.second[j.second.size() - 1] << endl;
            }
        }
        cout << endl
             << endl;
    }
}

int main()
{

    cout << "Enter the number of terminals" << endl;
    int n_ter, n_nter;
    cin >> n_ter;

    unordered_set<char> ter, nter;
    vector<string> first(256, "");
    vector<bool> vis(256, 0);
    vector<bool> epsilon(256, false);

    char c;
    cout << "Enter number the terminals" << endl;
    for (int i = 0; i < n_ter; i++)
    {
        cin >> c;
        ter.insert(c);
        first[c] = c;
        vis[c] = 1;
    }
    first['$'] = "$";
    cout << "Enter the number  of non-terminals" << endl;
    cin >> n_nter;

    cout << "Enter  the non-terminals" << endl;
    for (int i = 0; i < n_nter; i++)
    {
        cin >> c;
        nter.insert(c);
    }
    cout << "Enter number of production" << endl;
    int n_pro;
    cin >> n_pro;

    cout << "Enter the productions eg. S=A|Bb" << endl;
    vector<string> grammar(n_pro + 1);

    for (int i = 1; i <= n_pro; i++)
    {
        cin >> grammar[i];
    }
    cout << "Enter the start symbol" << endl;
    char start_symbol;
    cin >> start_symbol;

    string str = "Z=";
    str.push_back(start_symbol);

    grammar[0] = str;

    cout << endl
         << endl;
    // cout << "The augmented grammar is:" << endl;

    // for (int i = 0; i <= n_pro; i++)
    //     cout << grammar[i] << endl;

    unordered_map<char, set<string>> m;

    for (int i = 0; i <= n_pro; i++)
    {
        string p = ".";
        for (int j = 2; j < grammar[i].size(); j++)
        {
            if (grammar[i][j] == '|')
            {
                m[grammar[i][0]].insert(p);
                p = ".";
                continue;
            }
            p += grammar[i][j];
        }
        if (p != ".")
            m[grammar[i][0]].insert(p);
    }

    for (int i = 0; i < n_pro; i++)
    {
        if (vis[grammar[i][0]] == 0)
        {
            calculateFirst(grammar[i][0], m, first, vis, epsilon);
        }
    }
    unordered_map<char, unordered_map<string, unordered_map<char, set<pair<string, string>>>>> items;
    unordered_map<int, unordered_map<char, set<pair<string, string>>>> st;
    unordered_map<char, unordered_map<string, bool>> isComputed;

    compute_closer('Z', "$", m, nter, items, isComputed, epsilon, first);

    cout << endl
         << endl;
    cout << "State: 0" << endl
         << endl;

    //basically printing the closure of Z on lookup '$'
    // storing the state 1 in a variable

    int ptr = 0, states = 0;

    for (auto i : items['Z']["$"])
    {
        st[0][i.first] = i.second;
        for (auto j : i.second)
        {
            cout << i.first << " --> " << j.first << " , ";
            for (int k = 0; k < j.second.size() - 1; k++)
            {
                cout << j.second[k] << "|";
            }
            cout << j.second[j.second.size() - 1] << endl;
        }
    }

    cout << "COMPUTED STATE-0" << endl
         << endl;

    // compute_states(ptr, states, 'S', st, items, m, nter, isComputed, epsilon, first);

    while (ptr <= states)
    {
        for (auto i : nter)
            compute_states(ptr, states, i, st, items, m, nter, isComputed, epsilon, first);

        for (auto i : ter)
            compute_states(ptr, states, i, st, items, m, nter, isComputed, epsilon, first);
        ptr++;
    }

    return 0;
}

/*



3
a
b
c
2
S
A
2
S=A
A=aAb|c
S



*/