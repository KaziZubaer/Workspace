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

vector<LL>a;

int main() {
    FRI;
    LL need,x;
    int i,n;
    bool flag=false;
    scanf("%lld",&need);
    scanf("%d",&n);
    for(i=0;i<n;i++) {
        scanf("%lld",&x);
        a.PB(x);
    }
    sort(a.begin(),a.end());
    for(i=0;i<n;i++) {
        x=need-a[i];
        if(binary_search(a.begin(),a.end(),x)) {
            flag=true;
            break;
        }
    }
    if(flag) printf("1\n");
    else printf("0\n");
    return 0;
}
