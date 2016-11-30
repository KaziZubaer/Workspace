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
#define MAXN 10005
#define MAXV 65
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

struct Edge {
    int u,v,b,c;
    bool inTr;
    Edge() {}
    Edge(int _v,int _c,bool _inTr=false) {
        v=_v;
        c=_c;
        inTr=_inTr;
    }
} allEdges[MAXN];

int maxC,n,m,totV,dfs_num[MAXV],predfn,nG,grp[MAXV],par[MAXV];
bool inStk[MAXV];

void SetGrp(int u,int v,int nn) {
    grp[v]=nn;
    if(u==v) return;
    SetGrp(u,par[v],nn);
}

void DFS(vector< vector<Edge> >&dfsEdges,int s,bool chkTr=true) {
    dfs_num[s]=predfn;
    inStk[s]=true;
    totV++;
    if(s==0) grp[s]=nG++;
    else grp[s]=-1;
    int i,v;
    for(i=0;i<dfsEdges[s].size();i++) {
        v=dfsEdges[s][i].v;
        if(chkTr&&!dfsEdges[s][i].inTr) continue;
        if(inStk[v]) SetGrp(v,s,nG++);
        else if(dfs_num[v]!=predfn) {
            par[v]=s;
            DFS(dfsEdges,v,chkTr);
        }
    }
    if(grp[s]==-1) grp[s]=nG++;
    inStk[s]=false;
}

int DMST(vector< vector<Edge> >&dmstEdges) {
    int N=dmstEdges.size();
    vector<int>MU(N,-1);
    vector<int>MI(N);
    int u,v,i,MinCost=0;
    for(u=0;u<N;u++) {
        for(i=0;i<dmstEdges[u].size();i++) {
            v=dmstEdges[u][i].v;
            if(v==0) continue;
            if(MU[v]==-1||dmstEdges[MU[v]][MI[v]].c>dmstEdges[u][i].c) {
                if(MU[v]!=-1) {
                    MinCost-=dmstEdges[MU[v]][MI[v]].c;
                    dmstEdges[MU[v]][MI[v]].inTr=false;
                }
                MU[v]=u;
                MI[v]=i;
                MinCost+=dmstEdges[u][i].c;
                dmstEdges[u][i].inTr=true;
            }
        }
    }
    if(N==2) return dmstEdges[MU[1]][MI[1]].c;
    totV=0;
    nG=0;
    predfn++;
    DFS(dmstEdges,0);
    if(totV==N) return MinCost;
    for(i=0;i<N;i++) {
        if(dfs_num[i]==predfn) continue;
        DFS(dmstEdges,i);
    }
    vector< vector<Edge> > tmpE(nG);
    for(u=0;u<N;u++) {
        for(i=0;i<dmstEdges[u].size();i++) {
            int v=dmstEdges[u][i].v;
            if(v==0) continue;
            if(grp[u]==grp[v] ) continue;
            tmpE[grp[u]].PB(Edge(grp[v],dmstEdges[u][i].c-dmstEdges[MU[v]][MI[v]].c));
        }
    }
    dmstEdges.clear();
    return MinCost+DMST(tmpE);
}

bool Possible(int minB) {
    int i,idx;
    vector< vector<struct Edge> >edges(n);
    for(i=0;i<n;i++) {
        inStk[i]=false;
        dfs_num[i]=grp[i]=par[i]=0;
    }
    for(i=0;i<m;i++)
        if(allEdges[i].b>=minB)
            edges[allEdges[i].u].PB(Edge(allEdges[i].v,allEdges[i].c));
    nG=totV=0;
    predfn=1;
    DFS(edges,0,false);
    if(totV!=n) return false;
    return DMST(edges)<=maxC;
}

void Solve() {
    int i,lo,hi,mid,maxB=0;
    scanf("%d %d %d",&n,&m,&maxC);
    for(i=0;i<m;i++) {
        scanf("%d %d %d %d",&allEdges[i].u,&allEdges[i].v,&allEdges[i].b,&allEdges[i].c);
        maxB=max(maxB,allEdges[i].b);
    }
    lo=0;
    hi=maxB;
    while(lo<hi) {
        mid=(lo+hi+1)/2;
        if(Possible(mid)) lo=mid;
        else hi=mid-1;
    }
    if(lo==0) printf("streaming not possible.\n");
    else printf("%d kbps\n",lo);
}

int main() {
    FRI;
    int t;
    scanf("%d",&t);
    while(t--) Solve();
    return 0;
}

