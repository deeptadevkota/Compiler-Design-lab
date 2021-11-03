#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    int n;
    cout << "Enter the variables count" << endl;
    cin >> n;
    vector<char> var, typ;
    int flag = 0;
    for (int i = 0; i < n; i++)
    {
        cout << "Enter the variable" << endl;
        cin >> var[i];
        cout << "Enter the variable type" << endl;
        cin >> typ[i];
        if (typ[i] == 'F')
        {
            flag = 1;
        }
    }
    cout << "Enter the expression" << endl;
    string exp;
    cin >> exp;
    for (int i = 0; i < exp.size(); i++)
    {
        if (exp[i] == '/')
        {
            flag = 1;
            break;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (exp[0] == var[i])
        {
            if (flag == 1)
            {
                if (typ[i] == 'F')
                {
                    cout << "Type checking passed" << endl;
                    break;
                }
                else
                {
                    cout << var[i] << " variable must be a floating type" << endl;
                    break;
                }
            }
            else
            {
                cout << "Type checking passed" << endl;
                break;
            }
        }
    }
    return 0;
}