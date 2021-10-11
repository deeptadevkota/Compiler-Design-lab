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
                pt_entry.push_back(itr + '0');
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
            pt_entry.push_back(states + '0');
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

bool check_reduce(unordered_map<int, unordered_map<char, set<string>>> &st, int &ptr)
{
    for (auto j : st[ptr])
    {

        for (auto k : j.second)
        {
            if (k[k.size() - 1] == '.')
            {
                return true;
            }
        }
    }
    return false;
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
        if (check_reduce(st, ptr))
        {
            char LHS = (*st[ptr].begin()).first;
            if (LHS == 'Z')
            {
                for (auto i : ter)
                    PT[ptr][i] = "^";
                PT[ptr]['#'] = "accept";
                ptr++;
                continue;
            }
            string RHS = ".";
            RHS += *(st[ptr][LHS].begin());
            RHS = RHS.substr(0, RHS.size() - 1);
            // cout << "***********  " << LHS << " " << RHS << endl;
            for (auto x : ordered_grammar)
            {
                int nth_grammar = x.first;
                auto pair_grammar = x.second;

                // cout << "CHECKING" << endl;
                // cout << pair_grammar.first << " " << pair_grammar.second << endl;

                if ((pair_grammar.first == LHS) && (pair_grammar.second == RHS))
                {

                    string pt_entry = "r";
                    pt_entry.push_back(nth_grammar + '0');

                    for (auto i : ter)
                        PT[ptr][i] = pt_entry;
                }
            }
        }
        else
            for (auto i : ter)
            {
                compute_states(ptr, states, i, st, items, PT, ordered_grammar, nter, ter);
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

    string test_str;
    cin >> test_str;
    test_str.push_back('#');

    stack<char> st_data;
    st_data.push('0');
    ptr = 0;
    bool found = false;
    int n_states = states;

    cout << "The shift and reduce operations used are: " << endl
         << endl;

    while (!st_data.empty() && ptr < test_str.size())
    {
        if (st_data.top() < '0' || st_data.top() > (n_states + '0'))
        {
            char top2 = st_data.top();
            st_data.pop();
            int m_int = st_data.top() - '0';
            string res = PT[m_int][top2];
            cout << st_data.top() << " " << top2 << " --> " << res << endl;

            if (res == "accept")
            {
                found = true;
                break;
            }
            else if (res == "^" || res == "")
            {
                found = false;
                break;
            }
            st_data.push(top2);
            st_data.push(res[0]);
        }
        int m_int = st_data.top() - '0';
        string res = PT[m_int][test_str[ptr]];
        cout << st_data.top() << " " << test_str[ptr] << " --> " << res << endl;
        if (res == "accept")
        {
            found = true;
            break;
        }
        else if (res == "^" || res == "")
        {
            found = false;
            break;
        }

        if (res[0] == 'r')
        {
            int nth_g = res[1] - 48;
            int len = ordered_grammar[nth_g].second.size();
            len--;
            len *= 2;
            while (len--)
            {
                if (st_data.empty())
                {
                    found = false;
                    break;
                }
                st_data.pop();
            }

            st_data.push(ordered_grammar[nth_g].first);
        }
        else
        {
            st_data.push(test_str[ptr]);
            char to_state = res[1];
            st_data.push(to_state);
            ptr++;
        }
    }
    if (found == true)
    {
        cout << "Given string is accepted" << endl;
    }
    else
    {
        cout << "Given string is not accepted" << endl;
    }

    return 0;
}

/*

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

*/