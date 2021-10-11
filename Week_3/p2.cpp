#include <bits/stdc++.h>

using namespace std;

void compute_closure(char c, unordered_map<char, set<string>> &m, vector<bool> &is_computed, unordered_map<char, unordered_map<char, set<string>>> &items, unordered_set<char> &nter)
{
    items[c][c] = m[c];

    for (auto j : m[c])
    {

        if (nter.find(j[1]) != nter.end())
        {

            if (is_computed[c] == false && c != j[1])
                compute_closure(j[1], m, is_computed, items, nter);

            for (auto k : items[j[1]])
            {
                items[c][k.first] = k.second;
            }
        }
    }

    is_computed[c] = true;
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

void compute_states(int &ptr, int &states, char i, unordered_map<int, unordered_map<char, set<string>>> &st, unordered_map<char, unordered_map<char, set<string>>> &items)
{
    unordered_map<char, set<string>> temp;
    for (auto j : st[ptr])
    {

        for (auto k : j.second)
        {
            // checking if there can occur a transition on i from on
            // a grammar of the from j.first --> k

            if (check(k, i))
            {

                //if yes then
                int ind = find_index(k);
                swap(k[ind], k[ind + 1]);
                temp[j.first].insert(k);

                //now iterate through the items

                if ((ind + 2) < k.size())
                {
                    for (auto l : items[k[ind + 2]])
                    {
                        for (auto loop : l.second)
                            temp[l.first].insert(loop);
                    }
                }
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
                cout << i.first << " --> " << j << endl;
        }
        cout << endl
             << endl;
    }
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

    string str = "Z=";
    str.push_back(start_symbol);

    grammar[0] = str;

    cout << endl
         << endl;
    cout << "The augmented grammar is:" << endl;

    for (int i = 0; i <= n_pro; i++)
        cout << grammar[i] << endl;

    cout << endl
         << endl;
    cout << "The states for the given grammar are: " << endl
         << endl;

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

    vector<bool> is_computed(256, 0);
    unordered_map<char, unordered_map<char, set<string>>> items;
    unordered_map<int, unordered_map<char, set<string>>> st;

    compute_closure('Z', m, is_computed, items, nter);
    for (auto i : nter)
    {
        if (is_computed[i] == false)
            compute_closure(i, m, is_computed, items, nter);
    }

    cout << endl;
    cout << "State 0:" << endl;
    for (auto i : items['Z'])
    {
        st[0][i.first] = i.second;
        for (auto j : i.second)
        {
            cout << i.first << " --> " << j << endl;
        }
    }

    cout << endl
         << endl;

    int ptr = 0, states = 0;

    while (ptr <= states)
    {
        for (auto i : nter)
        {
            compute_states(ptr, states, i, st, items);
        }
        for (auto i : ter)
        {
            compute_states(ptr, states, i, st, items);
        }
        ptr++;
    }
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


SAMPLE INPUT:2

3
a
b
#
2
A
S
2
S=AA
A=aA|b
S

SAMPLE INPUT:3

3
a
b
#
3
A
B
S
3
S=AaAb|BbBa
A=#
B=#


SAMPLE INPUT:4
3
+
i
#
2
E
T
3
E=T+E
E=T
T=i
E


5
+
*
(
)
i
3
E
T
F
3
E=E+T|T
T=T*F|F
F=(E)|i
E

*/