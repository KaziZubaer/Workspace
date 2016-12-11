#define CALCDEL CalculateDelSigmoid
//#define CALCDEL CalculateDelSoftmax

class Layer {
public:
    int thisNCnt,preNCnt;
    Matrix A,B,W,Z,Zp,D,G,Gb;
    void RandomInit() {
        int i,j;
        W.SetDim(thisNCnt,preNCnt);
        B.SetDim(thisNCnt,1);
        for(i=0;i<thisNCnt;i++) {
            B.mat[i][0]=(rand()%1000+1)/50000.0;
            for(j=0;j<preNCnt;j++) {
                W.mat[i][j]=(rand()%1000+1)/50000.0;
            }
        }
    }
    Layer(int _thisNCnt,int _preNCnt=0) {
        thisNCnt=_thisNCnt;
        preNCnt=_preNCnt;
        A.SetDim(thisNCnt,1);
        if(preNCnt!=0) {
            Z.SetDim(thisNCnt,1);
            Zp.SetDim(thisNCnt,1);
            D.SetDim(thisNCnt,1);
            RandomInit();
        }
    }
    void FeedForwardSigmoid(Layer &toFeed) {
        int i;
        toFeed.Z=toFeed.W*A+toFeed.B;
        for(i=0;i<toFeed.thisNCnt;i++) {
            toFeed.A.mat[i][0]=1/(1+exp(-toFeed.Z.mat[i][0]));
            if(toFeed.A.mat[i][0]!=toFeed.A.mat[i][0]) {
                assert(false);
            }
        }
    }
    void FeedForwardSoftmax(Layer &toFeed) {
        int i;
        double total=0,maxi=-INFINITY;
        toFeed.Z=toFeed.W*A+toFeed.B;
        for(i=0;i<toFeed.thisNCnt;i++) {
            maxi=maxi>toFeed.Z.mat[i][0]?maxi:toFeed.Z.mat[i][0];
        }
        for(i=0;i<toFeed.thisNCnt;i++) {
            total+=exp(toFeed.Z.mat[i][0]-maxi);
            if(total!=total) {
                assert(false);
            }
        }
        for(i=0;i<toFeed.thisNCnt;i++) {
            toFeed.A.mat[i][0]=exp(toFeed.Z.mat[i][0]-maxi)/total;
            if(toFeed.A.mat[i][0]!=toFeed.A.mat[i][0]||toFeed.A.mat[i][0]==INFINITY) {
                assert(false);
            }
        }
    }
    void ClearGrad() {
        G.SetDim(thisNCnt,preNCnt);
        Gb.SetDim(thisNCnt,1);
    }
    void CalculateDelSoftmax() {
        int i;
        double total=0,maxi=-INFINITY;
        for(i=0;i<thisNCnt;i++) {
            maxi=maxi>Z.mat[i][0]?maxi:Z.mat[i][0];
        }
        for(i=0;i<thisNCnt;i++) {
            total+=exp(Z.mat[i][0]-maxi);
            if(total!=total) {
                assert(false);
            }
        }
        for(i=0;i<thisNCnt;i++) {
            Zp.mat[i][0]=((exp(Z.mat[i][0]-maxi)/total)*(total-exp(Z.mat[i][0]-maxi)))/total;
            if(Zp.mat[i][0]!=Zp.mat[i][0]||Zp.mat[i][0]==INFINITY) {
                assert(false);
            }
        }
    }
    void CalculateDelSigmoid() {
        int i;
        double x;
        for(i=0;i<thisNCnt;i++) {
            x=1/(1+exp(-Z.mat[i][0]));
            Zp.mat[i][0]=x*(1-x);
            if(Zp.mat[i][0]!=Zp.mat[i][0]) assert(false);
        }
    }
    void BackPropagate(Layer &toPropagate) {
        int i;
        Matrix M;
        M=W.Transpose()*D;
        toPropagate.CALCDEL();
        for(i=0;i<toPropagate.thisNCnt;i++) {
            toPropagate.D.mat[i][0]=M.mat[i][0]*toPropagate.Zp.mat[i][0];
        }
    }
    void UpdateGrad(Layer &prevLayer) {
        int x;
        G=G+D*prevLayer.A.Transpose();
        Gb=Gb+D;
    }
    void UpdateWeight(double learningRate,double trainCnt) {
        int x;
        W=W-G*(learningRate/trainCnt);
        B=B-Gb*(learningRate/trainCnt);
    }
};
