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
#define INF 1e18
#define MOD 1000000007
#define MAXN 100005
using namespace std;

typedef long long LL;
typedef unsigned long long ULL;
typedef long double LD;

vector<int>adj[MAXN];
bool vis[MAXN],alive[MAXN];
int sub[MAXN],bestChild[MAXN],rnk[MAXN];

void DFS(int s) {
    int i,c,maxi=0;
    vis[s]=true;
    sub[s]=1;
    bestChild[s]=-1;
    for(i=0;i<adj[s].size();i++) {
        c=adj[s][i];
        if(vis[c]||!alive[c]) continue;
        DFS(c);
        if(maxi<sub[c]) {
            maxi=sub[c];
            bestChild[s]=c;
        }
        sub[s]+=sub[c];
    }
}

int main() {
    int n,i,a,b,s,tS,cur;
    bool more;
    scanf("%d",&n);
    for(i=0;i<n-1;i++) {
        scanf("%d %d",&a,&b);
        a--,b--;
        adj[a].PB(b);
        adj[b].PB(a);
    }
    for(i=0;i<n;i++) alive[i]=true;
    cur=0;
    more=true;
    while(more) {
        more=false;
        for(i=0;i<n;i++) vis[i]=false;
        for(i=0;i<n;i++)
            if(alive[i]&&!vis[i]) {
                more=true;
                s=i;
                DFS(s);
                tS=sub[s];
                while(bestChild[s]!=-1&&sub[bestChild[s]]>=tS/2) s=bestChild[s];
                alive[s]=false;
                rnk[s]=cur;
            }
        cur++;
    }
    for(i=0;i<n;i++) printf("%c ",rnk[i]+'A');
    return 0;
}
