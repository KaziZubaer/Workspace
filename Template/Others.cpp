#include<bits/stdc++.h>
using namespace std;

int compress(vector<int>rawData,vector<int>&theArray) {
    map<int,int>compressed;
    sort(rawData.begin(),rawData.end());
    compressed.clear();
    int idx=0,i,sz=rawData.size();
    for(i=0;i<sz;i++)
        if(rawData[i]==0 || rawData[i]!=rawData[i-1])
            compressed[rawData[i]]=++idx;
    for(i=0;i<sz;i++)
        theArray[i]=compressed[theArray[i]];
    return idx;
}

bool operator<(const PQNode &a)const { //Defined in reversed order for priority queue
    return this->cost>a.cost;
}

int prime[3405];
bitset<15805> composite;

void MakePrimeList(){ //Lists first 3401 primes. The largest prime is 31607
    int k,len=1;
    composite[1]=1;
    prime[0]=2;
    for(int i=2;i<15805;i++)
        if(composite[i]==0){
            prime[len]=2*i-1;
            len++;
            for(int j=((2*i-1)*(2*i-1))/2+1;j<15805;j+=2*i-1)
                composite[j]=1;
        }
}

int phi(int n) {
    if(n<2) {
        return 0;
    }
    if(n==2) {
        return 1;
    }
    if(!(n&1)) {
        int m=n>>1;
        return !(m&1)?phi(m)<<1:phi(m);
    }
    for(int i=1;i<1230;i++) {
        int m=prime[i];
        if(m*m>n) {
            break;
        }
        if(n%m) {
            continue;
        }
        int o=n/m;
        int d=GCD(m,o);
        return d==1?phi(m)*phi(o):phi(m)*phi(o)*d/phi(d);
    }
    return n-1;
}

void MatrixMultiplication(int a[MatDimension][MatDimension], //Multiplies a and b. stores result in c
                          int b[MatDimension][MatDimension],
                          int c[MatDimension][MatDimension]){
    int i,j,k;
    for(i=0;i<MatDimension;i++)
        for(j=0;j<MatDimension;j++){
            c[i][j]=0;
            for(k=0;k<MatDimension;k++)
                c[i][j]+=a[i][k]*b[k][j];
        }
}

void PreProcessRMQ(int M[MAXN][LOGMAXN],int A[MAXN],int N){ //O(NlogN)
    int i,j;
    for(i=0;i<N;i++)
        M[i][0]=i;
    for(j=1;1<<j<=N;j++)
        for(i=0;i+(1<<j)-1<N;i++){
            if(A[M[i][j-1]]<A[M[i+(1<<(j-1))][j-1]])
                M[i][j]=M[i][j-1];
            else
                M[i][j]=M[i+(1<<(j-1))][j-1];
        }
}

int IndexOfRMQ(int M[MAXN][LOGMAXN],int A[MAXN],int i,int j){ //O(1)
    int k=log2(j-i+1);
    return A[M[i][k]]<A[M[j-1<<k+1][k]]?M[i][k]:M[j-1<<k+1][k];
}

int P[1005][20],T[1005],N,L[1005];
vector<int>adj[1005];

void PreProcessLCA(int root){ //O(NlogN)
    queue<int>Q;
    int t,i,c,j;
    for(i=0;i<N;i++)
        L[i]=-1;
    L[root]=0;
    Q.push(root);
    while(!Q.empty()) {
        t=Q.front();
        Q.pop();
        for(i=0;i<adj[t].size();i++) {
            c=adj[t][i];
            if(L[c]==-1) {
                L[c]=L[t]+1;
                Q.push(c);
            }
        }
    }
    for(i=0;i<N;i++)
        for(j=0;1<<j<N;j++)
            P[i][j]=-1;
    for(i=0;i<N;i++)
        P[i][0]=T[i];
    for(j=1;1<<j<N;j++)
        for(i=0;i<N;i++)
            if(P[i][j-1]!=-1)
                P[i][j]=P[P[i][j-1]][j-1];
}

int SearchLCA(int p,int q){ //O(logN)
    int tmp,log,i;
    if(L[p]<L[q]){
        tmp=p;
        p=q;
        q=tmp;
    }
    log=log2(L[p]);
    for(i=log;i>=0;i--)
        if(L[p]-(1<<i)>=L[q])
            p=P[p][i];
    if(p==q)
        return p;
    for(i=log;i>=0;i--)
        if(P[p][i]!=-1&&P[p][i]!=P[q][i]){
            p=P[p][i];
            q=P[q][i];
        }
    return T[p];
}

int BigMod(int b,int po) {
    if(po==0) return 1;
    long long r=BigMod(b,po/2);
    r*=r;
    r%=MOD;
    if(po&1) r*=b;
    return r%MOD;
}

int invMod(int b) {
    return BigMod(b,MOD-2);
}

int NCR(int nn,int rr) {
    long long ans=fact[nn];
    ans*=invFact[rr];
    ans%=MOD;
    return (ans*invFact[nn-rr])%MOD;
}

/*************DJKSTRA*****************/
class PQNode {
public:
    int id,cost;
    PQNode() {}
    PQNode(int id,int cost) : id(id), cost(cost) {}
    bool operator<(const PQNode &a)const {
        return this->cost>a.cost;
    }
};

int src[2],dist[2][MAXN],parent[2][MAXN];
vector< pair<int,int> >adj[2][MAXN];

void DJKSTRA(int idx) {
    int c,i,temp;
    priority_queue<PQNode>q;
    PQNode top;
    dist[idx][src[idx]]=0;
    parent[idx][src[idx]]=-1;
    q.push(PQNode(src[idx],0));
    while(!q.empty()) {
        top=q.top();
        for(i=0;i<adj[0][top.id].size();i++) {
            c=adj[0][top.id][i].F;
            temp=top.cost+adj[0][top.id][i].S;
            if(dist[idx][c]>temp) {
                dist[idx][c]=temp;
                parent[idx][c]=top.id;
                q.push(PQNode(c,temp));
            }
        }
        q.pop();
    }
}

/****************Grid*******************/
int R,C,dir[2][4]={{1,-1,0,0},{0,0,1,-1}};
bool grid[MAXN][MAXN];

void Move(pair<int,int>&pos,int d) {
    pos.F+=dir[0][d];
    pos.S+=dir[1][d];
}

bool OK(pair<int,int>pos,int d) {
    Move(pos,d);
    return  pos.F>=0&&pos.F<R
            &&pos.S>=0&&pos.S<C
            &&grid[pos.F][pos.S];
}


/******broken game grundy***************/
int g[MAXN];

int grundy(int n){
    if(n<=0) return 0;
    if(g[n]!=-1) return g[n];
    set <int> vis;
    for(int i=1;i<=n;i++){
        vis.insert(grundy(i-2)^grundy(n-i-1));
    }
    g[n]=0;
    while(vis.find(g[n])!=vis.end()){
        g[n]++;
    }
    return g[n];
}

int main() {
    int i;
    memset(g,-1,sizeof(g));
    for(i=0;i<1000;i++) {
        if(!grundy(i)) cout<<i<<endl;
    }
    return 0;
}
