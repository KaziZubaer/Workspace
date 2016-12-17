#define PI acos(-1.0)

class Channel {
    int k;
    double nexVal;
    bool nexValAvailable;
    vector<double>h;
    vector<double>avg;

public:
    /***helper***/
    double getGaussian() {
        if(nexValAvailable) {
            nexValAvailable=false;
            return nexVal;
        }
        else {
            double u1=(double)(rand()%1000000)/1000000.0;
            double u2=(double)(rand()%1000000)/1000000.0;
            double r=sqrt(-2*log(u1));
            nexVal=r*sin(2*PI*u2);
            nexValAvailable=true;
            return r*cos(2*PI*u2);
        }
    }
    double getGaussProb(double x,int state) {
        double prob=(x-avg[state]);
        prob*=prob*0.5*(-1);
        prob=exp(prob);
        prob/=sqrt(2*PI);
        return prob;
    }

    /***functions***/
    Channel() {}
    Channel(const vector<double>&_h) {
        int i;
        k=_h.size();
        for(i=0;i<k;i++) {
            h.push_back(_h[i]);
        }
        nexValAvailable=false;
    }
    double calculateChannelOut(int state,int last) {
        int i;
        double result=getGaussian();
        for(i=k-last;i<k;i++) {
            if(state&(1<<i)) {
                result+=h[i];
            }
            else {
                result-=h[i];
            }
        }
        return result;
    }
    void train(int n) {
        int i,state=0;
        vector<int>cnt;
        avg.clear();
        for(i=0;i<(1<<k);i++) {
            avg.push_back(0);
            cnt.push_back(0);
        }
        for(i=0;i<k-1;i++) {
            if(rand()&1) {
                state|=(1<<i);
            }
        }
        state=(state<<1);
        for(;i<n;i++) {
            state=(state>>1);
            if(rand()&1) {
                state|=(1<<(k-1));
            }
            avg[state]+=calculateChannelOut(state,k);
            cnt[state]++;
        }
        for(i=0;i<(1<<k);i++) {
            if(cnt[i]>0) {
                avg[i]/=cnt[i];
            }
        }
    }
    void test(const vector<bool>&given,vector<bool>&result) {
        int i,j,state=0;
        vector<int>preState[TEST_CNT];
        double x;
        vector<double>dp[2];
        for(i=0;i<(1<<k);i++) {
            dp[0].push_back(0);
            dp[1].push_back(0);
        }
        for(i=0;i<TEST_CNT;i++) {
            state=(state>>1);
            if(given[i]) {
                state|=(1<<(k-1));
            }
            x=calculateChannelOut(state,min(i+1,k));
            for(j=0;j<(1<<k);j++) {
                dp[1][j]=log(getGaussProb(x,j));
                if(dp[0][((1<<(k-1))-1)&(j<<1)]>dp[0][(((1<<(k-1))-1)&(j<<1))+1]) {
                    dp[1][j]+=dp[0][((1<<(k-1))-1)&(j<<1)];
                    preState[i].push_back(((1<<(k-1))-1)&(j<<1));
                }
                else {
                    dp[1][j]+=dp[0][((1<<(k-1))-1)&((j<<1)+1)];
                    preState[i].push_back((((1<<(k-1))-1)&(j<<1))+1);
                }
            }
            for(j=0;j<(1<<k);j++) {
                dp[0][j]=dp[1][j];
            }
        }
        result.clear();
        state=-1;
        x=-(1e18);
        for(i=0;i<(1<<k);i++) {
            if(dp[0][i]>x) {
                x=dp[0][i];
                state=i;
            }
        }
        if(state&(1<<(k-1))) {
            result.push_back(true);
        }
        else {
            result.push_back(false);
        }
        state=preState[TEST_CNT-1][state];
        for(i=TEST_CNT-2;i>=0;i--) {
            if(state&(1<<(k-1))) {
                result.push_back(true);
            }
            else {
                result.push_back(false);
            }
            state=preState[i][state];
        }
        reverse(result.begin(),result.end());
    }

    /***functions for debugging***/
    void showH() {
        int i;
        for(i=0;i<k;i++) cout<<h[i]<<" ";
        cout<<endl;
    }
};
