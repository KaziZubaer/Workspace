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
#define MAXN 100005
using namespace std;

typedef long long LL;
typedef unsigned long long ULL;
typedef long double LD;

const double PI=acos(-1.0);

int main() {
    int i,ans;
    LL x=2,n;
    vector<LL>pow2;
    pow2.PB(1);
    pow2.PB(2);
    for(i=2;i<86;i++) {
        x=pow2[i-1]+pow2[i-2]+1;
        pow2.PB(x);
    }
    cin>>n;
    n--;
    ans=upper_bound(pow2.begin(),pow2.end(),n)-pow2.begin();
    cout<<ans<<endl;
    return 0;
}
