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

#define PREC 100
#define EPS 1e-8
#define INF 1000000007
#define MOD 1000000007
#define MAXN 100005
using namespace std;

typedef long long LL;
typedef unsigned long long ULL;
typedef long double LD;

const double PI=3.14159265359;
const double EARTH_RADIUS=6371;

struct debugger{
    template<typename T> debugger& operator , (const T& v){
        cerr<<v<<" ";
        return *this;
    }
}dbg;

struct Point {
    int id;
    double lat,lon;
    Point(int _id=0,double _lat=0,double _lon=0) {
        id=_id;
        lon=RAD(_lon);
        lat=RAD(_lat);
    }
};
typedef struct Point PT;

LL distance_between(PT p1,PT p2) {
    double raw= acos(sin(p1.lat)*sin(p2.lat)+
                cos(p1.lat)*cos(p2.lat)*
                cos(p2.lon-p1.lon))*EARTH_RADIUS;
    LL ret=raw*PREC;
    raw*=PREC;
    raw-=ret;
    if(raw>=0.5) ret++;
    return ret;
}

PT hotel;
map<string,double>vel;
vector<PT>places;

bool cmp(PT a,PT b) {
    LL distA=distance_between(a,hotel),distB=distance_between(b,hotel);
    if(distA==distB) return a.id<b.id;
    return distA<distB;
}

int main() {
    FRI;
    FRO;
    int n,m,i,j,id;
    double lat,lon,tim,dist;
    bool first;
    string vehicle;

    vel["metro"]=20;
    vel["bike"]=15;
    vel["foot"]=5;

    cin>>n;
    for(i=0;i<n;i++) {
        cin>>id>>lat>>lon;
        places.PB(PT(id,lat,lon));
    }

    cin>>m;
    for(i=0;i<m;i++) {
        cin>>hotel.lat>>hotel.lon>>vehicle>>tim;
        hotel.lat=RAD(hotel.lat);
        hotel.lon=RAD(hotel.lon);
        sort(places.begin(),places.end(),cmp);
        dist=(vel[vehicle]*tim)/60;
        dist*=PREC;
        first=true;
        for(j=0;j<n;j++) {
            if((double)distance_between(places[j],hotel)>dist) break;
            if(!first) cout<<" ";
            cout<<places[j].id;
            first=false;
        }
        cout<<endl;
    }
    return 0;
}
