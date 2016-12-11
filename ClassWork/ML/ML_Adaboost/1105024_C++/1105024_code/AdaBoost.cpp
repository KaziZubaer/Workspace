#include<bits/stdc++.h>
#include "Example.h"
#include "Node.h"
#include "Tree.h"

#define NCLASS 2

#define TRAINSIZE 0.8
#define TREESIZE 0.5

using namespace std;

char criteriaName[8][100],dump[100];
vector<int>all,training,test;
vector<Tree>trees;
vector<double>probability;
int ROUND=30;

int main() {
    freopen("in.csv","r",stdin);
    int i,j,n=0,m,sz,k,idx,id,decision;
    double  totalWeight,error,truePositive=0,falseNegative=0,falsePositive=0,
            trueNegative=0,precision,recall,accuracy,f1,classProbability[NCLASS];
    for(i=0;i<NCRITERIA;i++) {
        scanf("%[^,]",criteriaName[i]);
        scanf(",");
    }
    scanf("%s",dump);
    while(scanf("%d",&examples[n].val[0])!=EOF) {
        examples[n].val[0]--;
        scanf(",");
        for(i=1;i<NCRITERIA;i++) {
            scanf("%d",&examples[n].val[i]);
            examples[n].val[i]--;
            scanf(",");
        }
        scanf("%d",&examples[n].label);
        all.push_back(n++);
    }
    srand(time(NULL));
    random_shuffle(all.begin(),all.end());
    m=n*TRAINSIZE;
    sz=m*TREESIZE;
    for(i=0;i<m;i++) training.push_back(all[i]);
    for(;i<n;i++) test.push_back(all[i]);
    for(i=0;i<m;i++) examples[training[i]].weight=((double)1)/m;
    for(k=0;k<ROUND;k++) {
        probability.clear();
        totalWeight=0;
        for(i=0;i<m;i++) {
            totalWeight+=examples[training[i]].weight;
            examples[training[i]].selected=false;
        }
        for(i=0;i<m;i++) probability.push_back(examples[training[i]].weight/totalWeight);
        for(i=1;i<m;i++) probability[i]+=probability[i-1];
        trees.push_back(Tree());
        trees[k].nodes.push_back(Node(-1,0));
        trees[k].nodeCnt++;
        for(i=0;i<sz;i++) {
            idx=lower_bound(probability.begin(),probability.end(),((double)(rand()%1000000))/1000000)-probability.begin();
            if(idx==m) idx--;
            if(examples[training[idx]].selected) {
                i--;
                continue;
            }
            trees[k].nodes[0].data.push_back(training[idx]);
            examples[training[idx]].selected=true;
        }
        for(i=0;i<NCRITERIA;i++)
            trees[k].nodes[0].criteriaSet.push_back(i);
        trees[k].MakeTree(0);
        error=0;
        for(i=0;i<m;i++) {
            if(!examples[training[i]].selected) continue;
            id=trees[k].GetResult(0,training[i]);
            if(id!=examples[training[i]].label) {
                examples[training[i]].selected=false;
                error+=probability[i]-(i>0?probability[i-1]:0);
            }
        }
        if(error>0.5) {
            ROUND=k;
            break;
        }
        trees[k].beta=error/(1-error);
        for(i=0;i<m;i++)
            if(examples[training[i]].selected)
                examples[training[i]].weight*=trees[k].beta;
    }
    for(i=0;i<test.size();i++) {
        for(j=0;j<NCLASS;j++) classProbability[j]=0;
        for(j=0;j<ROUND;j++)
            classProbability[trees[j].GetResult(0,test[i])]-=log(trees[j].beta);
        decision=classProbability[0]>classProbability[1]?0:1;
        if(decision==examples[test[i]].label) {
            if(decision==1) truePositive++;
            else trueNegative++;
        }
        else {
            if(decision==1) falsePositive++;
            else falseNegative++;
        }
    }
    precision=truePositive/(truePositive+falsePositive);
    recall=truePositive/(truePositive+falseNegative);
    accuracy=(truePositive+trueNegative)/test.size();
    f1=(2*precision*recall)/(precision+recall);
    cout<<"Precision = "<<precision<<", Recall = "<<recall<<", Accuracy = "<<accuracy<<", F1-Score = "<<f1<<endl;
    return 0;
}
