#include<stdio.h>
#include<algorithm>

#define INF 1000000007
#define MOD 1000000007
#define MAXN 50005

using namespace std;

int A[MAXN],sum[MAXN],preTree[3*MAXN],sufTree[3*MAXN],ansTree[3*MAXN];
int tempPre[3*MAXN],tempSuf[3*MAXN];

int SumRange(int i,int j) {
    if(j<i) return 0;
    if(i==0) return sum[j];
    return sum[j]-sum[i-1];
}

void BuildSegmentTree(int node,int l,int r) {
    if(l==r) {
        ansTree[node]=preTree[node]=sufTree[node]=A[l];
    }
    else {
        int mid=(l+r)/2;
        BuildSegmentTree(node<<1,l,mid);
        BuildSegmentTree((node<<1)+1,mid+1,r);
        preTree[node]=max(preTree[node<<1],SumRange(l,mid)+preTree[(node<<1)+1]);
        sufTree[node]=max(sufTree[(node<<1)+1],SumRange(mid+1,r)+sufTree[node<<1]);
        ansTree[node]=max(ansTree[node<<1],ansTree[(node<<1)+1]);
        ansTree[node]=max(ansTree[node],sufTree[node<<1]+preTree[(node<<1)+1]);
    }
}

int SearchSegmentTreeAns(int node,int l,int r,int i,int j) {
    if(i<=l&&r<=j) {
        tempPre[node]=preTree[node];
        tempSuf[node]=sufTree[node];
        return ansTree[node];
    }
    int mid=(l+r)/2,ret=-INF;
    if(j<=mid) {
        ret=SearchSegmentTreeAns(node<<1,l,mid,i,j);
        tempPre[node]=tempPre[node<<1];
        tempSuf[node]=tempSuf[node<<1];
        return ret;
    }
    else if(i>mid) {
        ret=SearchSegmentTreeAns((node<<1)+1,mid+1,r,i,j);
        tempPre[node]=tempPre[(node<<1)+1];
        tempSuf[node]=tempSuf[(node<<1)+1];
        return ret;
    }
    else {
        ret=max(ret,SearchSegmentTreeAns(node<<1,l,mid,i,j));
        ret=max(ret,SearchSegmentTreeAns((node<<1)+1,mid+1,r,i,j));
        ret=max(ret,tempSuf[node<<1]+tempPre[(node<<1)+1]);
        tempPre[node]=max(SumRange(max(i,l),mid)+tempPre[(node<<1)+1],tempPre[node<<1]);
        tempSuf[node]=max(SumRange(mid+1,min(j,r))+tempSuf[node<<1],tempSuf[(node<<1)+1]);
        return ret;
    }
}

int main() {
    int n,i,x,y,m;
    scanf("%d",&n);
    scanf("%d",&A[0]);
    sum[0]=A[0];
    for(i=1;i<n;i++) {
        scanf("%d",&A[i]);
        sum[i]=sum[i-1]+A[i];
    }
    BuildSegmentTree(1,0,n-1);
    scanf("%d",&m);
    for(i=0;i<m;i++) {
        scanf("%d %d",&x,&y);
        printf("%d\n",SearchSegmentTreeAns(1,0,n-1,x-1,y-1));
    }
    return 0;
}
