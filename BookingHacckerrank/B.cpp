#include<bits/stdc++.h>

#define PB push_back
#define MP make_pair
#define F first
#define S second

#define FRI freopen("input001.txt","r",stdin)
#define FRO freopen("out.txt","w",stdout)
#define debug(args...) {dbg,args; cerr<<endl;}
#define DB(x) #x"=>",x
#define RAD(x) ((x*PI)/180)
#define NEX(x) ((x)==n-1?0:(x)+1)
#define PRE(x) ((x)==0?n-1:(x)-1)
#define DEG(x) ((x*180)/PI)

#define EPS 1e-12
#define INF 1000000007
#define MOD 1000000007
#define MAXN 100005
#define ALPHABET_SIZE 26
using namespace std;

typedef long long LL;
typedef unsigned long long ULL;
typedef long double LD;

const double PI=acos(-1.0);

struct debugger{
    template<typename T> debugger& operator , (const T& v){
        cerr<<v<<" ";
        return *this;
    }
}dbg;

struct TrieNode {
    int val;
    struct TrieNode *children[ALPHABET_SIZE];
    bool isLeaf;
};

struct TrieNode *getNode(void) {
    struct TrieNode *pNode=NULL;
    pNode=(struct TrieNode *)malloc(sizeof(struct TrieNode));
    if(pNode) {
        int i;
        pNode->isLeaf=false;
        pNode->val=0;
        for(i=0;i<ALPHABET_SIZE;i++)
            pNode->children[i]=NULL;
    }
    return pNode;
}

void insertTrie(struct TrieNode *root, const char *key) {
    int level;
    int length=strlen(key);
    int index;
    struct TrieNode *pCrawl=root;
    for(level=0;level<length;level++) {
        pCrawl->val++;
        index=key[level]-'a';
        if(!pCrawl->children[index])
            pCrawl->children[index]=getNode();
        pCrawl=pCrawl->children[index];
    }
    pCrawl->val++;
    pCrawl->isLeaf = true;
}

bool searchTrieExact(struct TrieNode *root, const char *key) {
    int level;
    int length=strlen(key);
    int index;
    struct TrieNode *pCrawl=root;
    for (level=0;level<length;level++) {
        index=key[level]-'a';
        if (!pCrawl->children[index])
            return false;
        pCrawl=pCrawl->children[index];
    }
    return (pCrawl!=NULL&&pCrawl->isLeaf);
}

char oneWord[1000];
TrieNode *root=getNode();
map<int,int>score;
vector< pair<int,int> >result;

int main() {
    FRI;
    int i=0,j,n,id,sc;
    bool flag=true;
    while(flag) {
        scanf("%c",&oneWord[i]);
        if(oneWord[i]=='\n'||oneWord[i]==' ') {
            if(oneWord[i]=='\n')
                flag=false;
            if(i>0) {
                oneWord[i]=0;
                i=0;
                insertTrie(root,oneWord);
            }
        }
        else {
            if('A'<=oneWord[i]&&oneWord[i]<='Z') oneWord[i]+=('a'-'A');
            i++;
        }
    }
    cout<<"OK"<<endl;
    scanf("%d",&n);
    for(i=0;i<n;i++) {
        scanf("%d",&id);
        flag=true;
        j=0;
        scanf("%c",&oneWord[j]);
        while(flag) {
            if(scanf("%c",&oneWord[j])==EOF) {
                oneWord[j]=0;
                if(j>0) {
                    if(searchTrieExact(root,oneWord)) {
                        if(score.find(id)!=score.end()) sc=score[id];
                        else sc=0;
                        score[id]=sc+1;
                    }
                }
                break;
            }
            if(oneWord[j]=='.'||oneWord[j]==',')
                continue;
            if(oneWord[j]=='\n'||oneWord[j]==' '||!flag) {
                if(oneWord[j]=='\n')
                    flag=false;
                if(j>0) {
                    oneWord[j]=0;
                    j=0;
                    cout<<string(oneWord)<<endl;
                    cout<<"2"<<endl;
                    if(searchTrieExact(root,oneWord)) {
                        if(score.find(id)!=score.end()) sc=score[id];
                        else sc=0;
                        score[id]=sc+1;
                    }
                    cout<<"22"<<endl;
                }
            }
            else {
                if('A'<=oneWord[j]&&oneWord[j]<='Z') oneWord[j]+=('a'-'A');
                j++;
            }
        }
    }
    for(auto it=score.begin();it!=score.end();it++)
        result.PB(MP(-(it->S),it->F));
    sort(result.begin(),result.end());
    for(i=0;i<result.size()-1;i++)
        printf("%d ",result[i].S);
    printf("%d\n",result[i].S);
    return 0;
}
