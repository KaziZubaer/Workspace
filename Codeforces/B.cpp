#include<bits/stdc++.h>

#define PB push_back
#define MP make_pair
#define F first
#define S second

#define FRI freopen("in.txt","r",stdin)
#define FRO freopen("out.txt","w",stdout)
#define RAD(x) ((x*PI)/180)
#define NEX(x) ((x)==n-1?0:(x)+1)
#define PRE(x) ((x)==0?n-1:(x)-1)
#define DEG(x) ((x*180)/PI)

#define EPS 1e-12
#define INF 1000000007
#define MOD 1000000007
#define MAXN 1000005
using namespace std;

typedef long long LL;
typedef unsigned long long ULL;
typedef long double LD;

const double PI=acos(-1.0);

LL prime[MAXN],len;
bitset<22370>composite;

void MakePrimeList() {
    len=1;
    composite[1]=1;
    prime[0]=2;
    for(LL i=2;i<22370;i++)
        if(composite[i]==0){
            prime[len]=2*i-1;
            len++;
            for(LL j=((2*i-1)*(2*i-1))/2+1;j<15805;j+=2*i-1)
                composite[j]=1;
        }
}

bool isPrime(LL x) {
    int i=0;
    while(i<len&&prime[i]*prime[i]<=x) {
        if(x%prime[i]==0)
            return false;
        i++;
    }
    return true;
}

int main() {
    LL n;
    MakePrimeList();
    cin>>n;
    if(isPrime(n)) cout<<"1\n";
    else if(n%2==0) cout<<"2\n";
    else if(isPrime(n-2)) cout<<"2\n";
    else cout<<"3\n";
    return 0;
}
