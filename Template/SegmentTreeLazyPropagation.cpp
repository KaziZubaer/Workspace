#include<bits/stdc++.h>

#define PB push_back
#define MP make_pair
#define F first
#define S second

#define FRI freopen("in.txt","r",stdin)
#define FRO freopen("out.txt","w",stdout)
#define debug(args...) {dbg,args; cerr<<endl;}
#define RAD(x) ((x*PI)/180)

#define EPS 1e-12
#define INF 1000000007
#define MOD 1000000007
#define MAXN 100005
using namespace std;

const double PI=acos(-1.0);

struct debugger{
    template<typename T> debugger& operator , (const T& v){
        cerr<<v<<" ";
        return *this;
    }
}dbg;

int tree[3*MAXN][3],lazy[3*MAXN];

void BuildSegmentTree(int node,int l,int r) {
    if(l==r) {
        tree[node][0]=1;
        tree[node][1]=tree[node][2]=0;
    }
    else {
        int mid=(l+r)/2,it;
        BuildSegmentTree(node<<1,l,mid);
        BuildSegmentTree((node<<1)+1,mid+1,r);
        for(it=0;it<3;it++) tree[node][it]=tree[node<<1][it]+tree[(node<<1)+1][it];
    }
}

void UpdateSegmentRange(int node,int l,int r,int i,int j,int lazyParent) {
    lazyParent+=lazy[node];
    lazy[node]=0;
    if(i<=l&&r<=j) {
        lazyParent++;
        while(lazyParent>=3) lazyParent-=3;
        int temp=tree[node][0],mate=3-lazyParent;
        tree[node][0]=tree[node][mate];
        tree[node][mate]=tree[node][3-mate];
        tree[node][3-mate]=temp;
        if(l!=r) {
            lazy[node<<1]+=lazyParent;
            while(lazy[node<<1]>=3) lazy[node<<1]-=3;
            lazy[(node<<1)+1]+=lazyParent;
            while(lazy[(node<<1)+1]>=3) lazy[(node<<1)+1]-=3;
        }
    }
    else {
        int mid=(l+r)/2,it;
        if(i<=mid) {
            UpdateSegmentRange(node<<1,l,mid,i,j,lazyParent);
        }
        else {
            UpdateSegmentRange(node<<1,l,mid,l,mid,lazyParent-1);
        }
        if(j>mid) {
            UpdateSegmentRange((node<<1)+1,mid+1,r,i,j,lazyParent);
        }
        else {
            UpdateSegmentRange((node<<1)+1,mid+1,r,mid+1,r,lazyParent-1);
        }
        for(it=0;it<3;it++) tree[node][it]=tree[node<<1][it]+tree[(node<<1)+1][it];
    }
}

int SearchSegmentTree(int node,int l,int r,int i,int j,int lazyParent) {
    lazyParent+=lazy[node];
    while(lazyParent>=3) lazyParent-=3;
    if(i<=l&&r<=j) {
        switch(lazyParent) {
        case 0: return tree[node][0];
        case 1: return tree[node][2];
        case 2: return tree[node][1];
        }
    }
    int mid=(l+r)/2,ret=0;
    if(i<=mid) {
        ret=SearchSegmentTree(node<<1,l,mid,i,j,lazyParent);
    }
    if(j>mid) {
        ret+=SearchSegmentTree((node<<1)+1,mid+1,r,i,j,lazyParent);
    }
    return ret;
}

int main() {
    int n,q,i,op,a,b;
    scanf("%d %d",&n,&q);
    BuildSegmentTree(1,0,n-1);
    for(i=0;i<q;i++) {
        scanf("%d %d %d",&op,&a,&b);
        if(op==0) UpdateSegmentRange(1,0,n-1,a,b,0);
        else printf("%d\n",SearchSegmentTree(1,0,n-1,a,b,0));
    }
    return 0;
}
