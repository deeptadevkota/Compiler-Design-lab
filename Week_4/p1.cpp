#include <bits/stdc++.h>

using namespace std;

void calculateFirst(char c, unordered_map<char, set<string>> &m, vector<string> &first, vector<bool> &vis, vector<bool> &epsilon)
{

    cout << 1 << endl;
    string first_c = "";
    for (string i : m[c])
    {
        i = i.substr(1, i.size() - 1);
        if (i == "#")
        {
            epsilon[c] = true;
            continue;
        }
        if (vis[i[0]] == 0 && i[0] != c)
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
        // if (first_c == "")
        //     epsilon[c] = true;
    }
    // if (first_c == "")
    //     epsilon[c] = true;
    vis[c] = 1;
    first[c] = first_c;

    return;
}

void calculateFollow(char c, unordered_map<char, set<string>> &m, vector<string> &first, vector<bool> &vis_f, vector<bool> &epsilon, vector<string> &follow)
{
    for (auto j : m)
    {
        for (string str : j.second)
        {
            str = str.substr(1, str.size() - 1);
            for (int l = 0; l < str.size(); l++)
            {
                if (str[l] == c)
                {
                    //if l is the last character
                    if (l == (str.size() - 1))
                    {
                        if (vis_f[j.first] == 0 && c != j.first)
                            calculateFollow(j.first, m, first, vis_f, epsilon, follow);
                        follow[c] += follow[j.first];
                    }
                    else
                    {
                        follow[c] += first[str[l + 1]];
                        while ((l + 2) < str.size() && epsilon[str[l + 1]])
                        {
                            l++;
                            follow[c] += first[str[l + 1]];
                        }
                        if (l == (str.size() - 2) && epsilon[str[l + 1]])
                        {
                            if (vis_f[j.first] == 0 && c != j.first)
                                calculateFollow(j.first, m, first, vis_f, epsilon, follow);
                            follow[c] += follow[j.first];
                        }
                    }
                }
            }
        }
    }
    vis_f[c] = 1;
    return;
}

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

void compute_states(int &ptr, int &states, char i, unordered_map<int, unordered_map<char, set<string>>> &st, unordered_map<char, unordered_map<char, set<string>>> &items, unordered_map<int, unordered_map<char, string>> &PT, unordered_map<int, pair<char, string>> &ordered_grammar, unordered_set<char> &nter, unordered_set<char> &ter)
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
    {
        PT[ptr][i] = "^";
        return;
    }

    int flag = 0;
    for (int itr = 0; itr <= states; itr++)
    {
        if (st[itr] == temp)
        {
            if (nter.find(i) == nter.end())
            {
                string pt_entry = "s";
                pt_entry += to_string(itr);
                PT[ptr][i] = pt_entry;
            }
            else
            {
                string pt_entry = to_string(itr);
                PT[ptr][i] = pt_entry;
            }

            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        states++;

        if (nter.find(i) == nter.end())
        {
            string pt_entry = "s";
            pt_entry += to_string(states);
            PT[ptr][i] = pt_entry;
        }
        else
        {
            string pt_entry = to_string(states);
            PT[ptr][i] = pt_entry;
        }

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

    vector<string> first(256, "");
    vector<string> follow(256, "");
    vector<bool> vis(256, 0);
    vector<bool> vis_f(256, 0);
    vector<bool> epsilon(256, false);
    char c;

    for (int i = 0; i < n_ter; i++)
    {
        cin >> c;
        ter.insert(c);

        first[c] = c;
        follow[c] = c;
        vis[c] = c;
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

    unordered_map<char, set<string>> m;
    unordered_map<int, pair<char, string>> ordered_grammar;

    int cnt = 0;
    for (int i = 0; i <= n_pro; i++)
    {
        string p = ".";
        for (int j = 2; j < grammar[i].size(); j++)
        {
            if (grammar[i][j] == '|')
            {
                m[grammar[i][0]].insert(p);
                ordered_grammar[cnt].first = grammar[i][0];
                ordered_grammar[cnt].second = p;
                cnt++;
                p = ".";
                continue;
            }
            p += grammar[i][j];
        }
        if (p != ".")
        {
            m[grammar[i][0]].insert(p);
            ordered_grammar[cnt].first = grammar[i][0];
            ordered_grammar[cnt].second = p;
            cnt++;
        }
    }
    cout << "Grammar is calculated" << endl;
    for (int i = 0; i <= n_pro; i++)
    {
        if (vis[grammar[i][0]] == 0)
            calculateFirst(grammar[i][0], m, first, vis, epsilon);
    }
    cout << "First is calculated" << endl;
    cout << endl
         << endl;

    //Computing the Follow() of the given non-terminals

    follow['Z'] = "$";

    for (int i = 0; i <= n_pro; i++)
    {
        if (vis_f[grammar[i][0]] == 0)
            calculateFollow(grammar[i][0], m, first, vis_f, epsilon, follow);
    }
    cout << "Follow is calculated" << endl;
    unordered_map<char, unordered_set<char>> follow_of;
    for (int i = 0; i <= n_pro; i++)
    {
        cout << grammar[i][0] << '\t' << '\t';
        unordered_set<char> s;
        for (char c : follow[grammar[i][0]])
        {
            if (s.find(c) == s.end())
            {
                cout << c << " ";
                s.insert(c);
            }
        }
        follow_of[grammar[i][0]] = s;
        cout << endl;
    }

    cout << "CHECKING THE FOLLOW OBTAINED" << endl;

    for (auto i : m)
    {
        cout << i.first << " --> ";
        for (auto j : follow_of[i.first])
        {
            cout << j << " ";
        }
        cout << endl;
    }

    vector<bool> is_computed(256, 0);
    unordered_map<char, unordered_map<char, set<string>>> items;
    unordered_map<int, unordered_map<char, set<string>>> st;
    unordered_map<int, unordered_map<char, string>> PT;

    compute_closure('Z', m, is_computed, items, nter);
    for (auto i : nter)
    {
        if (is_computed[i] == false)
            compute_closure(i, m, is_computed, items, nter);
    }
    cout << "The states for the given grammar are: " << endl
         << endl;
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
            compute_states(ptr, states, i, st, items, PT, ordered_grammar, nter, ter);
        }
        for (auto i : ter)
        {
            compute_states(ptr, states, i, st, items, PT, ordered_grammar, nter, ter);
        }

        //SLR(0) logic starts here
        for (auto j : st[ptr])
        {
            if (j.first == 'Z')
            {
                for (auto k : j.second)
                {
                    if (k[k.size() - 1] == '.')
                    {
                        PT[ptr]['$'] = "accept";
                        continue;
                    }
                }
            }

            for (auto k : j.second)
            {
                if (k[k.size() - 1] == '.' && j.first != 'Z')
                {
                    string RHS = ".";
                    RHS += k.substr(0, k.size() - 1);

                    for (auto x : ordered_grammar)
                    {
                        int nth_grammar = x.first;
                        auto pair_grammar = x.second;

                        if ((pair_grammar.first == j.first) && (pair_grammar.second == RHS))
                        {
                            for (auto l : follow_of[j.first])
                            {
                                string pt_entry = "r";
                                pt_entry += to_string(nth_grammar);
                                PT[ptr][l] = pt_entry;
                            }
                        }
                    }
                }
            }
        }
        ptr++;
    }

    cout << "The parsing table is obtained as follows: " << endl
         << endl;
    cout << '\t';

    for (auto i : ter)
        cout << i << '\t';
    for (auto i : nter)
        cout << i << '\t';
    cout << endl;

    for (int i = 0; i <= states; i++)
    {
        cout << i << '\t';
        for (auto j : ter)
            cout << PT[i][j] << '\t';
        for (auto j : nter)
            cout << PT[i][j] << '\t';
        cout << endl;
    }

    cout << endl
         << endl;

    return 0;
}

/*

6
+
*
(
)
i
$
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