#include <bits/stdc++.h>

using namespace std;

int main()
{
    unordered_map<int, unordered_map<char, string>> PT;

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

    vector<string> grammar(n_pro);

    for (int i = 0; i < n_pro; i++)
    {
        cin >> grammar[i];
    }

    unordered_map<int, pair<char, string>> m;

    int cnt = 0;
    for (int i = 0; i < n_pro; i++)
    {
        string p = "";
        for (int j = 2; j < grammar[i].size(); j++)
        {
            if (grammar[i][j] == '|')
            {
                m[cnt].first = grammar[i][0];
                m[cnt].second = p;
                cnt++;
                p = "";
                continue;
            }
            p += grammar[i][j];
        }
        if (p != "")
        {
            m[cnt].first = grammar[i][0];
            m[cnt].second = p;
            cnt++;
        }
    }

    // for (int i = 0; i < cnt; i++)
    // {
    //     cout << i << " : " << m[i].first << " --> " << m[i].second << endl;
    // }
    // return 0;
    int n_states;
    cin >> n_states;

    for (int i = 0; i <= n_states; i++)
    {
        for (auto j : ter)
        {
            // cout << i << " " << j << " --> ";
            cin >> PT[i + '0'][j];
        }
        // cout << i << " "
        //      << "$"
        //      << " --> ";
        cin >> PT[i + '0']['$'];
        for (auto j : nter)
        {
            // cout << i << " " << j << " --> ";
            cin >> PT[i + '0'][j];
        }
    }

    string str;
    cin >> str;
    str.push_back('$');

    stack<char> st;
    st.push('0');
    int ptr = 0;
    bool found = false;
    while (!st.empty() && ptr < str.size())
    {
        if (st.top() < '0' || st.top() > (n_states + '0'))
        {
            char top2 = st.top();
            st.pop();
            string res = PT[st.top()][top2];
            cout << st.top() << " " << top2 << " --> " << res << endl;
            if (res == "accept")
            {
                found = true;
                break;
            }
            else if (res == "^")
            {
                found = false;
                break;
            }
            st.push(top2);
            st.push(res[0]);
        }
        string res = PT[st.top()][str[ptr]];
        cout << st.top() << " " << str[ptr] << " --> " << res << endl;
        if (res == "accept")
        {
            found = true;
            break;
        }
        else if (res == "^")
        {
            found = false;
            break;
        }

        if (res[0] == 'r')
        {
            int nth_g = res[1] - 48;
            nth_g--;
            int len = m[nth_g].second.size();
            len *= 2;
            while (len--)
            {
                if (st.empty())
                {
                    found = false;
                    break;
                }
                st.pop();
            }
            st.push(m[nth_g].first);
        }
        else
        {
            st.push(str[ptr]);
            char to_state = res[1];
            st.push(to_state);
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
}

/*
2
a
b
2
A
S
2
S=AA
A=aA|b

6
 s4
s3
^
1
2
^
^ 
accept
 ^
 ^
 s4
 s3
 ^
 ^
 5
 s4
 s3
  ^
^
 6
 r3
r3
 r3
 ^
 ^
 r1
r1
 r1
 ^
 ^
 r2
 r2
 r2
 ^
 ^

*/