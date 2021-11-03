#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main()
{

    int n, i, flag;
    vector<char> variable, type;
    char c;
    string expression;

    cout << "\nEnter the number of variables: ";
    cin >> n;

    flag=0;
    for(i=0;i<n;i++)
    {
        cout << "\nEnter the variable " << i << " : ";
        fflush(stdin);
        cin >> c;
        variable.push_back(c);

        cout << "Enter the variable type [f: float, i: int]: ";
        fflush(stdin);
        cin >> c;
        type.push_back(c);

        if(c=='f')
            flag=1;
    }

    cout << "\nEnter the Expression: " ;
    fflush(stdin);
    cin >> expression;

    for(i=0;i<expression.length();i++)
    {
        if(expression[i]=='/')
        {
            flag=1;
            break;  
        }  
    }

    for(i=0;i<n;i++)
    {
        if(expression[0]==variable[i])
        {
            if(flag==1)
            {
                if(type[i]=='f')
                {  
                    cout << "\nThe datatype is correctly defined\n";
                    break;  
                }
                else
                {  
                    cout << "\nIdentifier \"" << variable[i] << "\" must be of type float\n";
                    break;  
                }  
            }
            else
            {  
                cout <<"\nThe datatype is correctly defined\n";
                break;  
            }  
        }
    }


    return 0;
}