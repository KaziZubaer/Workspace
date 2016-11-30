#include<bits/stdc++.h>
#define PB push_back
#define MP make_pair
#define F first
#define S second
#define FRI freopen("in.txt","r",stdin)
#define FRO freopen("out.txt","w",stdout)
#define debug(args...) {dbg,args; cerr<<endl;}
#define EPS 1e-12
#define RAD(x) ((x*PI)/180)
#define ALPHABET_SIZE 26
using namespace std;

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

int searchTriePrefix(struct TrieNode *root, const char *key) {
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
    if(!pCrawl) return 0;
    return pCrawl->val;
}

int main() {
    char keys[][8] = {"the", "a", "there", "answer", "any",
                     "by", "bye", "their"};
    char output[][32] = {"Not present in trie", "Present in trie"};
    char s[100];
    struct TrieNode *root = getNode();
    int i;
    for (i=0;i<8;i++)
        insertTrie(root,keys[i]);
    while(true) {
        scanf("%s",s);
        printf("%d\n",searchTriePrefix(root,s));
    }
    return 0;
}
