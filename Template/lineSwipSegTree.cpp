#include<stdio.h>
#include<vector>
#include<algorithm>

#define PB push_back
#define MP make_pair
#define F first
#define S second

#define FRI freopen("in.txt","r",stdin)
#define FRO freopen("out.txt","w",stdout)
#define RAD(x) ((x*PI)/180)

#define EPS 1e-12
#define BIAS 10002
#define MAXN 20010
#define INF 1e9
using namespace std;

int tree[3*MAXN];

void InitSegmentTree() {
    int i;
    for(i=0;i<3*MAXN;i++) tree[i]=0;
}

void UpdateSegmentRange(int node,int l,int r,int i,int j,int val) {
    if(i<=l&&r<=j&&tree[node]!=-1) {
        tree[node]+=val;
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
        return tree[node]>0?r-l+1:0;
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

vector< pair< pair<int,int>,pair<int,int> > >xswip,yswip;

int main() {
    int n,i,x[2],y[2],j,ans=0,max_x=0,max_y=0,val;
    scanf("%d",&n);
    for(i=0;i<n;i++) {
        scanf("%d %d %d %d",&x[0],&y[0],&x[1],&y[1]);
        for(j=0;j<2;j++) {
            x[j]+=BIAS;
            y[j]+=BIAS;
            max_x=max(max_x,x[j]);
            max_y=max(max_y,y[j]);
        }
        xswip.PB(MP(MP(x[0],-1),MP(y[0],y[1]-1)));
        xswip.PB(MP(MP(x[1],0),MP(y[0],y[1]-1)));
        yswip.PB(MP(MP(y[0],-1),MP(x[0],x[1]-1)));
        yswip.PB(MP(MP(y[1],0),MP(x[0],x[1]-1)));
    }
    sort(xswip.begin(),xswip.end());
    sort(yswip.begin(),yswip.end());
    n=xswip.size();
    for(i=0;i<n;i++) {
        if(xswip[i].F.S==-1) {
            val=SearchSegmentTree(1,0,max_y,xswip[i].S.F,xswip[i].S.S);
            ans+=xswip[i].S.S-xswip[i].S.F+1-val;
            UpdateSegmentRange(1,0,max_y,xswip[i].S.F,xswip[i].S.S,1);
        }
        else {
            UpdateSegmentRange(1,0,max_y,xswip[i].S.F,xswip[i].S.S,-1);
            val=SearchSegmentTree(1,0,max_y,xswip[i].S.F,xswip[i].S.S);
            ans+=xswip[i].S.S-xswip[i].S.F+1-val;
        }
    }
    InitSegmentTree();
    for(i=0;i<n;i++) {
        if(yswip[i].F.S==-1) {
            val=SearchSegmentTree(1,0,max_x,yswip[i].S.F,yswip[i].S.S);
            ans+=yswip[i].S.S-yswip[i].S.F+1-val;
            UpdateSegmentRange(1,0,max_x,yswip[i].S.F,yswip[i].S.S,1);
        }
        else {
            UpdateSegmentRange(1,0,max_x,yswip[i].S.F,yswip[i].S.S,-1);
            val=SearchSegmentTree(1,0,max_x,yswip[i].S.F,yswip[i].S.S);
            ans+=yswip[i].S.S-yswip[i].S.F+1-val;
        }
    }
    printf("%d\n",ans);
    return 0;
}
