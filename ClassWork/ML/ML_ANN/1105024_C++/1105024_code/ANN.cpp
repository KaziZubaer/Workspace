#include <bits/stdc++.h>
#include "Matrix.h"
#include "Layer.h"

#define DIM (28*28)
#define TRAINPCT 80
#define CLASSCNT 10
#define LAYERCNT 3
#define LEARNINGRATE 3
#define BATCHSIZE 30
#define ITERATION 1000

/**
                Sigmoid

raining example: 10000
Validation example: 1000

rate    batchSize   midLayer    Accuracy

3       30          30          99.2% & 95.5%

                Softmax

Training example: 5000
Validation example: 500

rate    batchSize   midLayer    Accuracy

3       30          30          40% & Flactuates
0.75    100         30          50%
1       100         30          50%
3       100         30          40%
1       100         50          60%
1       100         80          60%
1       30          50,30       --
1       30          50          77%
1       30          80          60%
1       30          30          55%
1       60          50          78.6%
3       60          50          55% & Flactuates%
0.8     30          50          80%
0.8     60          50          78.6%
1       80          50          62.4%

Training example: 25000
Validation example: 100

rate    batchSize   midLayer    Accuracy

1       30          46          84%

**/

#define FEED FeedForwardSigmoid
//#define FEED FeedForwardSoftmax

using namespace std;

void ReverseInt(unsigned int &i) {
    i = (i & 0x00FFFF00) | ((i & 0xFF000000) >> 24 | (i & 0x000000FF) << 24);
    i = (i & 0xFF0000FF) | ((i & 0x00FF0000) >> 8 | (i & 0x0000FF00) << 8);
}

vector<Layer>network;
vector<Matrix>trainX,trainY,validationX,validationY,testX,testY;
int row,col;
int neuronCnt[LAYERCNT]={DIM,30,CLASSCNT};

void ReadFromFile(int dataCnt,FILE *image,FILE *label,vector<Matrix>&X,vector<Matrix>&Y) {
    unsigned char *imageBuffer,*labelBuffer;
    int i,j,k,l;

    /**Read from file**/
    imageBuffer=(unsigned char*)malloc(dataCnt*DIM);
    labelBuffer=(unsigned char*)malloc(dataCnt);

    fread(imageBuffer,1,dataCnt*DIM,image);
    fread(labelBuffer,1,dataCnt,label);

    /**Store data in data structure**/
    for(i=0;i<dataCnt;i++) {

        X.push_back(Matrix(DIM,1));
        Y.push_back(Matrix(CLASSCNT,1));

        l=0;
        for(j=0;j<row;j++) {
            for(k=0;k<col;k++) {
                X[i].mat[l++][0]=((double)imageBuffer[i*DIM+j*col+k])/255.0;
            }
        }
        for(j=0;j<CLASSCNT;j++) {
            Y[i].mat[j][0]=labelBuffer[i]==j?1:0;
        }
    }
}

void ShowExample(int idx) {
    int i,j,l=0;
    for(i=0;i<row;i++) {
        for(j=0;j<col;j++)
            cout<<setw(4)<<fixed<<(int)trainX[idx].mat[l++][0];
        cout<<endl;
    }
    for(i=0;i<CLASSCNT;i++) if((int)trainY[idx].mat[i][0]==1) cout<<i<<endl;
}

int main() {
    ofstream resOut("result.txt");
    int trainCnt,validationCnt,testCnt,i,j,cnt,b,thisCnt,rnd,maxi[2]={0,0};
    unsigned int n;

    FILE *trainImage,*trainLabel,*testImage,*testLabel;

    /**Open files**/
    trainImage=fopen("train-images.idx3-ubyte","rb");
    trainLabel=fopen("train-labels.idx1-ubyte","rb");
    testImage=fopen("t10k-images.idx3-ubyte","rb");
    testLabel=fopen("t10k-labels.idx1-ubyte","rb");

    /**Read first bytes of training label**/
    fread(&n,1,4,trainLabel);
    fread(&n,1,4,trainLabel);

    ReverseInt(n);
    trainCnt=n*(TRAINPCT)/100;
    validationCnt=n-trainCnt;

    /**Read first bytes of training image**/
    fread(&n,1,4,trainImage);
    fread(&n,1,4,trainImage);

    fread(&n,1,4,trainImage);
    ReverseInt(n);
    row=n;

    fread(&n,1,4,trainImage);
    ReverseInt(n);
    col=n;

    /**Read first bytes of test label**/
    fread(&n,1,4,testLabel);
    fread(&n,1,4,testLabel);

    ReverseInt(n);
    testCnt=n;

    /**Read first bytes of test image**/
    fread(&n,1,4,testImage);
    fread(&n,1,4,testImage);
    fread(&n,1,4,testImage);
    fread(&n,1,4,testImage);

    trainCnt=2500;
    validationCnt=500;
    testCnt=50;

    /**Read data**/
    ReadFromFile(trainCnt,trainImage,trainLabel,trainX,trainY);
    ReadFromFile(validationCnt,trainImage,trainLabel,validationX,validationY);
    ReadFromFile(testCnt,testImage,testLabel,testX,testY);

    /**Initialize the network**/
    network.push_back(Layer(neuronCnt[0]));

    for(i=1;i<LAYERCNT;i++) {
        network.push_back(Layer(neuronCnt[i],neuronCnt[i-1]));
    }

    /**Main run**/
    cnt=0;
    while(cnt++<ITERATION) {

        rnd=rand()%trainCnt;

        for(b=0;b<trainCnt/BATCHSIZE;b++) {

            for(i=1;i<LAYERCNT;i++) {
                network[i].ClearGrad();
            }

            for(i=b*BATCHSIZE,thisCnt=0;i<(b+1)*BATCHSIZE;i++,thisCnt++) {

//                if(i==rnd) ShowExample(i);

                /**Input layer**/
                for(j=0;j<DIM;j++) {
                    network[0].A.mat[j][0]=trainX[i].mat[j][0];
                }

                /**Forward feed**/
                for(j=0;j<LAYERCNT-1;j++) {
                    network[j].FEED(network[j+1]);
                }

                /**Calculate error of last layer**/
                network[LAYERCNT-1].CALCDEL();
                for(j=0;j<network[LAYERCNT-1].thisNCnt;j++) {
                    network[LAYERCNT-1].D.mat[j][0]
                    = ((network[LAYERCNT-1].A.mat[j][0]-trainY[i].mat[j][0])*network[LAYERCNT-1].Zp.mat[j][0]);
                }

                /**Back propagate the errors**/
                for(j=LAYERCNT-1;j>1;j--) {
                    network[j].BackPropagate(network[j-1]);
                }

                /**Update Gradient Descent**/
                for(j=1;j<LAYERCNT;j++) {
                    network[j].UpdateGrad(network[j-1]);
                }
            }

            /**Update weights**/
            for(i=1;i<LAYERCNT;i++) {
                network[i].UpdateWeight(LEARNINGRATE,thisCnt);
            }
        }

        int trueCnt=0;
        int falseCnt=0;

        for(i=0;i<trainCnt;i++) {
            /**Input layer**/
            for(j=0;j<DIM;j++) {
                network[0].A.mat[j][0]=trainX[i].mat[j][0];
            }

            /**Forward feed**/
            for(j=0;j<LAYERCNT-1;j++) {
                network[j].FEED(network[j+1]);
            }

            /**Validate**/
            double maxi=0;
            int res=-1;
            for(j=0;j<network[LAYERCNT-1].thisNCnt;j++) {
                if(network[LAYERCNT-1].A.mat[j][0]>maxi) {
                    maxi=network[LAYERCNT-1].A.mat[j][0];
                    res=j;
                }
            }

            if(res>=0&&trainY[i].mat[res][0]>0.5) trueCnt++;
            else falseCnt++;
        }

        maxi[0]=max(maxi[0],trueCnt);
        cout<<cnt<<": "<<trueCnt<<" "<<falseCnt<<" "<<maxi[0]<<endl;

        trueCnt=0;
        falseCnt=0;

        for(i=0;i<validationCnt;i++) {
            /**Input layer**/
            for(j=0;j<DIM;j++) {
                network[0].A.mat[j][0]=validationX[i].mat[j][0];
            }

            /**Forward feed**/
            for(j=0;j<LAYERCNT-1;j++) {
                network[j].FEED(network[j+1]);
            }

            /**Validate**/
            double maxi=0;
            int res=-1;
            for(j=0;j<network[LAYERCNT-1].thisNCnt;j++) {
                if(network[LAYERCNT-1].A.mat[j][0]>maxi) {
                    maxi=network[LAYERCNT-1].A.mat[j][0];
                    res=j;
                }
            }

            if(res>=0&&validationY[i].mat[res][0]>0.5) trueCnt++;
            else falseCnt++;
        }

        maxi[1]=max(maxi[1],trueCnt);
        cout<<cnt<<": "<<trueCnt<<" "<<falseCnt<<" "<<maxi[1]<<endl;
        resOut<<cnt<<" : "<<((double)maxi[1]*100.00)/validationCnt<<endl;
    }

    /**Close files**/
    fclose(trainLabel);
    fclose(trainImage);
    fclose(testLabel);
    fclose(testImage);

    return 0;
}
