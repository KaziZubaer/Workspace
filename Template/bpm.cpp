#include<bits/stdc++.h>
#define PB push_back
#define MP make_pair
#define F first
#define S second
#define FRI freopen("in.txt","r",stdin)
#define FRO freopen("out.txt","w",stdout)
#define debug(args...) {dbg,args; cerr<<endl;}
#define EPS 1e-12
#define RAD(x) ((x*PI)/180)
#define MAXN 100
#define INF 7777777
using namespace std;

vector<long> Edge[MAXN+7];
int L,R,n,m;	// L->left vertex ; R->Right vertex ; n->Tot vertex(L+R)
long D[MAXN+7];	// D->Shortest distance
long M[MAXN+7];
/* Node 0 is used as dummy here */

bool BFS() {
    queue<long> q;
	long i;
	for(i=1;i<=L;i++) {
		if(M[i]) D[i]=INF;
		else {
			D[i]=0;
			q.push(i);
		}
	}
	D[0]=INF;
	while(!q.empty()){
		long u=q.front();
		q.pop();
		for(i=0;i<Edge[u].size();i++){
			long v=Edge[u][i];
			if(D[M[v]]==INF) {
				D[M[v]]=D[u]+1;
				q.push(M[v]);
			}
		}
	}
	return D[0]!=INF;
}

bool DFS(long u) {
	if(!u) return true;
	long i;
	for(i=0;i<Edge[u].size();i++) {
		long v=Edge[u][i];
		if(D[M[v]]==D[u]+1 && DFS(M[v])){
			M[u]=v;
			M[v]=u;
			return true;
		}
	}
	D[u]=INF;
	return false;
}

int main() {
    int T,t=0,h[MAXN],a[MAXN],d[MAXN],i,j,ans;
    scanf("%d",&T);
    while(t++<T) {
        scanf("%d %d",&L,&R);
        n=L+R;
        Edge[0].clear();
        M[0]=D[0]=0;
        for(i=1;i<=L;i++) {
            scanf("%d %d %d",&h[i],&a[i],&d[i]);
            Edge[i].clear();
            M[i]=D[i]=0;
        }
        for(;i<=n;i++) {
            scanf("%d %d %d",&h[i],&a[i],&d[i]);
            Edge[i].clear();
            M[i]=D[i]=0;
        }
        for(i=1;i<=L;i++)
            for(j=1;j<=R;j++)
                if(abs(h[i]-h[L+j])<=12&&abs(a[i]-a[L+j])<=5&&d[i]==d[L+j]) {
                    Edge[i].PB(L+j);
                    Edge[L+j].PB(i);
                }
        ans=0;
        while(BFS())
            for(i=1;i<=L;i++) {
                if(M[i]) continue;
                if(DFS(i)) ans++;
            }
        printf("Case %d: %d\n",t,ans);
    }
    return 0;
}

