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
#define MAXN 510
using namespace std;

const double PI=acos(-1.0);
/*
 * Algorithm : Simplex ( Linear Programming )
 * Author : Simon Lo
 * Note: Simplex algorithm on augmented matrix a of dimension (m+1)x(n+1)
 * returns 1 if feasible, 0 if not feasible, -1 if unbounded
 * returns solution in b[] in original var order, max(f) in ret
 * form: maximize sum_j(a_mj*x_j)-a_mn s.t. sum_j(a_ij*x_j)<=a_in
 * in standard form.
 * To convert into standard form:
 * 1. if exists equality constraint, then replace by both >= and <=
 * 2. if variable x doesn't have nonnegativity constraint, then replace by
 * difference of 2 variables like x1-x2, where x1>=0, x2>=0
 * 3. for a>=b constraints, convert to -a<=-b
 * note: watch out for -0.0 in the solution, algorithm may cycle
 * EPS = 1e-7 may give wrong answer, 1e-10 is better
 */

double Aug[MAXN][MAXN];
double dummy[MAXN];

void Pivot(long m,long n,double A[MAXN][MAXN],long *B,long *N,long r,long c) {
    long i,j;
    swap(N[c],B[r]);
    A[r][c]=1/A[r][c];
    for(j=0;j<=n;j++) if(j!=c) A[r][j]*=A[r][c];
    for(i=0;i<=m;i++) {
        if(i!=r) {
            for(j=0;j<=n;j++) if(j!=c) A[i][j]-=A[i][c]*A[r][j];
            A[i][c]=-A[i][c]*A[r][c];
        }
    }
}

long Feasible( long m,long n,double A[MAXN][MAXN],long *B,long *N ) {
    long r,c,i;
    double p,v;
    while(true) {
        for(p=INF,i=0;i<m;i++) if(A[i][n]<p) p=A[r=i][n];
        if(p>-EPS) return 1;
        for(p=0,i=0;i<n;i++) if(A[r][i]<p) p=A[r][c=i];
        if(p>-EPS) return 0;
        p=A[r][n]/A[r][c];
        for(i=r+1;i<m;i++) {
            if(A[i][c]>EPS) {
                v=A[i][n]/A[i][c];
                if(v<p) r=i,p=v;
            }
        }
        Pivot(m,n,A,B,N,r,c);
    }
}

long Simplex(long m,long n,double A[MAXN][MAXN],double *b,double &Ret) {
    long B[MAXN+7],N[MAXN+7],r,c,i;
    double p,v;
    for(i=0;i<n;i++) N[i]=i;
    for(i=0;i<m;i++) B[i]=n+i;
    if(!Feasible(m,n,A,B,N)) return 0;
    while(true) {
        for(p=0,i=0;i<n;i++) if(A[m][i]>p) p=A[m][c=i];
        if(p<EPS) {
            for(i=0;i<n;i++) if(N[i]<n) b[N[i]]=0;
            for(i=0;i<m;i++) if(B[i]<n) b[B[i]]=A[i][n];
            Ret=-A[m][n];
            return 1;
        }
        for(p=INF,i=0;i<m;i++) {
            if(A[i][c]>EPS) {
                v=A[i][n]/A[i][c];
                if(v<p) p=v,r=i;
            }
        }
        if(p==INF) return -1;
        Pivot(m,n,A,B,N,r,c);
    }
}

long long dist[MAXN][MAXN];
vector<pair<int,long long> >edge[MAXN];

void BFS(int s) {
    int c,cc,i;
    pair<int,int> t;
    queue<pair<int,int> >q;
    dist[s][s]=0;
    q.push(MP(s,0));
    while(!q.empty()) {
        t=q.front();
        c=t.F;
        for(i=0;i<edge[c].size();i++) {
            cc=edge[c][i].F;
            if(dist[s][cc]==-1) {
                dist[s][cc]=dist[s][c]+edge[c][i].S;
                q.push(edge[c][i]);
            }
        }
        q.pop();
    }
}

int main() {
    int n,i,b,j,u,v,w,result;
    long long x;
    double ans;
    cin>>n>>x;
    for(i=0;i<n;i++) {
        scanf("%d",&b);
        if(b==1) {
            Aug[n][n]++;
            Aug[n+1][n]--;
        }
        Aug[n+2][i]=b-1;
    }
    for(i=0;i<n;i++) for(j=0;j<n;j++) dist[i][j]=-1;
    for(i=0;i<n-1;i++) {
        scanf("%d %d %d",&u,&v,&w);
        u--;
        v--;
        edge[u].PB(MP(v,w));
        edge[v].PB(MP(u,w));
    }
    for(i=0;i<n;i++) BFS(i);
    for(i=0;i<n;i++) {
        for(j=0;j<n;j++) {
            if(dist[i][j]<=x) Aug[i][j]=-1;
            else Aug[i][j]=0;
        }
        Aug[i][j]=-1;
        Aug[n][i]=1;
        Aug[n+1][i]=-1;
    }
    Aug[n+2][n]=0;
    result=Simplex(n+2,n,Aug,dummy,ans);
    if(result==0) cout<<-1<<endl;
    else {
        ans*=-1;
        if(ans-(int)ans>0.5) cout<<1+(int)ans<<endl;
        else cout<<(int)ans<<endl;
    }
    return 0;
}
