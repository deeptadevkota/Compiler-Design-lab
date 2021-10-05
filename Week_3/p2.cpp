#include <bits/stdc++.h>

using namespace std;

void compute_items(char c, unordered_map<char, set<string>> &m, vector<bool> &is_computed, unordered_map<char, unordered_map<char, set<string>>> &items, unordered_set<char> &nter)
{

    items[c][c] = m[c];

    for (auto j : m[c])
    {

        if (nter.find(j[1]) != nter.end())
        {
            if (is_computed[c] == false)
                compute_items(j[1], m, is_computed, items, nter);

            for (auto k : items[j[1]])
            {
                items[c][k.first] = k.second;
            }
        }
        
    }

    is_computed[c] = true;
}

int main()
{
    int n_ter, n_nter;
    cin >> n_ter;

    unordered_set<char> ter, nter;
    char c;

    for (int i = 0; i < n_ter; i++)
    {
        cin >> c;
        ter.insert(c);
    }

    cin >> n_nter;

    for (int i = 0; i < n_nter; i++)
    {
        cin >> c;
        nter.insert(c);
    }

    int n_pro;
    cin >> n_pro;

    vector<string> grammar(n_pro + 1);

    for (int i = 1; i <= n_pro; i++)
    {
        cin >> grammar[i];
    }

    char start_symbol;
    cin >> start_symbol;


    string str="Z=";
    str.push_back(start_symbol);
    // cout << "str: " << str << endl;
    grammar[0] = str;
    // return 0;

    // cout << "The augmented grammar is:" << endl;

    // for(int i=0;i<=n_pro;i++)
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
    


    // for(auto i:m['Z'])
    // {
    //     cout << i << endl;
    // }

    vector<bool> is_computed(256,0);
    unordered_map<char, unordered_map<char, set<string>>> items;

    compute_items('Z', m, is_computed, items, nter);

    cout << endl;
    cout << "State 1:" << endl;
    for (auto i : items['Z'])
    {
        for (auto j : i.second)
            cout << i.first << " --> " << j << endl;
    }

    unordered_map<int, unordered_map<char, set<string>>> st;


    return 0;
}

/*
SAMPLE INPUT

4
a
b
c
d
5
A
B
C
D
S
5
S=ABCD|DA    
A=a
B=b
C=c
D=d
S

*/