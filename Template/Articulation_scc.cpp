int bothCapacity[MAXN][MAXN];
vector<int>bothEdges[MAXN];

int BFSforMaxFlow(int source,int destination){ //Directed graph. No cycle
    queue<int>Q;
    int i,parent[MAXN],top,next,len,answer;
    bool visited[MAXN];
    Q.push(source);
    for(i=0;i<MAXN;i++){
        visited[i]=false;
        parent[i]=-1;
    }
    visited[source]=true;
    while(!Q.empty()){
        top=Q.front();
        Q.pop();
        len=bothEdges[top].size();
        for(i=0;i<len;i++){
            next=bothEdges[top][i];
            if(visited[next]||bothCapacity[top][next]==0)
                continue;
            Q.push(next);
            visited[next]=true;
            parent[next]=top;
            if(next==destination){
                while(!Q.empty())
                    Q.pop();
                break;
            }
        }
    }
    top=destination;answer=INF;
    while(parent[top]!=-1){
        next=parent[top];
        answer=min(answer,bothCapacity[next][top]);
        top=next;
    }
    top=destination;
    while(parent[top]!=-1){
        next=parent[top];
        bothCapacity[next][top]-=answer;
        bothCapacity[top][next]+=answer;
        top=next;
    }
    if(answer==INF)
        return 0;
    return answer;
}

int MaxFlow(int source,int destination,int capacity[MAXN][MAXN],vector<int>edges[MAXN]){ //Directed graph. No cycle
    int answer=0,i,j,len,next,temp;
    for(i=0;i<MAXN;i++)
        for(j=0;j<MAXN;j++)
            bothCapacity[i][j]=0;
    for(i=0;i<MAXN;i++){
        len=edges[i].size();
        for(j=0;j<len;j++){
            next=edges[i][j];
            bothEdges[i].push_back(next);
            bothEdges[next].push_back(i);
            bothCapacity[next][i]=0;
            bothCapacity[i][next]=capacity[i][next];
        }
    }
    while(true){
        temp=BFSforMaxFlow(source,destination);
        if(temp==0)
            break;
        answer+=temp;
    }
    return answer;
}

void articulationPointAndBridge (int u) {
    bool thisArticulationPoint=false;
    dfs_low[u]=dfs_num[u]=predfn++; //dfs_low[u]<=dfs_num
    for(int j=0;j<edge[u].size();j++) {
        int v=edge[u][j];
        if(dfs_num[v]==UNVISITED) {
            dfs_parent[v]=u;
            if(u==dfsRoot) rootChild++; //Remember to check in main function
            articulationPointAndBridge(v);

            /**For articulation point**/
            if(dfs_low[v]>=dfs_num[u])
                thisArticulationPoint=true;
            /**For articulation bridge**/
            if(dfs_low[v]>dfs_num[u])
                bridges.push_back(Edge(u,v));

            dfs_low[u]=min(dfs_low[u],dfs_low[v]);
        }
        else if(v!=dfs_parent[u])
            dfs_low[u]=min(dfs_low[u],dfs_low[v]);
    }
    if(thisArticulationPoint)
        articulationPoints.push_back(u);
}

void tarjanSCC (int u) {
    dfs_low[u]=dfs_num[u]=predfn++; //dfs_low[u]<=dfs_num
    S.push(u);
    visiting[u]=true;
    for(int j=0;j<edge[u].size();j++) {
        int v=edge[u][j];
        if(dfs_num[v]==UNVISITED)
            tarjanSCC(v);
        if(visiting[v])
            dfs_low[u]=min(dfs_low[u],dfs_low[v]);
    }
    if(dfs_low[u]==dfs_num[u]) {
        while(true) {
            int v=S.top();
            S.pop();
            visiting[v]=false;
            scc[v]=numSCC;
            if(v==u) break;
        }
        numSCC++;
    }
}
