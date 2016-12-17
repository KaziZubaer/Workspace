#include <iomanip>
#include <iostream>
#include <fstream>
#define EPS 1e-4
using namespace std;

int main() {
    int missTolerance[2],i,cnt=0,l,choice;
    float avgInterval,sdInterval,medInterval,avg[2],sd[2],med[2],j,k,mm;
    bool param[5];
    char reply;
    ofstream sout("resultScript.sh");
    CHOOSE:
    cout<<"Which parameter do you want to change?"<<endl
        <<"1. Average Tolerance"<<endl
        <<"2. SD Tolerance"<<endl
        <<"3. Median Tolerance"<<endl
        <<"4. Miss Tolearance"<<endl;
    cin>>choice;
    switch(choice) {
    case 1:
        cout<<"Average Tolerance From: ";
        cin>>avg[0];
        cout<<"Average Tolerance To: ";
        cin>>avg[1];
        cout<<"Average Tolerance Interval: ";
        cin>>avgInterval;
        cout<<"Keep SD Tolerance at: ";
        cin>>sd[0];
        sd[1]=sd[0];
        sdInterval=1e18;
        cout<<"Keep Median Tolerance at: ";
        cin>>med[0];
        med[1]=med[0];
        medInterval=1e18;
        cout<<"Keep Miss Tolerance at: ";
        cin>>missTolerance[0];
        missTolerance[1]=missTolerance[0];
        break;
    case 2:
        cout<<"SD Tolerance From: ";
        cin>>sd[0];
        cout<<"SD Tolerance To: ";
        cin>>sd[1];
        cout<<"SD Tolerance Interval: ";
        cin>>sdInterval;
        cout<<"Keep Avg Tolerance at: ";
        cin>>avg[0];
        avg[1]=avg[0];
        avgInterval=1e18;
        cout<<"Keep Median Tolerance at: ";
        cin>>med[0];
        med[1]=med[0];
        medInterval=1e18;
        cout<<"Keep Miss Tolerance at: ";
        cin>>missTolerance[0];
        missTolerance[1]=missTolerance[0];
        break;
    case 3:
        cout<<"Median Tolerance From: ";
        cin>>med[0];
        cout<<"Median Tolerance To: ";
        cin>>med[1];
        cout<<"Median Tolerance Interval: ";
        cin>>medInterval;
        cout<<"Keep Avg Tolerance at: ";
        cin>>avg[0];
        avg[1]=avg[0];
        avgInterval=1e18;
        cout<<"Keep SD Tolerance at: ";
        cin>>sd[0];
        sd[1]=sd[0];
        sdInterval=1e18;
        cout<<"Keep Miss Tolerance at: ";
        cin>>missTolerance[0];
        missTolerance[1]=missTolerance[0];
        break;
    case 4:
        cout<<"Miss Tolerance From: ";
        cin>>missTolerance[0];
        cout<<"Miss Tolerance To: ";
        cin>>missTolerance[1];
        cout<<"Keep Avg Tolerance at: ";
        cin>>avg[0];
        avg[1]=avg[0];
        avgInterval=1e18;
        cout<<"Keep SD Tolerance at: ";
        cin>>sd[0];
        sd[1]=sd[0];
        sdInterval=1e18;
        cout<<"Keep Median Tolerance at: ";
        cin>>med[0];
        med[1]=med[0];
        medInterval=1e18;
    default:
        cout<<"Please enter a number from 1 to 4\n";
        goto CHOOSE;
    }
    cout<<endl;
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
                for(mm=med[0]/100;mm<=med[1]/100+EPS;mm+=medInterval/100) {
                    sout<<"./clusterify "<<j<<" "<<k<<" "<<mm<<" "<<i<<" ";

                    /*******Add the new responses to the sh file*****/
                    sout<<cnt<<" ";
                    for(l=0;l<5;l++) if(param[l]) sout<<l<<" ";
                    /************************************************/

                    sout<<"result_varying";
                    switch(choice) {
                    case 1:
                        sout<<"_avgTol";
                        break;
                    case 2:
                        sout<<"_sdTol";
                        break;
                    case 3:
                        sout<<"_medTol";
                        break;
                    case 4:
                        sout<<"_MissTol";
                    }
                    if(choice!=1) {
                        sout<<"_"<<setprecision(2)<<fixed<<(j*100);
                    }
                    if(choice!=2) {
                        sout<<"_"<<setprecision(2)<<fixed<<(k*100);
                    }
                    if(choice!=3) {
                        sout<<"_"<<setprecision(2)<<fixed<<(mm*100);
                    }
                    if(choice!=4) {
                        sout<<"_"<<i;
                    }
                    sout<<".txt"<<endl;
                }
            }
        }
    }
    return 0;
}
