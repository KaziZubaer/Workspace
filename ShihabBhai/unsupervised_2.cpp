#include <bits/stdc++.h>

#define INF 1e9
#define EPS 0.000001

using namespace std;

int degree;
double avgTolerancePCT,sdTolerancePCT;

double getRand(double lo,double hi) {
    return lo+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(hi-lo+EPS)));
}

class Point {
public:
    vector <double> coord;
    string owner;
    double distanceFrom(Point p) {
        int i;
        double ans=0;
        for(i=0;i<degree;i++) {
            ans+=(coord[i]-p.coord[i])*(coord[i]-p.coord[i]);
        }
        return sqrt(ans);
    }
};

class Person {
public:
    Point mid,tolerance;
    vector <Point> points;

    double calculate() {
        int i,j;
        double gap,x;
        vector<double>previ;

        if(mid.coord.size()==degree) {
            for(j=0;j<degree;j++) {
                previ.push_back(mid.coord[j]);
            }
        }
        else {
            for(j=0;j<degree;j++) {
                previ.push_back(INF);
            }
        }
        mid.coord.clear();
        tolerance.coord.clear();
        for(j=0;j<degree;j++) {
            mid.coord.push_back(0);
            tolerance.coord.push_back(0);
        }

        if(points.size()==0) {
            for(j=0;j<degree;j++) {
                mid.coord[j]=INF;
            }
        }
        else {
            for(i=0;i<points.size();i++) {
                for(j=0;j<degree;j++) {
                    mid.coord[j]+=points[i].coord[j];
                }
            }
            for(j=0;j<degree;j++) {
                mid.coord[j]/=points.size();
                tolerance.coord[j]=abs(mid.coord[j]*avgTolerancePCT);
            }
        }
        for(i=0;i<degree;i++) {
            x=0;
            for(j=0;j<points.size();j++) {
                x+=(points[j].coord[i]-mid.coord[i])*(points[j].coord[i]-mid.coord[i]);
            }
            if(points.size()>0) {
                x/=points.size();
            }
            x=sqrt(x);
            tolerance.coord[i]+=x*sdTolerancePCT;
        }
        for(j=0;j<degree;j++) {
            gap=max(gap,abs(previ[j]-mid.coord[j]));
        }
        return gap;
    }

    double distanceFrom(Point p) {
        return mid.distanceFrom(p);
    }

    bool contains(Point p) {
        int i;
        for(i=0;i<degree;i++) {
            if(abs(p.coord[i]-mid.coord[i])>tolerance.coord[i]) {
                return false;
            }
        }
        return true;
    }
};

int main(int argc, char *argv[]) {
    int n,i,j,k,pId,cnt;
    double x,maxDiff=1,tDiff,minDiff;
    char cc;
    string name;
    set<string>::iterator it;

    vector <Point> dataPoints;
    vector <double> ranges[2];
    vector <Person> people;
    set<string>names;

    ifstream din("data.txt");
    ofstream uout("unsupervisedResult.txt");
    Point tPoint;
    Person tPerson;

    srand(time(NULL));

    din>>n>>degree;
    for(i=0;i<degree;i++) {
        ranges[0].push_back(INF);
        ranges[1].push_back(-INF);
    }
    for(i=0;i<n;i++) {
        tPoint.coord.clear();
        for(j=0;j<degree;j++) {
            din>>x;
            ranges[0][j]=min(ranges[0][j],x);
            ranges[1][j]=max(ranges[1][j],x);
            din>>cc;
            tPoint.coord.push_back(x);
        }
        din>>name;
        tPoint.owner=name;
        dataPoints.push_back(tPoint);
        names.insert(name);
    }

    cout<<"Number of clusters: ";
    cin>>k;
    cout<<"AVG Tolerance PCT: ";
    cin>>avgTolerancePCT;
    avgTolerancePCT/=100;
    cout<<"SD Tolerance PCT: ";
    cin>>sdTolerancePCT;
    sdTolerancePCT/=100;
    for(i=0;i<k;i++) {
        tPerson.mid.coord.clear();
        for(j=0;j<degree;j++) {
            tPerson.mid.coord.push_back(getRand(ranges[0][j],ranges[1][j]));
        }
        people.push_back(tPerson);
    }

    while(maxDiff>EPS) {
        for(i=0;i<k;i++) {
            people[i].points.clear();
        }
        for(i=0;i<dataPoints.size();i++) {
            minDiff=INF;
            for(j=0;j<k;j++) {
                tDiff=people[j].distanceFrom(dataPoints[i]);
                if(tDiff<minDiff) {
                    minDiff=tDiff;
                    pId=j;
                }
            }
            people[pId].points.push_back(dataPoints[i]);
        }
        maxDiff=0;
        for(i=0;i<k;i++) {
            maxDiff=max(maxDiff,people[i].calculate());
        }
    }

    uout<<"   ";
    for(it=names.begin();it!=names.end();it++) {
        uout<<setw(3)<<*it;
    }
    uout<<endl;

    for(i=0;i<k;i++) {
        uout<<setw(2)<<i+1<<":";
        for(it=names.begin();it!=names.end();it++) {
            cnt=0;
            for(j=0;j<dataPoints.size();j++) {
                if(dataPoints[j].owner!=*it) {
                    continue;
                }
                if(people[i].contains(dataPoints[j])) {
                    cnt++;
                }
            }
            uout<<setw(3)<<cnt;
        }
        uout<<endl;
    }
    return  0;
}
