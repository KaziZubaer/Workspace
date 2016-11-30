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
typedef vector<double> VD;

const double PI=acos(-1.0);

struct debugger{
    template<typename T> debugger& operator , (const T& v){
        cerr<<v<<" ";
        return *this;
    }
}dbg;

vector<VD> A;

/* return 1 if unique sol ,0 if no sol , INF if multiple sol */
long Gauss(vector< double> &Ans) {
    long Row=(long)A.size();
    long Col=(long)A[0].size()-1;
    vector<long>Where(Col,-1);
    long r,c,i,j;
    for(c=0,r=0;c<Col&&r<Row;c++) {
        long sel=r;
        for(i=r;i<Row;i++)
            if(fabs(A[i][c])>fabs(A[sel][c]))
                sel=i;
        if(fabs(A[sel][c])<EPS) continue;
        swap(A[sel],A[r]);
        Where[c]=r;
        for(i=0;i<Row;i++) {
            if(i==r) continue;
            double CON=A[i][c]/A[r][c];
            for(j=c;j<=Col;j++) A[i][j]-=A[r][j]*CON;
        }
        r++;
    }
    Ans.assign(Col,0);
    for(i=0;i<Col;i++) {
        if(Where[i]==-1) continue;
        Ans[i]=A[Where[i]][Col]/A[Where[i]][i];
    }
    for(i=0;i<Row;i++) {
        double Where=0;
        for(j=0;j<Col;j++) Where+=Ans[j]*A[i][j];
        /* Found a row which hs all zero co-eff */
        if(fabs(Where-A[i][Col])>EPS) return 0;
    }
    for(i=0;i<Col;i++)
        if(Where[i]==-1)
            return INF ;
    return 1 ;
}

int main() {
    return 0;
}
