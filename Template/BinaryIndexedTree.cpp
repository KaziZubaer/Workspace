/** Binary Indexed Tree implementation **/
#include<bits/stdc++.h>
#define MAXN 100
using namespace std;

int tree[MAXN],tree2D[MAXN][MAXN],maxN,max_x,max_y;

/**
Read cumulative sum from 1D BIT
**/
int read(int idx) {
    int sum=0;
    while(idx>0) {
        sum+=tree[idx];
        idx-=(idx & -idx);
    }
    return sum;
}

/**
Update in 1D BIT. Single element. Increases by val
**/
void update(int idx,int val) {
    while(idx<=maxN) {
        tree[idx]+=val;
        idx+=(idx & -idx);
    }
}

/**
Range update in 1D BIT. Special Functions
**/
/*
int tree1[MAXN],tree2[MAXN];

int read1(int idx) {
    int sum=0;
    while(idx>0) {
        sum+=tree1[idx];
        idx-=(idx & -idx);
    }
    return sum;
}

int read2(int idx) {
    int sum=0;
    while(idx>0) {
        sum+=tree2[idx];
        idx-=(idx & -idx);
    }
    return sum;
}

int read(int idx) {
    int sum=read1(idx)+read2(idx)*idx;
    return sum;
}

void update1(int idx,int val) {
    while(idx<=maxN) {
        tree1[idx]+=val;
        idx+=(idx & -idx);
    }
}

void update2(int idx,int val) {
    while(idx<=maxN) {
        tree2[idx]+=val;
        idx+=(idx & -idx);
    }
}

void rangeUpdate(int a,int b,int val) {
    update1(a,-(val*(a-1)));
    update1(b+1,val*b);
    update2(a,val);
    update2(b+1,-val);
}
*/

/**
Read single index from 1D BIT
**/
int readSingle(int idx) {
    int sum=tree[idx];
    if(idx>0) {
        int z=idx-(idx & -idx);
        idx--;
        while(idx!=z) {
            sum-=tree[idx];
            idx-=(idx & -idx);
        }
    }
    return sum;
}

/**
If in tree exists more than one index with a same
cumulative frequency, this procedure will return
the greatest one. Numbers should be non-negative.
If not exists, return -1
**/
int findG(int cumFre) {
    int idx=0;

    int bitMask = log2(maxN);
    bitMask = 1<<bitMask;
    while((bitMask!=0) && (idx<maxN)) {
        int tIdx=idx+bitMask;
        if(cumFre>=tree[tIdx]) {
            idx=tIdx;
            cumFre-=tree[tIdx];
        }
        bitMask>>=1;
    }
    if(cumFre!=0)
        return-1;
    else
        return idx;
}

/**
Update in 2D BIT
**/
void update(int x,int y,int val){
    int y1;
    while(x<=max_x) {
        y1=y;
        while(y1<=max_y) {
            tree2D[x][y1]+=val;
            y1+=(y1 & -y1);
        }
        x+=(x & -x);
    }
}

int read(int x,int y) {
    int sum=0,sumy,y1;
    while(x>0) {
        sumy=0;
        y1=y;
        while(y1>0) {
            sumy+=tree2D[x][y1];
            y1-=(y1 & -y1);
        }
        sum+=sumy;
        x-=(x & -x);
    }
    return sum;
}

int main() {
    return 0;
}
