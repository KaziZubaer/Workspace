#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

int CeilIndex(int A[],int l,int r,int key) {
    while (r-l>1) {
        int m=l+(r-l)/2;
        if(A[m]>=key)
            r=m;
        else
            l=m;
    }
    return r;
}

//Send a little bit bigger array
int LongestIncreasingSubsequenceLength(int A[], int n) {
    int *tailTable=new int[n];
    int len;
    memset(tailTable,0,sizeof(tailTable[0])*n);
    tailTable[0]=A[0];
    len=1;
    for(int i=1;i<n;i++) {
        if(A[i]<tailTable[0])
            tailTable[0] = A[i];
        else if (A[i]>tailTable[len-1])
            tailTable[len++]=A[i];
        else
            tailTable[CeilIndex(tailTable,-1,len-1,A[i])]=A[i];
    }
    delete[] tailTable;
    return len;
}


/*
 * C++ Program to Implement Knuth–Morris–Pratt Algorithm (KMP)
 */
#include <iostream>
#include <cstring>
using namespace std;
void preKMP(string pattern, int f[])
{
    int m = pattern.length(), k;
    f[0] = -1;
    for (int i = 1; i < m; i++)
    {
        k = f[i - 1];
        while (k >= 0)
        {
            if (pattern[k] == pattern[i - 1])
                break;
            else
                k = f[k];
        }
        f[i] = k + 1;
    }
}

//check whether target string contains pattern
bool KMP(string pattern, string target)
{
    int m = pattern.length();
    int n = target.length();
    int f[m];
    preKMP(pattern, f);
    int i = 0;
    int k = 0;
    while (i < n)
    {
        if (k == -1)
        {
            i++;
            k = 0;
        }
        else if (target[i] == pattern[k])
        {
            i++;
            k++;
            if (k == m)
                return 1;
        }
        else
            k = f[k];
    }
    return 0;
}

int main()
{
    string tar = "san and linux training";
    string pat = "lin";
    if (KMP(pat, tar))
        cout<<"'"<<pat<<"' found in string '"<<tar<<"'"<<endl;
    else
        cout<<"'"<<pat<<"' not found in string '"<<tar<<"'"<<endl;
    pat = "sanfoundry";
    if (KMP(pat, tar))
        cout<<"'"<<pat<<"' found in string '"<<tar<<"'"<<endl;
    else
        cout<<"'"<<pat<<"' not found in string '"<<tar<<"'"<<endl;
    return 0;
}
