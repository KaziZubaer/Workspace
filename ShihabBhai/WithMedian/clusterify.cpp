#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <algorithm>
#define eps 0.000001
using namespace std;

double avgTolerancePCT,sdTolerancePCT,medTolerancePCT;
int missTolerance;
bool param[5];

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

            /*******Increase missed only if param[i] is being considered*******/
            if(temp>tolerance[i]&&param[i]) {
                missed++;
            }
            /******************************************************************/
        }
        return missed<=missTolerance;
    }
};

int main(int argc, char *argv[]) {
    int n,d,k,i,j,m,cnt,totalFalseTrue,totalFalseFalse,itCount=0,N;
    double x,minDist,maxDif,diff,avgTolerance,sdTolerance,medTolerance;
    vector<double>arrMed;
    char cc;
    ifstream din("data.txt");
    ifstream tin("test.txt");
    fstream rout;
    Point temp;
    vector <Point> points,mids;
    avgTolerancePCT=atof(argv[1]);
    sdTolerancePCT=atof(argv[2]);
    medTolerancePCT=atof(argv[3]);
    missTolerance=atoi(argv[4]);
    cout<<avgTolerancePCT<<" "<<sdTolerancePCT<<" "<<medTolerancePCT<<" "<<missTolerance<<endl;
    for(i=0;i<5;i++) param[i]=false;
    cnt=atoi(argv[5]);
    for(i=0;i<cnt;i++) param[atoi(argv[6+i])]=true;
    rout.open(argv[6+i],fstream::in | fstream::out | fstream::app);
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
        din>>cc;
    }
    for(i=0;i<k;i++) {
        temp.coord.clear();
        temp.tolerance.clear();
        for(j=0;j<d;j++) {
            x=0;
            arrMed.clear();
            for(m=i*N;m<(i+1)*N;m++){
                x+=points[m].coord[j];
                arrMed.push_back(points[m].coord[j]);
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
            sort(arrMed.begin(),arrMed.end());
            if(arrMed.size()%2==0) {
                medTolerance=arrMed[arrMed.size()/2];
            }
            else {
                medTolerance=(arrMed[arrMed.size()/2-1]+arrMed[arrMed.size()/2])/2;
            }
            medTolerance=abs(medTolerance)*medTolerancePCT;
            temp.tolerance.push_back(avgTolerance+sdTolerance+medTolerance);
        }
        mids.push_back(temp);
    }
    maxDif=eps+1;
    while(maxDif>eps && ++itCount<=1000){
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
                    arrMed.clear();
                    for(j=0;j<n;j++) {
                        if(points[j].assigned==i) {
                            x+=(points[j].coord[m]-mids[i].coord[m])*(points[j].coord[m]-mids[i].coord[m]);
                            arrMed.push_back(points[j].coord[m]);
                        }
                    }
                    x/=cnt;
                    x=sqrt(x);
                    sdTolerance=x*sdTolerancePCT;
                    sort(arrMed.begin(),arrMed.end());
                    if(arrMed.size()%2==0) {
                        medTolerance=arrMed[arrMed.size()/2];
                    }
                    else {
                        medTolerance=(arrMed[arrMed.size()/2-1]+arrMed[arrMed.size()/2])/2;
                    }
                    medTolerance=abs(medTolerance)*medTolerancePCT;
                    mids[i].tolerance[m]=avgTolerance+sdTolerance+medTolerance;
                    diff=mids[i].coord[m]-temp.coord[m];
                    diff=diff<0?diff*(-1):diff;
                    maxDif=max(maxDif,diff);
                }
            }
        }
    }
    if(++itCount>1000) {
        rout.close();
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
                if(cnt!=j+1)
                    totalFalseTrue++;
                else
                    temp.assigned=j;
            }
        if(temp.assigned==-1)
            totalFalseFalse++;
    }
    rout<<setw(20)<<avgTolerancePCT*100<<setw(20)<<sdTolerancePCT*100<<setw(20)<<missTolerance<<setw(20)<<totalFalseTrue*(100.0)/(n*k-n)<<setw(20)<<totalFalseFalse*(100.0)/n<<endl;
    rout.close();
    return  0;
}
