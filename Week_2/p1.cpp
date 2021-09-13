#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// considering one letter non-terminal and terminals
// considering small letter words as terminals
// *, /, +, -, (, ), [,],{,}, # are considered as terminals

// bool is_terminal(char c)
// {
//     if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '#')
//         return true;
//     return false;
// }

void calculateFirst(char c, unordered_map<char, vector<string>> &m, vector<string> &first, vector<char> &vis, vector<bool> &epsilon)
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
    vector<char> vis(256, 0);
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

    for (int i = 0; i < n; i++)
    {
        if (vis[grammar[i][0]] == 0)
        {
            calculateFirst(grammar[i][0], m, first, vis, epsilon);
        }
    }
    cout << "Non-terminal" << '\t' << "First()" << '\t' << '\t' << "is_epsilon" << endl;

    for (int i = 0; i < n; i++)
    {
        cout << grammar[i][0] << '\t' << '\t' << first[grammar[i][0]] << "\t" << '\t' << epsilon[grammar[i][0]] << endl;
    }

    return 0;
}