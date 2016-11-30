#include<bits/stdc++.h>
#define MAXN 250005
#define INF 1e9
using namespace std;

class Node {
public:
    int res[MAXN];
}

int A[MAXN];
Node tree[5*MAXN];

void BuildSegmentTree(int node,int l,int r) {
    if(l==r) {
        /****this line is operation dependant****/
        tree[node]=A[l];
        /****************************************/
    }
    else {
        int mid=(l+r)/2;
        BuildSegmentTree(node<<1,l,mid);
        BuildSegmentTree((node<<1)+1,mid+1,r);
        /****this line is operation dependant****/
        tree[node]=min(tree[node<<1],tree[(node<<1)+1]);
        /****************************************/
    }
}

void UpdateSegmentTree(int node,int l,int r,int t,int val) {
    if(l==r&&r==t) {
        /****this line is operation dependant****/
        tree[node]=val;
        return;
        /****************************************/
    }
    int mid=(l+r)/2;
    if(t<=mid) {
        UpdateSegmentTree(node<<1,l,mid,t,val);
    }
    else {
        UpdateSegmentTree((node<<1)+1,mid+1,r,t,val);
    }
    /****this line is operation dependant****/
        tree[node]=min(tree[node<<1],tree[(node<<1)+1]);
        /****************************************/
}

int SearchSegmentTree(int node,int l,int r,int i,int j) {
    if(i<=l&&r<=j) {
        /****this line is operation dependant****/
        return tree[node];
        /****************************************/
    }
    /****this line is operation dependant****/
    int mid=(l+r)/2,ret=INF;
    /****************************************/
    if(i<=mid) {
        ret=SearchSegmentTree(node<<1,l,mid,i,j);
    }
    if(j>mid) {
        /****this line is operation dependant****/
        ret=min(ret,SearchSegmentTree((node<<1)+1,mid+1,r,i,j));
        /****************************************/
    }
    return ret;
}

int main() {
    int t=0,T,n,q,i,l,r;
    scanf("%d",&T);
    while(t++<T) {
        scanf("%d %d",&n,&q);
        for(i=0;i<n;i++) {
            scanf("%d",&A[i]);
        }
        BuildSegmentTree(1,0,n-1);
        printf("Case %d:\n",t);
        for(i=0;i<q;i++) {
            scanf("%d %d",&l,&r);
            l--;r--;
            printf("%d\n",SearchSegmentTree(1,0,n-1,l,r));
        }
    }
    return 0;
}
