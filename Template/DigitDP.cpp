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

const double PI=acos(-1.0);

struct debugger{
    template<typename T> debugger& operator , (const T& v){
        cerr<<v<<" ";
        return *this;
    }
}dbg;

vector<int>digits;
long long dp[15][MAXN][2],theNumber;
int divisor;

void DP(int pos,int mod,bool smallTaken) {
    int i;
    if(pos==digits.size()) return;
    if(smallTaken) {
        for(i=0;i<10;i++)
            dp[pos][(mod*10+i)%divisor][1]+=dp[pos-1][mod][1];
    }
    else {
        for(i=0;i<digits[pos];i++)
            dp[pos][(mod*10+i)%divisor][1]+=dp[pos-1][mod][0];
        dp[pos][(mod*10+i)%divisor][0]+=dp[pos-1][mod][0];
    }
}

int main() {
    long long x;
    int i,pos;
    cin>>theNumber>>divisor;
    x=theNumber;
    while(x) {
        digits.PB(x%10);
        x/=10;
    }
    reverse(digits.begin(),digits.end());
    for(i=0;i<digits[0];i++) dp[0][i%divisor][1]=1;
    dp[0][i%divisor][0]=1;
    for(pos=1;pos<digits.size();pos++)
        for(i=0;i<divisor;i++) {
            DP(pos,i,false);
            DP(pos,i,true);
        }
    cout<<"Correct "<<theNumber/divisor<<endl;
    cout<<"Assumed "<<dp[digits.size()-1][0][0]+dp[digits.size()-1][0][1]<<endl;
    return 0;
}
