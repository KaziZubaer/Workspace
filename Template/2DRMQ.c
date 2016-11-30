#include<stdio.h>

#define F(x) ((x)>>10)
#define S(x) ((x)-(((x)>>10)<<10))
#define ELEMENT(A,x) (A)[F(x)][S(x)]

#define EPS 1e-12
#define INF 1000000007
#define MOD 1000000007
#define MAXN 1001
#define LOGMAX 10

int cumSum[MAXN][MAXN],n,m,A[MAXN][MAXN],log2Val[MAXN],pow2Val[LOGMAX],M[MAXN][LOGMAX][MAXN][LOGMAX],multi[MAXN];

void PreProcessRMQ2D() {
    int i,li,j,lj,lim1,lim2;
    for(i=0;i<n;i++) {
        for(j=0;j<m;j++) M[i][0][j][0]=(i<<10)+j;
        for(lj=1;(1<<lj)<=m;lj++) {
            lim1=m-(1<<lj)+1;
            for(j=0;j<lim1;j++) {
                M[i][0][j][lj]=(i<<10);
                if(ELEMENT(A,M[i][0][j][lj-1])>ELEMENT(A,M[i][0][j+(1<<(lj-1))][lj-1]))
                    M[i][0][j][lj]+=S(M[i][0][j][lj-1]);
                else
                    M[i][0][j][lj]+=S(M[i][0][j+(1<<(lj-1))][lj-1]);
            }
        }
    }
    for(lj=0;(1<<lj)<=m;lj++) {
        lim1=m-(1<<lj)+1;
        for(j=0;j<lim1;j++)
            for(li=1;(1<<li)<=n;li++) {
                lim2=n-(1<<li)+1;
                for(i=0;i<lim2;i++) {
                    if(ELEMENT(A,M[i][li-1][j][lj])>ELEMENT(A,M[i+(1<<(li-1))][li-1][j][lj])) M[i][li][j][lj]=M[i][li-1][j][lj];
                    else M[i][li][j][lj]=M[i+(1<<(li-1))][li-1][j][lj];
                }
            }
    }
}

int main() {
    int i,j,x,y,ans,q,temp,column,dim,maxi,tempp,k1,k2,maxim=0;
    scanf("%d %d",&n,&m);
    for(i=0;i<n;i++) for(j=0;j<m;j++) {
        scanf("%d",&A[i][j]);
        if(A[i][j]>maxim) maxim=A[i][j];
    }
    for(j=0;j<m;j++) {
        column=0;
        for(i=0;i<n;i++) {
            column+=A[i][j];
            cumSum[i][j]=(j>0?cumSum[i][j-1]:0)+column;
        }
    }
    PreProcessRMQ2D();
    pow2Val[0]=1;
    for(i=1;i<LOGMAX;i++) pow2Val[i]=pow2Val[i-1]<<1;
    j=0;
    for(i=1;i<MAXN;i++) {
        for(;j<LOGMAX;j++) {
            if(pow2Val[j]>i) break;
            log2Val[i]=j;
        }
        j--;
    }
    scanf("%d",&q);
    while(q--) {
        scanf("%d %d",&x,&y);
        k1=log2Val[x];
        k2=log2Val[y];
        dim=x*y;
        multi[0]=0;
        for(i=1;i<=maxim;i++) multi[i]=multi[i-1]+dim;
        ans=INF;
        for(i=0;i+x-1<n;i++)
            for(j=0;j+y-1<m;j++) {
                maxi=ELEMENT(A,M[i][k1][j][k2]);
                tempp=ELEMENT(A,M[i][k1][j+y-pow2Val[k2]][k2]);
                if(tempp>maxi) maxi=tempp;
                tempp=ELEMENT(A,M[i+x-pow2Val[k1]][k1][j][k2]);
                if(tempp>maxi) maxi=tempp;
                tempp=ELEMENT(A,M[i+x-pow2Val[k1]][k1][j+y-pow2Val[k2]][k2]);
                if(tempp>maxi) maxi=tempp;
                temp=multi[maxi];
                temp-=cumSum[i+x-1][j+y-1]-(j>0?cumSum[i+x-1][j-1]:0)-(i>0?cumSum[i-1][j+y-1]:0)+(i>0&&j>0?cumSum[i-1][j-1]:0);
                if(temp<ans) ans=temp;
            }
        printf("%d\n",ans);
    }
    return 0;
}
