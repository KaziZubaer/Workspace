#include<bits/stdc++.h>
#define PB push_back
#define MP make_pair
#define F first
#define S second
#define FRI freopen("in.txt","r",stdin)
#define FRO freopen("out.txt","w",stdout)
#define debug(args...) {dbg,args; cerr<<endl;}
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

struct debugger{
    template<typename T> debugger& operator , (const T& v){
        cerr<<v<<" ";
        return *this;
    }
}dbg;

int cnt[MAXN],prime[MAXN],mobi[MAXN],CNT=0;
bool composite[MAXN];
bool mobiZero[MAXN];

void SieveWithMobi() {
    int len=0;
    for(LL i=2;i<MAXN;i++)
        if(!composite[i]) {
            prime[len++]=i;
            for(LL j=i;j<MAXN;j+=i) {
                cnt[j]++;
                composite[j]=true;
            }
            for(LL j=i*i;j<MAXN;j+=i*i)
                mobiZero[j]=true;
        }
    for(int i=2;i<MAXN;i++) {
        if(mobiZero[i]) mobi[i]=0;
        else if(cnt[i]&1) mobi[i]=-1;
        else mobi[i]=1;
    }
}

void Solve(int t) {
    LL n,m,i,ans;
    scanf("%lld %lld",&n,&m);
    ans=n*m;
    for(i=2;i<=min(n,m);i++) ans+=mobi[i]*(n/i)*(m/i);
    ans+=2;
    if(m==0&&n==0) ans-=2;
    else if(n==0||m==0) ans--;
    printf("Case %d: %lld\n",t,ans);
}

int main() {
    int T,t=0;
    SieveWithMobi();
    scanf("%d",&T);
    while(T--) Solve(++t);
    return 0;
}
