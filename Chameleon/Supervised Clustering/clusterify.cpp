#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#define eps 0.000001
using namespace std;

double avgTolerancePCT=0.05,sdTolerancePCT=5.3;

class Point {
public:
    int assigned;
    vector <double> coord, tolerance;
    double distanceFrom(Point p) {
        int d=coord.size(),i;
        double ans=0;
        for(i=0;i<d;i++) {
            ans+=(coord[i]-p.coord[i])*(coord[i]-p.coord[i]);
        }
        return sqrt(ans);
    }
    bool contains(Point P) {
        int i,d=coord.size(),missed=0;
        double temp;
        for(i=0;i<d;i++) {
            temp=coord[i]-P.coord[i];
            temp=temp<0?temp*(-1):temp;
            if(temp>tolerance[i]) {
                missed++;
            }
        }
        return missed==0;
    }
};

int main(int argc, char *argv[]) {
    int n,d,k,i,j,m,cnt,totalFalseTrue,totalFalseFalse,itCount=0,N;
    double x,minDist,maxDif,diff,avgTolerance,sdTolerance;
    char cc;
    string name;
    ifstream din("phoneTrain.txt");
    ifstream tin("phoneTest.txt");
//    ifstream din("tabTrain.txt");
//    ifstream tin("tabTest.txt");
    Point temp;
    vector <Point> points,mids;
    /****************************************************************/
    din>>n>>d>>k;
    N=n/k;
    temp.assigned=-1;
    for(i=0;i<n;i++) {
        temp.coord.clear();
        for(j=0;j<d;j++) {
            din>>x;
            din>>cc;
            temp.coord.push_back(x);
        }
        points.push_back(temp);
        din>>name;
    }
    for(i=0;i<k;i++) {
        temp.coord.clear();
        temp.tolerance.clear();
        for(j=0;j<d;j++) {
            x=0;
            for(m=i*N;m<(i+1)*N;m++){
                x+=points[m].coord[j];
            }
            x/=N;
            temp.coord.push_back(x);
            x=0;
            for(m=i*N;m<(i+1)*N;m++){
                x+=(points[m].coord[j]-temp.coord[j])*(points[m].coord[j]-temp.coord[j]);
            }
            x/=N;
            x=sqrt(x);
            avgTolerance=temp.coord[j]*avgTolerancePCT;
            sdTolerance=x*sdTolerancePCT;
            avgTolerance=avgTolerance<0?avgTolerance*(-1):avgTolerance;
            temp.tolerance.push_back(avgTolerance+sdTolerance);
        }
        mids.push_back(temp);
    }
    maxDif=eps+1;
    while(maxDif>eps && ++itCount<=1000) {
        maxDif=0;
        for(i=0;i<n;i++) {
            points[i].assigned=-1;
            minDist=1e9;
            for(j=0;j<k;j++)
                if(mids[j].contains(points[i]))
                    if(points[i].distanceFrom(mids[j])<minDist){
                        points[i].assigned=j;
                        minDist=points[i].distanceFrom(mids[j]);
                    }
        }
        for(i=0;i<k;i++) {
            for(m=0;m<d;m++) {
                temp.coord[m]=mids[i].coord[m];
                mids[i].coord[m]=0;
            }
            cnt=0;
            for(j=0;j<n;j++)
                if(points[j].assigned==i) {
                    cnt++;
                    for(m=0;m<d;m++) {
                        mids[i].coord[m]+=points[j].coord[m];
                    }
                }
            mids[i].assigned=cnt;
            if(cnt>0){
                for(m=0;m<d;m++){
                    mids[i].coord[m]/=cnt;
                    avgTolerance=mids[i].coord[m]*avgTolerancePCT;
                    avgTolerance=avgTolerance<0?avgTolerance*(-1):avgTolerance;
                    x=0;
                    for(j=0;j<n;j++) {
                        if(points[j].assigned==i) {
                            x+=(points[j].coord[m]-mids[i].coord[m])*(points[j].coord[m]-mids[i].coord[m]);
                        }
                    }
                    x/=cnt;
                    x=sqrt(x);
                    sdTolerance=x*sdTolerancePCT;
                    mids[i].tolerance[m]=avgTolerance+sdTolerance;
                    diff=mids[i].coord[m]-temp.coord[m];
                    diff=diff<0?diff*(-1):diff;
                    maxDif=max(maxDif,diff);
                }
            }
        }
    }
    if(++itCount>1000) {
        return 0;
    }
    totalFalseFalse=0;
    totalFalseTrue=0;
    tin>>n;
    for(i=0;i<n;i++){
        temp.assigned=-1;
        temp.coord.clear();
        for(j=0;j<d;j++) {
            tin>>x;
            tin>>cc;
            temp.coord.push_back(x);
        }
        tin>>cnt;
        for(j=0;j<k;j++)
            if(mids[j].contains(temp)){
                if(sdTolerancePCT>0){
                    for(m=0;m<d;m++){
                        x=mids[j].coord[m]<0?mids[j].coord[m]*(-1):mids[j].coord[m];
                        mids[j].tolerance[m]=(mids[j].tolerance[m]-x*avgTolerancePCT)/sdTolerancePCT;
                        mids[j].tolerance[m]=mids[j].tolerance[m]*mids[j].tolerance[m]*mids[j].assigned;

                        mids[j].coord[m]=mids[j].coord[m]*mids[j].assigned+temp.coord[m];
                        mids[j].assigned++;
                        mids[j].coord[m]/=mids[j].assigned;
                        x=mids[j].coord[m]<0?mids[j].coord[m]*(-1):mids[j].coord[m];

                        mids[j].tolerance[m]+=(mids[j].coord[m]-temp.coord[m])*(mids[j].coord[m]-temp.coord[m]);
                        mids[j].tolerance[m]/=mids[j].assigned;
                        mids[j].tolerance[m]=sqrt(mids[j].tolerance[m]);
                        mids[j].tolerance[m]=mids[j].tolerance[m]*sdTolerancePCT+x*avgTolerancePCT;
                    }
                }
                if(cnt%5!=(j+1)%5)
                    totalFalseTrue++;
                else
                    temp.assigned=j;
            }
        if(temp.assigned==-1)
            totalFalseFalse++;
    }
    cout<<totalFalseTrue*(100.0)/(n*k-n)<<setw(20)<<totalFalseFalse*(100.0)/n<<endl;
    return  0;
}
