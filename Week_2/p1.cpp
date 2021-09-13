#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// Considering one letter non-terminal and terminals

void calculateFirst(char c, unordered_map<char, vector<string>> &m, vector<string> &first, vector<bool> &vis, vector<bool> &epsilon)
{
    string first_c = "";
    for (string i : m[c])
    {
        if (i == "#")
        {
            epsilon[c] = true;
            continue;
        }
        if (vis[i[0]] == 0)
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

void calculateFollow(char c, unordered_map<char, vector<string>> &m, vector<string> &first, vector<bool> &vis_f, vector<bool> &epsilon, vector<string> &follow)
{
    for (auto j : m)
    {
        for (string str : j.second)
        {
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

int main()
{
    int n;
    cin >> n;
    vector<string> grammar(n);
    for (int i = 0; i < n; i++)
    {
        cin >> grammar[i];
    }
    vector<string> first(256, "");
    vector<string> follow(256, "");
    vector<bool> vis(256, 0);
    vector<bool> vis_f(256, 0);
    vector<bool> epsilon(256, false);

    // enter the terminals
    //base case where terminal gives terminal itself
    int ts;
    cin >> ts;
    for (int i = 0; i < ts; i++)
    {
        char t;
        cin >> t;
        first[t] = t;
        follow[t] = t;
        vis[t] = 1;
    }

    unordered_map<char, vector<string>> m;

    for (int i = 0; i < n; i++)
    {
        string p = "";
        for (int j = 2; j < grammar[i].size(); j++)
        {
            if (grammar[i][j] == '|')
            {
                m[grammar[i][0]].push_back(p);
                p = "";
                continue;
            }
            p += grammar[i][j];
        }
        if (p != "")
            m[grammar[i][0]].push_back(p);
    }

    //Computing the First() of the given non-terminals

    for (int i = 0; i < n; i++)
    {
        if (vis[grammar[i][0]] == 0)
        {
            calculateFirst(grammar[i][0], m, first, vis, epsilon);
        }
    }

    //removing duplicates from first()

    cout << endl
         << endl;

    //Computing the Follow() of the given non-terminals

    //considering grammar[0][0] as the starting symbol

    follow[grammar[0][0]] = "$";

    for (int i = 0; i < n; i++)
    {
        if (vis_f[grammar[i][0]] == 0)
            calculateFollow(grammar[i][0], m, first, vis_f, epsilon, follow);
    }

    cout << "Non-terminal" << '\t' << "First" << endl;

    for (int i = 0; i < n; i++)
    {
        cout << grammar[i][0] << '\t' << '\t';
        unordered_set<char> s;
        for (char c : first[grammar[i][0]])
        {
            if (s.find(c) == s.end())
            {
                cout << c << " ";
                s.insert(c);
            }
        }
        if (epsilon[grammar[i][0]])
            cout << "#";
        cout << endl;
    }
    cout << endl;

    cout << "Non-terminal" << '\t' << "Follow" << endl;

    for (int i = 0; i < n; i++)
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
        cout << endl;
    }

    return 0;
}