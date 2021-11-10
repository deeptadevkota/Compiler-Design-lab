#include <iostream>

using namespace std;

struct equation
{
    char lf;
    string rt;
};

int main()
{
    int n;
    cin >> n;
    equation inp[n];
    for (int i = 0; i < n; i++)
    {
        cin >> inp[i].lf >> inp[i].rt;
    }
    cout << "The given segment of code is as follows:" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << inp[i].lf << " = " << inp[i].rt << endl;
    }

    cout << "\n\nRemoving the dead code...." << endl
         << endl;
    // the loop is till n-1 the last equation LHS stores the answers
    equation d_op[n];
    int itr = 0;

    for (int i = 0; i < n - 1; i++)
    {
        int flag = 0;
        char temp = inp[i].lf;
        for (int j = 0; j < n; j++)
        {

            for (int k = 0; k < inp[j].rt.size(); k++)
            {
                if (temp == inp[j].rt[k])
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
            {
                d_op[itr].lf = temp;
                d_op[itr].rt = inp[i].rt;
                itr++;
                break;
            }
        }
    }
    d_op[itr].lf = inp[n - 1].lf;
    d_op[itr].rt = inp[n - 1].rt;
    itr++;
    cout << "\nCode segment after dead code removal" << endl;
    for (int i = 0; i < itr; i++)
    {
        cout << d_op[i].lf << " = " << d_op[i].rt << endl;
    }

    cout << "\n\nApplying variable propagation...." << endl
         << endl;
    for (int i = 0; i < itr; i++)
    {
        string str = d_op[i].rt;
        for (int j = i + 1; j < itr; j++)
        {
            if (str == d_op[j].rt)
            {
                char temp = d_op[j].lf;
                //replacing the current LHS with the LHS of the previous equation with the same RHS
                d_op[j].lf = d_op[i].lf;

                for (int k = 0; k < itr; k++)
                {
                    //checking the if the replaced LHS is present in any equations
                    if (temp == d_op[k].rt[k])
                    {
                        d_op[k].rt[k] = d_op[j].lf;
                    }
                }
            }
        }
    }
    cout << "\nCode segment after applying variable propagation" << endl;
    for (int i = 0; i < itr; i++)
    {
        cout << d_op[i].lf << " = " << d_op[i].rt << endl;
    }

    cout << "\nDead code can occur after applying variable propagation" << endl;
    cout << "\n\nFurther removing the dead code...." << endl
         << endl;

    for (int i = 0; i < itr; i++)
    {
        for (int j = i + 1; j < itr; j++)
        {
            if (d_op[i].lf == d_op[j].lf && d_op[i].rt == d_op[j].rt)
            {
                d_op[i].rt = "Null";
            }
        }
    }

    cout << "\nThe code segment obtained after optimization is as follows:\n"
         << endl;
    for (int i = 0; i < itr; i++)
    {
        if (d_op[i].rt != "Null")
            cout << d_op[i].lf << " = " << d_op[i].rt << endl;
    }
}

/*
SAMPLE INPUT-1:
5
a
9
b
c+d
e
c+d
f
b+e
r
f

SAMPLE INPUT-2
3
e
a*b
x
a
d
x*b+4

SAMPLE INPUT-3:
3
c
a*b
x
a
d
a*b+4



*/