#include<bits/stdc++.h>

#define PB push_back
#define MP make_pair
#define F first
#define S second

#define FRI freopen("in.txt","r",stdin)
#define FRO freopen("out.txt","w",stdout)
#define debug(args...) {dbg,args; cerr<<endl;}
#define DB(x) #x"=>",x
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

char facility[55],facilities[105][55],description[200005];

void sortem(int n) {
    char temp[55];
    bool flag=true;
    int i,j;
    for(i=0;i<n&&flag;i++) {
        flag=false;
        for(j=0;j<n-1;j++)
            if(strcmp(facilities[j],facilities[j+1])>0) {
                strcpy(temp,facilities[j]);
                strcpy(facilities[j],facilities[j+1]);
                strcpy(facilities[j+1],temp);
                flag=true;
            }
    }
}

int main() {
    FRI;
    char a,b;
    int n,i,j,k;
    scanf("%d",&n);
    gets(facility);
    for(i=0;i<n;i++) {
        gets(facility);
        strcpy(facilities[i],facility);
    }
    sortem(n);
    i=0;
    while(scanf("%c",&description[i])!=EOF) i++;
    description[i]=0;
    for(i=0;i<n;i++) {
        for(j=0;j<strlen(description);j++) {
            for(k=0;k<strlen(facilities[i])&&j+k<strlen((description));k++) {
                a=facilities[i][k];
                if(a>='A'&&a<='Z') a=a-'A'+'a';
                b=description[j+k];
                if(b>='A'&&b<='Z') b=b-'A'+'a';
                if(a!=b)
                    break;
            }
            if(k==strlen(facilities[i]))
                break;
        }
        if(j<strlen(description)) printf("%s\n",facilities[i]);
    }
    return 0;
}
