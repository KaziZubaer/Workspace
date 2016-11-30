//AC code for warehouse

#include<algorithm>
#include<bits/stdc++.h>
using namespace std;

int ans[65536],n,pIndex[17][17];

int DP(unsigned int bit){
    int minimum,box,i,temp;
    if(ans[bit]!=-1)
        return ans[bit];
    minimum=INT_MAX;
    box=n;
    for(i=0;i<n;i++)
        if((bit&(1<<i))!=0)
            box--;
    for(i=0;i<n;i++)
        if((bit&(1<<i))!=0){
            bit&=~(1<<i);
            temp=pIndex[box][i]+DP(bit);
            if(temp<0) temp=INT_MAX;
            minimum=min(minimum,temp);
            bit|=(1<<i);
        }
    return ans[bit]=minimum;
}

int main(){
    char grid[50][50];
    int T,i,j,boxMap[50][50],placeMap[50][50],dist[50][50],R,C,k,l,top,x,y,boxIndex,placeIndex;
    unsigned int bit;
    queue<int>q;
    cin>>T;
    while(T--){
        cin>>R>>C;
        boxIndex=0;
        placeIndex=0;
        for(i=0;i<R;i++)
            for(j=0;j<C;j++) {
                cin>>grid[i][j];
                if(grid[i][j]=='B') boxMap[i][j]=boxIndex++;
                if(grid[i][j]=='X') placeMap[i][j]=placeIndex++;
                else placeMap[i][j]=-1;
            }
        n=boxIndex;
        for(i=0;i<n;i++) for(j=0;j<n;j++) pIndex[i][j]=INT_MAX;
        for(i=0;i<R;i++)
            for(j=0;j<C;j++) {
                if(grid[i][j]!='B') continue;
                for(k=0;k<R;k++) for(l=0;l<C;l++) dist[k][l]=-1;
                dist[i][j]=0;
                q.push(i*C+j);
                while(!q.empty()) {
                    top=q.front();
                    q.pop();
                    y=top%C;
                    x=top/C;
                    if(placeMap[x][y]!=-1) pIndex[boxMap[i][j]][placeMap[x][y]]=dist[x][y];
                    if(x>0&&grid[x-1][y]!='#'&&dist[x-1][y]==-1) {
                        dist[x-1][y]=dist[x][y]+1;
                        q.push((x-1)*C+y);
                    }
                    if(x<R-1&&grid[x+1][y]!='#'&&dist[x+1][y]==-1) {
                        dist[x+1][y]=dist[x][y]+1;
                        q.push((x+1)*C+y);
                    }
                    if(y>0&&grid[x][y-1]!='#'&&dist[x][y-1]==-1) {
                        dist[x][y-1]=dist[x][y]+1;
                        q.push(x*C+y-1);
                    }
                    if(y<C-1&&grid[x][y+1]!='#'&&dist[x][y+1]==-1) {
                        dist[x][y+1]=dist[x][y]+1;
                        q.push(x*C+y+1);
                    }
                }
            }
        bit=(1<<n);
        for(i=1;i<bit;i++)
            ans[i]=-1;
        ans[0]=0;
        cout<<DP(bit-1)<<endl;
    }
    return 0;
}
