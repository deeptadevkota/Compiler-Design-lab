#include<bits/stdc++.h>

using namespace std;

int compute_items()
{

}



int main()
{
    int n_ter,n_nter;
    cin >> n_ter;

    unordered_set<char> ter,nter;
    char c; 

    for(int i=0;i<n_ter;i++)
    {   
        cin >> c;
        ter.insert(c);
    }

    cin >> n_nter;

    for(int i=0;i<n_nter;i++)
    {  
        cin >> c;
        nter.insert(c);
    }

    int n_pro;
    cin >> n_pro;

    vector<string> grammar(n_pro+1);

    for (int i = 1; i <= n_pro; i++)
    {
        cin >> grammar[i];
    }

    

    string start_symbol;
    cin >> start_symbol;

    grammar[0]="Z="+start_symbol;

    // cout << "The augmented grammar is:" << endl;

    // for(int i=0;i<=n_pro;i++)
    //     cout << grammar[i] << endl;

    unordered_map<char, set<string>> m;

    for (int i = 0; i <=n_pro; i++)
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

    
  
    unordered_map<int,unordered_map<char, set<string>>> st;

  



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