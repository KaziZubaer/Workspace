#include<bits/stdc++.h>
#include "Example.h"
#include "Node.h"
#include "Tree.h"

#define NCLASS 2

#define TRAINSIZE 0.8

using namespace std;

char criteriaName[8][100],dump[100];
vector<int>all,training,test;
Tree tree;

int main() {
    freopen("in.csv","r",stdin);
    int i,j,n=0,m,decision;
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
    for(i=0;i<m;i++) training.push_back(all[i]);
    for(;i<n;i++) test.push_back(all[i]);
    tree.nodes.push_back(Node(-1,0));
    tree.nodeCnt++;

    for(i=0;i<m;i++)
        tree.nodes[0].data.push_back(training[i]);
    for(i=0;i<NCRITERIA;i++)
        tree.nodes[0].criteriaSet.push_back(i);
    tree.MakeTree(0);

    for(i=0;i<test.size();i++) {
        decision=tree.GetResult(0,test[i]);
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
