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

int main() {
    int sq=0,rect=0,other=0,a,b,c,d;
    while(scanf("%d %d %d %d",&a,&b,&c,&d)!=EOF) {
        if(a<=0||b<=0||c<=0||d<=0) other++;
        else if(a==b&&b==c&&c==d) sq++;
        else if(a==c&&b==d) rect++;
        else other++;
    }
    cout<<sq<<" "<<rect<<" "<<other<<endl;
    return 0;
}
