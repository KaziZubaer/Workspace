#include<bits/stdc++.h>

#define PB push_back
#define MP make_pair
#define F first
#define S second

#define FRI freopen("in.txt","r",stdin)
#define FRO freopen("out.txt","w",stdout)
#define debug(args...) {dbg,args; cerr<<endl;}
#define DB(x) #x"=>",x
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

struct debugger{
    template<typename T> debugger& operator , (const T& v){
        cerr<<v<<" ";
        return *this;
    }
}dbg;

double dp[15][10005];

int main() {
    int c,b,n,p,i,j,k,loTotal=0,mini;
    double s,ans;
    scanf("%d %d",&c,&b);
    for(j=0;j<=b;j++) dp[0][j]=0;
    for(i=1;i<=c;i++) {
        for(j=0;j<=b;j++) dp[i][j]=0;
        scanf("%d",&n);
        mini=1000000;
        for(j=0;j<n;j++) {
            scanf("%d %lf",&p,&s);
            mini=min(mini,p);
            for(k=p;k<=b;k++) dp[i][k-p]=max(dp[i][k-p],dp[i-1][k]+s);
        }
        loTotal+=mini;
    }
    if(loTotal>b) printf("-1\n");
    else {
        ans=0;
        for(i=0;i<=b;i++) ans=max(ans,dp[c][i]);
        printf("%.2lf\n",ans);
    }
    return 0;
}
