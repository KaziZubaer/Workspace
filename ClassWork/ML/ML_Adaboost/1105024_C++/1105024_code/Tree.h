#define MAXEXPAND 1

class Tree {
public:
    std::vector<Node>nodes;
    double beta;
    int nodeCnt;
    Tree() {
        nodeCnt=0;
    }

    void MakeTree(int s) {
        if(nodes[s].isDataOver()) return;
        if(nodes[s].isCriteriaOver()) return;
        if(nodes[s].isSame()) return;
        int id=nodes[s].getBest(),n=nodes[s].data.size(),m=nodes[s].criteriaSet.size(),i,j,cid,d;
        for(i=0;i<NVAL;i++) {
            nodes.push_back(Node(i,nodes[s].level+1));
            for(j=0;j<n;j++) {
                d=examples[nodes[s].data[j]].val[id];
                if(d==i)
                    nodes[nodeCnt].data.push_back(nodes[s].data[j]);
            }
            for(j=0;j<m;j++) {
                cid=nodes[s].criteriaSet[j];
                if(cid!=id&&nodes[nodeCnt].level<MAXEXPAND)
                    nodes[nodeCnt].criteriaSet.push_back(cid);
            }
            nodes[s].children.push_back(nodeCnt++);
            MakeTree(nodeCnt-1);
        }
    }

    int GetResult(int s,int id) {
        if(nodes[s].label!=-1) return nodes[s].label;
        int i,c;
        for(i=0;i<NVAL;i++)
            if(examples[id].val[nodes[s].childCriteria]==nodes[nodes[s].children[i]].val) {
                c=nodes[s].children[i];
                if(nodes[c].data.size()==0)
                    return nodes[s].getMajority();
                else
                    return GetResult(c,id);
            }
        assert(false);
    }
};
