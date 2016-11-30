#include<bits/stdc++.h>
#define MAXN 20005
#define INF 1e9
using namespace std;

int tree[3*MAXN];

void InitSegmentTree() {
    int i;
    for(i=0;i<3*MAXN;i++) tree[i]=0;
}

void UpdateSegmentRange(int node,int l,int r,int i,int j,int val) {
    if(i<=l&&r<=j) {
        tree[node]=val;
    }
    else {
        if(tree[node]!=-1) {
            tree[(node<<1)+1]=tree[node<<1]=tree[node];
        }
        int mid=(l+r)/2;
        if(i<=mid) {
            UpdateSegmentRange(node<<1,l,mid,i,j,val);
        }
        if(j>mid) {
            UpdateSegmentRange((node<<1)+1,mid+1,r,i,j,val);
        }
        if(tree[(node<<1)+1]!=tree[node<<1]) tree[node]=-1;
        else tree[node]=tree[node<<1];
    }
}

int SearchSegmentTree(int node,int l,int r,int i,int j) {
    if(i<=l&&r<=j&&tree[node]!=-1) {
        return tree[node]*(r-l+1);
    }
    if(tree[node]!=-1) {
        tree[(node<<1)+1]=tree[node<<1]=tree[node];
    }
    int mid=(l+r)/2,ret=0;
    if(i<=mid) {
        ret=SearchSegmentTree(node<<1,l,mid,i,j);
    }
    if(j>mid) {
        ret=ret+SearchSegmentTree((node<<1)+1,mid+1,r,i,j);
    }
    return ret;
}

int main() {
    int t,x,y,val;
    while(true) {
        cin>>t>>x>>y;
        if(t==0) {
            cin>>val;
            UpdateSegmentRange(1,0,5,x,y,val);
        }
        else {
            cout<<SearchSegmentTree(1,0,5,x,y)<<endl;
        }
    }
    return 0;
}
