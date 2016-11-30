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
#define MAXN 100005
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

class Node {
public:
    Node *leftChild,*rightChild;
    bool lazy;
    int total,range;
    Node(int l,int r) {
        lazy=false;
        total=0;
        range=r-l+1;
        leftChild=nullptr;
        rightChild=nullptr;
    }
    void get(Node *nd) {
        leftChild=nd->leftChild;
        rightChild=nd->rightChild;
        lazy=nd->lazy;
        total=nd->total;
        range=nd->range;
    }
};

Node *state[MAXN],*root;

Node* Propagate(Node *nd,int l,int r,bool val) {
    Node *newNd=new Node(l,r);
    if(nd!=nullptr) newNd->get(nd);

    if(val) newNd->lazy=!(newNd->lazy);
    return newNd;
}

Node* UpdateSegmentPoint(Node *nd,int l,int r,int pos,bool place) {
    Node *newNd=new Node(l,r);
    if(nd!=nullptr) newNd->get(nd);

    if(l==r) {
        if(place) newNd->total=1;
        else newNd->total=0;
        newNd->lazy=false;
        return newNd;
    }

    int mid=(l+r)>>1;

    newNd->leftChild=Propagate(newNd->leftChild,l,mid,newNd->lazy);
    newNd->rightChild=Propagate(newNd->rightChild,mid+1,r,newNd->lazy);
    newNd->lazy=false;

    if(pos<=mid) newNd->leftChild=UpdateSegmentPoint(newNd->leftChild,l,mid,pos,place);
    else newNd->rightChild=UpdateSegmentPoint(newNd->rightChild,mid+1,r,pos,place);

    newNd->total=newNd->leftChild->lazy?mid-l+1-newNd->leftChild->total:newNd->leftChild->total;
    newNd->total+=newNd->rightChild->lazy?r-mid-newNd->rightChild->total:newNd->rightChild->total;

    return newNd;
}

Node* UpdateSegmentRange(Node *nd,int l,int r,int i,int j) {
    int mid=(l+r)>>1;
    Node *newNd=new Node(l,r);
    if(nd!=nullptr) newNd->get(nd);

    if(i<=l&&r<=j) {
        newNd->lazy=!(newNd->lazy);
        return newNd;
    }

    newNd->leftChild=Propagate(newNd->leftChild,l,mid,newNd->lazy);
    newNd->rightChild=Propagate(newNd->rightChild,mid+1,r,newNd->lazy);
    newNd->lazy=false;

    if(i<=mid) newNd->leftChild=UpdateSegmentRange(newNd->leftChild,l,mid,i,j);
    if(j>mid) newNd->rightChild=UpdateSegmentRange(newNd->rightChild,mid+1,r,i,j);

    newNd->total=newNd->leftChild->lazy?mid-l+1-newNd->leftChild->total:newNd->leftChild->total;
    newNd->total+=newNd->rightChild->lazy?r-mid-newNd->rightChild->total:newNd->rightChild->total;
    return newNd;
}

int main() {
    int n,m,q,i,t,r,c,p;
    scanf("%d %d %d",&n,&m,&q);
    root=state[0]=new Node(0,n*m-1);
    for(i=1;i<=q;i++) {
        state[i]=nullptr;
        scanf("%d",&t);
        switch(t) {
        case 1:
            scanf("%d %d",&r,&c);
            r--,c--;
            root=state[i]=UpdateSegmentPoint(root,0,n*m-1,r*m+c,true);
            break;
        case 2:
            scanf("%d %d",&r,&c);
            r--,c--;
            root=state[i]=UpdateSegmentPoint(root,0,n*m-1,r*m+c,false);
            break;
        case 3:
            scanf("%d",&r);
            r--;
            root=state[i]=UpdateSegmentRange(root,0,n*m-1,r*m,(r+1)*m-1);
            break;
        case 4:
            scanf("%d",&p);
            root=state[i]=state[p];
        }
        printf("%d\n",root->lazy?n*m-root->total:root->total);
    }
    return 0;
}
