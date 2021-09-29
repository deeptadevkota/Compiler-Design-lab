#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int main()
{
    //  Let given grammar is G = (V, T, S, P)

    cout << "Enter the number of non-terminal and terminal respectively:" << endl;

    int n_ter, ter, ss;
    cin >> n_ter >> ter;

    unordered_set<char> v(n_ter), t(ter);

    //non terminals
    cout << "Enter non-terminals:" << endl;
    for (int i = 0; i < n_ter; i++)
    {
        char c;
        cin >> c;
        v.insert(c);
    }

    cout << "Enter terminals:" << endl;
    //terminals
    for (int i = 0; i < ter; i++)
    {
        char c;
        cin >> c;
        t.insert(c);
    }

    //starting symbol
    char start_symbol;
    cin >> start_symbol;

    //accepting the parsing table
    unordered_map<char, unordered_map<char, string>> PT;

    cout << "Enter the productions:" << endl;
    for (auto i = v.begin(); i != v.end(); i++)
    {
        char non_ter = *i;
        for (auto j = t.begin(); j != t.end(); j++)
        {
            char ter = *j;
            // cout << non_ter << " " << ter << " : ";
            string a, b, c;
            cin >> a >> b >> c;
            string production;
            cin >> production;
            PT[non_ter][ter] = production;
        }
    }
    //string for testing the parsing algorithm
    cout << "Enter the string to be tested" << endl;
    string test;
    cin >> test;
    test.push_back('$');

    //stack for the parsing algorithm
    stack<char> st;
    st.push('$');
    st.push(start_symbol);

    int ptr = 0;
    bool flag = 0;
    while (!st.empty() && ptr < test.size())
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
                break;
            }
        }
        else if (v.find(A) != v.end())
        {
            if (PT[A][test[ptr]] != "^")
            {
                st.pop();
                string rule = PT[A][test[ptr]];
                if (rule == "#")
                    continue;
                for (int i = rule.size() - 1; i >= 0; i--)
                {
                    st.push(rule[i]);
                }
            }
            else
            {
                flag = 1;
                break;
            }
        }
    }

    if (flag == 1 || !st.empty())
        cout << "String is not accepted by the parser" << endl;
    else
        cout << "String is accepted by the parser" << endl;
}

/*

SAMPLE INPUT:

1
3
S
(
)
$
S
S $ : #
S ) : #
S ( : (S)
((()))

SAMPLE OUTPUT:

String is accepted by the parser





6
6
S
A
B
C
D
E
a
b
c
d
e
$
S


SAMPLE INPUT 2:

6
6
S
A
B
C
D
E
a
b
c
d
e
$
S
D e : #
D d : d
D $ : #
D c : ^
D b : ^
D a : ^
C e : ^
C d : ^
C $ : ^
C c : c
C b : ^
C a : ^
B e : ^
B d : ^
B $ : ^
B c : #
B b : b
B a : ^
A e : ^
A d : ^
A $ : ^
A c : #
A b : #
A a : a
E e : e
E d : ^
E $ : #
E c : ^
E b : ^
E a : ^
S e : ^
S d : ^
S $ : ^
S c : ABCDE
S b : ABCDE
S a : ABCDE
ace
String is accepted by the parser




*/