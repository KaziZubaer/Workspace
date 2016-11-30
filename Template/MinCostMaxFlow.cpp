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
#define MOD 1000000007
#define MAXN 105
#define MAX_E 10010
#define MAX_V 210
#define INF 1e18
using namespace std;

typedef long long LL;
const double PI=acos(-1.0);

struct debugger{
    template<typename T> debugger& operator , (const T& v) {
        cerr<<v<<" ";
        return *this;
    }
}dbg;

class PT {
public:
    double x,y;
    PT() {}
    PT(double x,double y) : x(x), y(y) {}
    PT(const PT &p) : x(p.x), y(p.y)    {}
    PT operator - (const PT &p)  const { return PT(x-p.x, y-p.y); }
};

double dot(PT p,PT q)     { return p.x*q.x+p.y*q.y; }
LL dist(PT p,PT q) {
    double ret=sqrt(dot(p-q,p-q))*1e12;
    return (LL)ret;
}

struct NODE{
    int v;
    LL Cap,Cost;
    int RevInd;
};

vector<NODE>Edge[MAX_V+7];
int nV;
int SRC,TNK;
// This PInd is neceserry for multigraph to knw which edge ind of parent is used to take flow
int Par[MAX_V+7],PInd[MAX_V+7];
LL SD[MAX_V+7];

void Reset(int n) {
    int i;
    for(i=0;i<n;i++) Edge[i].clear();
}

void SetEdge(int u,int v,LL Cap,LL Cost) {
    NODE U={v,Cap,Cost,(int)Edge[v].size()};
    NODE V={u,0,-Cost,(int)Edge[u].size()};
    Edge[u].push_back(U);
    Edge[v].push_back(V);
}

bool BFord() {
    int i,u;
    LL TD;
    for(i=0;i<nV;i++) {
        Par[i]=-1;
        SD[i]=INF;
    }
    bool IsChange=true;
    SD[SRC]=0;
    int cnt=0;
    while(IsChange) {
        cnt++;
        assert(cnt<100000);
        IsChange=false;
        for(u=SRC;u<=TNK;u++)
            for(i=0;i<Edge[u].size();i++) {
                if(!Edge[u][i].Cap) continue;
                int v=Edge[u][i].v;
                TD=SD[u]+Edge[u][i].Cost;
                // relaxation
                if(SD[v]>TD) {
                    SD[v]=TD;Par[v]=u;PInd[v]=i;
                    IsChange=true;
                }
            }
    }
    return Par[TNK]!=-1;
}

LL FindVol(int s,int t) {
    LL Cap=Edge[Par[t]][PInd[t]].Cap;
    if(s==Par[t]) return Cap;
    else return min(Cap,FindVol(s,Par[t]));
}

LL AugmentPath(int s,int t,LL V) {
    if(s==t) return 0;
    LL Cost=Edge[Par[t]][PInd[t]].Cost*V;
    Edge[Par[t]][PInd[t]].Cap-=V;
    Edge[t][Edge[Par[t]][PInd[t]].RevInd].Cap+=V;
    return Cost+AugmentPath(s,Par[t],V);
}

void MinCost(LL &Flow,LL &Cost) {
    Flow=Cost=0;
    while(BFord()) {
        LL V=FindVol(SRC,TNK);
        Flow+=V;
        Cost+=AugmentPath(SRC,TNK,V);
    }
}

void Solve(int n,bool &first) {
    int i,j;
    LL Fl,Cs;
    PT ant[MAXN],tree[MAXN];
    for(i=0;i<n;i++) scanf("%lf %lf",&ant[i].x,&ant[i].y);
    for(i=0;i<n;i++) scanf("%lf %lf",&tree[i].x,&tree[i].y);
    nV=2*n+2;
    Reset(nV);
    for(i=0;i<n;i++) {
        SetEdge(0,1+i,1,0);
        SetEdge(n+1+i,2*n+1,1,0);
        for(j=0;j<n;j++)
            SetEdge(1+i,n+1+j,1,dist(ant[i],tree[j]));
    }
    SRC=0;
    TNK=nV-1;
    MinCost(Fl,Cs);
    if(!first) printf("\n");
    for(i=1;i<=n;i++)
        for(j=0;j<Edge[i].size();j++)
            if(Edge[i][j].Cap<0.5) {
                printf("%d\n",Edge[i][j].v-n);
                break;
            }
    first=false;
}

int main() {
    FRI;
    int n;
    bool first=true;
    while(scanf("%d",&n)!=EOF) Solve(n,first);
    return 0;
}
