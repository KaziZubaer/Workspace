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

typedef long long LL;

const double PI=acos(-1.0);

struct debugger{
    template<typename T> debugger& operator , (const T& v){
        cerr<<v<<" ";
        return *this;
    }
}dbg;

class Matrix {
public:
    unsigned int a[2][2];
    Matrix() {
        a[0][0]=1;
        a[0][1]=0;
        a[1][0]=0;
        a[1][1]=1;
    }
    Matrix(unsigned int a00,unsigned int a01,unsigned int a10,unsigned int a11) {
        a[0][0]=a00;
        a[0][1]=a01;
        a[1][0]=a10;
        a[1][1]=a11;
    }
    Matrix(const Matrix &m) {
        a[0][0]=m.a[0][0];
        a[0][1]=m.a[0][1];
        a[1][0]=m.a[1][0];
        a[1][1]=m.a[1][1];
    }
    Matrix operator + (const Matrix &m) const {
        return Matrix(
            (a[0][0]+m.a[0][0])%MOD,
            (a[0][1]+m.a[0][1])%MOD,
            (a[1][0]+m.a[1][0])%MOD,
            (a[1][1]+m.a[1][1])%MOD
        );
    }
    Matrix operator * (const Matrix &m) const {
        unsigned long long
            a00[]={a[0][0],a[0][1]},
            a01[]={a[0][0],a[0][1]},
            a10[]={a[1][0],a[1][1]},
            a11[]={a[1][0],a[1][1]};
        a00[0]*=m.a[0][0];a00[1]*=m.a[1][0];
        a01[0]*=m.a[0][1];a01[1]*=m.a[1][1];
        a10[0]*=m.a[0][0];a10[1]*=m.a[1][0];
        a11[0]*=m.a[0][1];a11[1]*=m.a[1][1];
        a00[0]=(a00[0]+a00[1])%MOD;
        a01[0]=(a01[0]+a01[1])%MOD;
        a10[0]=(a10[0]+a10[1])%MOD;
        a11[0]=(a11[0]+a11[1])%MOD;
        return Matrix(a00[0],a01[0],a10[0],a11[0]);
    }
};

Matrix BigMod(Matrix b,int p) {
    if(p==0) return Matrix(1,0,0,1);
    Matrix ret=BigMod(b,p/2);
    ret=ret*ret;
    if(p&1) ret=ret*b;
    return ret;
}

Matrix A[MAXN],tree[8*MAXN],lazy[8*MAXN],I;

void BuildSegmentTree(int node,int l,int r) {
    if(l==r) tree[node]=A[l];
    else {
        int mid=(l+r)>>1;
        BuildSegmentTree(node<<1,l,mid);
        BuildSegmentTree((node<<1)+1,mid+1,r);
        tree[node]=tree[node<<1]+tree[(node<<1)+1];
    }
    lazy[node]=I;
}

void UpdateSegmentTree(int node,int l,int r,int i,int j,Matrix val) {
    if(i<=l&&r<=j) {
        lazy[node]=lazy[node]*val;
        return;
    }
    int mid=(l+r)>>1;
    lazy[node<<1]=lazy[node<<1]*lazy[node];
    lazy[(node<<1)+1]=lazy[(node<<1)+1]*lazy[node];
    if(i<=mid) UpdateSegmentTree(node<<1,l,mid,i,j,val);
    if(j>mid) UpdateSegmentTree((node<<1)+1,mid+1,r,i,j,val);
    tree[node]=tree[node<<1]*lazy[node<<1]+tree[(node<<1)+1]*lazy[(node<<1)+1];
    lazy[node]=I;
}

Matrix SearchSegmentTree(int node,int l,int r,int i,int j) {
    if(i<=l&&r<=j) return tree[node]*lazy[node];
    int mid=(l+r)>>1;
    Matrix ret(0,0,0,0);
    lazy[node<<1]=lazy[node<<1]*lazy[node];
    lazy[(node<<1)+1]=lazy[(node<<1)+1]*lazy[node];
    if(i<=mid) ret=SearchSegmentTree(node<<1,l,mid,i,j);
    if(j>mid) ret=ret+SearchSegmentTree((node<<1)+1,mid+1,r,i,j);
    lazy[node]=I;
    return ret;
}

int main() {
    int n,m,x,l,r,i,typ;
    Matrix val,Base=Matrix(1,1,1,0);
    scanf("%d %d",&n,&m);
    for(i=0;i<n;i++) {
        scanf("%d",&x);
        A[i]=BigMod(Base,x);
    }
    BuildSegmentTree(1,0,n-1);
    for(i=0;i<m;i++) {
        scanf("%d",&typ);
        if(typ==1) {
            scanf("%d %d %d",&l,&r,&x);
            l--,r--;
            val=BigMod(Base,x);
            UpdateSegmentTree(1,0,n-1,l,r,val);
        }
        else {
            scanf("%d %d",&l,&r);
            l--;r--;
            val=SearchSegmentTree(1,0,n-1,l,r);
            printf("%d\n",val.a[1][0]);
        }
    }
    return 0;
}
