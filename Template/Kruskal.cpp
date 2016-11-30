#include<bits/stdc++.h>
using namespace std;

class Edge {
public:
    int u,v,c;
    Edge(int _u,int _v,int _c) { u=_u;v=_v;c=_c; }
};

bool cmp(Edge a,Edge b) {
    return a.c<b.c;
}

int setID[105],setRank[105];

void makeSet(int x) {
    setID[x]=x;
    setRank[x]=0;
}

int getSetID(int x) {
    if(setID[x]==x) return x;
    setID[x]=getSetID(setID[x]);
    return setID[x];
}

void mergeSet(int x,int y) {
    x=getSetID(x);
    y=getSetID(y);
    if(setRank[x]>setRank[y]) setID[y]=x;
    else setID[x]=y;
    if(setRank[x]==setRank[y]) setRank[y]++;
}

//nodes are numbered from 0 to n
int kruskal(int n,vector<Edge> &edge) {
    int taken=0,ans=0,i,u,v;
    for(i=0;i<=n;i++) makeSet(i);
    sort(edge.begin(),edge.end(),cmp);
    i=0;
    while(taken!=n) {
        u=edge[i].u;
        v=edge[i].v;
        if(getSetID(u)==getSetID(v)) {
            i++;
            continue;
        }
        ans+=edge[i].c;
        mergeSet(u,v);
        taken++;
        i++;
    }
    return ans;
}

int main() {
    int T,t=0,n,u,v,c,minimum,maximum,i;
    vector<Edge>edge;
    scanf("%d",&T);
    while(t++<T) {
        edge.clear();
        scanf("%d",&n);
        while(true) {
            scanf("%d %d %d",&u,&v,&c);
            if(u==0&&v==0&&c==0) break;
            edge.push_back(Edge(u,v,c));
        }
        minimum=kruskal(n,edge);
        for(i=0;i<edge.size();i++) edge[i].c=edge[i].c*(-1);
        maximum=(-1)*kruskal(n,edge);
        if((minimum+maximum)&1) printf("Case %d: %d/2\n",t,minimum+maximum);
        else printf("Case %d: %d\n",t,(minimum+maximum)/2);
    }
    return 0;
}
