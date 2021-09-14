#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using namespace std;

void ERROR()
{
}

int main()
{
    //  Let given grammar is G = (V, T, S, P)

    int n_ter, ter, ss, pro;
    cin >> n_ter >> ter >> pro;

    unordered_set<char> v(n_ter), t(ter);
    vector<string> p(pro);

    //non terminals
    for (int i = 0; i < n_ter; i++)
    {
        char c;
        cin >> c;
        v.insert(c);
    }

    //terminals
    for (int i = 0; i < ter; i++)
    {
        char c;
        cin >> c;
        t.insert(c);
    }

    //starting symbols
    char start_symbol;
    cin >> start_symbol;

    //productions
    for (int i = 0; i < pro; i++)
    {
        cin >> p[i];
    }

    //accepting the parsing table
    unordered_map<char, unordered_map<char, string>> PT;

    for (auto i = v.begin(); i != v.end(); i++)
    {
        char non_ter = *i;
        for (auto j = t.begin(); j != t.end(); j++)
        {
            string production;
            cin >> production;
            PT[non_ter][*j] = production;
        }
    }
    //string for testing the parsing algorithm
    string test;
    cin >> test;

    //stack for the parsing algorithm
    stack<char> st;
    st.push('$');
    st.push(start_symbol);

    int ptr = 0;
    bool flag = 0;
    while (!st.empty())
    {
        char A = st.top();
        if (t.find(A) != t.end() || A == '$')
        {
            if (A == test[ptr])
            {
                st.pop();
                ptr++;
            }
            else
            {
                flag = 1;
                ERROR();
                break;
            }
        }
        else if (v.find(A) != v.end())
        {
            if (PT[A][test[ptr]] != "")
            {
                string rule = PT[A][test[ptr]];
                for (int i = rule.size() - 1; i >= 0; i--)
                {
                    st.push(rule[i]);
                }
            }
            else
            {
                flag = 1;
                ERROR();
                break;
            }
        }
    }

    if (flag == 1)
        cout << "String is accepted by the parser" << endl;
    else
        cout << "String is not accepted by the parser" << endl;
}