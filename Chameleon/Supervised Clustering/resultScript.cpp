#include <iomanip>
#include <iostream>
#include <fstream>
#define EPS 1e-8
using namespace std;

int main() {
    int missTolerance[2],i,cnt=0,l;
    float avgInterval,sdInterval,avg[2],sd[2],j,k;
    bool param[5];
    char reply;
    ofstream sout("resultScript.sh");
    cout<<"Average Tolerance From: ";
    cin>>avg[0];
    cout<<"Average Tolerance To: ";
    cin>>avg[1];
    cout<<"Average Tolerance Interval: ";
    cin>>avgInterval;
    cout<<"SD Tolerance From: ";
    cin>>sd[0];
    cout<<"SD Tolerance To: ";
    cin>>sd[1];
    cout<<"SD Tolerance Interval: ";
    cin>>sdInterval;
    cout<<"Miss Tolerance Tolerance From: ";
    cin>>missTolerance[0];
    cout<<"Miss Tolerance Tolerance To: ";
    cin>>missTolerance[1];

    /************************New 5 Questions*********************/
    for(i=0;i<5;i++) {
        cout<<"Do you want to consider parameter #"<<(i+1)<<"? ";
        cin>>reply;
        param[i] = (reply == 'y');
        if(param[i]) cnt++;
    }
    /************************************************************/

    for(i=missTolerance[0];i<=missTolerance[1];i++) {
        for(j=avg[0]/100;j<=avg[1]/100+EPS;j+=avgInterval/100) {
            for(k=sd[0]/100;k<=sd[1]/100+EPS;k+=sdInterval/100) {
                sout<<"./clusterify "<<j<<" "<<k<<" "<<i<<" ";

                /*******Add the new responses to the sh file*****/
                sout<<cnt<<" ";
                for(l=0;l<5;l++) if(param[l]) sout<<l<<" ";
                /************************************************/

                sout<<"result_"<<i<<"_"<<setprecision(2)<<fixed<<(j*100)<<".txt"<<endl;
            }
        }
    }
    return 0;
}
