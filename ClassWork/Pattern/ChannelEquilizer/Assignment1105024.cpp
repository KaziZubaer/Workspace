using namespace std;

#define TRAIN_CNT 1000000
#define TEST_CNT 10000

#include <bits/stdc++.h>
#include "Channel.h"

vector<bool>given,obtained;

int main() {
    int i,k,err;
    double x;
    vector<double>h;
    Channel channel;

    cin>>k;
    for(i=0;i<k;i++) {
        cin>>x;
        h.push_back(x);
    }

    /***create channel and train***/
    channel=Channel(h);
    srand(time(NULL));
    channel.train(TRAIN_CNT);

    /***generate test data and test***/
    for(i=0;i<TEST_CNT;i++) {
        if(rand()&1) {
            given.push_back(true);
        }
        else {
            given.push_back(false);
        }
    }
    channel.test(given,obtained);

    err=0;
    for(i=0;i<TEST_CNT;i++) {
        if(given[i]^obtained[i]) {
            err++;
        }
    }

    /***get result summary***/
    cout<<err<<" "<<TEST_CNT<<endl;

    return 0;
}
