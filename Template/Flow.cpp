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
#define INF 1000000000
using namespace std;

const double PI=acos(-1.0);

struct debugger{
    template<typename T> debugger& operator , (const T& v){
        cerr<<v<<" ";
        return *this;
    }
}dbg;

const int MAX_E=200;
const int MAX_V=500;
int ver[MAX_E],cap[MAX_E],nx[MAX_E],last[MAX_V],ds[MAX_V],st[MAX_V],now[MAX_V],edge_count,S,T;

inline void reset() {
    memset(nx,-1,sizeof(nx));
    memset(last,-1,sizeof(last));
    edge_count=0;
}

inline void addedge(const int v,const int w,const int capacity,const int reverse_capacity) {
    ver[edge_count]=w; cap[edge_count]=capacity; nx[edge_count]=last[v]; last[v]=edge_count++;
    ver[edge_count]=v; cap[edge_count]=reverse_capacity; nx[edge_count]=last[w]; last[w]=edge_count++;
}

inline bool bfs() {
    memset(ds,-1,sizeof(ds));
    int a,b;
    a=b=0;
    st[0]=T;
    ds[T]=0;
    while(a<=b) {
        int v=st[a++];
        for(int w=last[v];w>=0;w=nx[w]) {
            if(cap[w^1]>0 && ds[ver[w]]==-1) {
                st[++b]=ver[w];
                ds[ver[w]]=ds[v]+1;
            }
        }
    }
    return ds[S]>=0;
}

int dfs(int v,int cur) {
    if(v==T) return cur;
    for(int &w=now[v];w>=0;w=nx[w]) {
        if(cap[w]>0 && ds[ver[w]]==ds[v]-1) {
            int d=dfs(ver[w],min(cur,cap[w]));
            if(d) {
                cap[w]-=d;
                cap[w^1]+=d;
                return d;
            }
        }
    }
    return 0;
}

inline int flow() {
    int res=0;
    while(bfs()) {
        for(int i=0;i<=T;i++) now[i]=last[i];
        while(1) {
            int tf=dfs(S,INF);
            res+=tf;
            if(!tf) break;
        }
    }
    return res;
}

int main() {
    FRI;
    int t,n,m,i,j,isOn[15][15],req;
    scanf("%d",&t);
    while(t--) {
        scanf("%d %d",&n,&m);
        cout<<n<<" "<<m<<endl;
        for(i=0;i<n;i++)
            for(j=0;j<m;j++)
                scanf("%1d",&isOn[i][j]);
        reset();
        for(i=0;i<n*m;i++)
            if(isOn[i/m][i%m]) {
                addedge(n*m+n+m,i,2,0);
                addedge(i,n*m+i/m,1,0);
                addedge(i,n*m+n+i%m,1,0);
            }
        req=0;
        for(i=0;i<n;i++)
            for(j=0;j<m;j++)
                if(isOn[i][j]) {
                    addedge(n*m+i,n*m+n+m+1,1,0);
                    addedge(n*m+n+j,n*m+n+m+1,1,0);
                    req+=2;
                }
        cout<<req<<endl;
        S=n*m+n+m;
        T=S+1;
        if(flow()<req) {
            printf("impossible\n");
            printf("----------\n");
            continue;
        }
        else {
            printf("possible\n");
            printf("----------\n");
            continue;
        }
    }
    return 0;
}
