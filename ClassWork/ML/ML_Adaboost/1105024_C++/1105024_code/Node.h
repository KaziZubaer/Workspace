#define EPS 1e-12
#define NVAL 10

class Node {
public:
    int label,childCriteria,val,level;
    std::vector<int>children,data,criteriaSet;

    Node(int val,int level) : val(val), level(level), label(-1) {}
    double Entropy() {
        int i;
        double total=data.size(),cnt[2]={0,0},entropy=0;
        for(i=0;i<data.size();i++)
            cnt[examples[data[i]].label]++;
        for(i=0;i<2;i++)
            entropy-=cnt[i]<EPS?0:(cnt[i]/total)*log2(cnt[i]/total);
        return entropy;
    }
    double EntropyOf(int id,int v) {
        int i;
        double total=0,cnt[2]={0,0},entropy=0;
        for(i=0;i<data.size();i++)
            if(examples[data[i]].val[id]==v) {
                cnt[examples[data[i]].label]++;
                total++;
            }
        if(total<EPS)
            return 0;
        for(i=0;i<2;i++)
            entropy-=cnt[i]<EPS?0:(cnt[i]/total)*log2(cnt[i]/total);
        return entropy;
    }
    double Gain(int id) {
        int i;
        double total=data.size(),cnt[NVAL],gain=Entropy();
        for(i=0;i<NVAL;i++)
            cnt[i]=0;
        for(i=0;i<data.size();i++)
            cnt[examples[data[i]].val[id]]++;
        for(i=0;i<NVAL;i++)
            gain-=(cnt[i]/total)*EntropyOf(id,i);
        return gain;
    }
    int getMajority() {
        int i,cnt[2]={0,0};
        for(i=0;i<data.size();i++)
            cnt[examples[data[i]].label]++;
        if(cnt[0]>cnt[1]) return 0;
        else return 1;
    }
    bool isDataOver() {
        return data.size()==0;
    }
    bool isCriteriaOver() {
        if(criteriaSet.size()>0)
            return false;
        label=getMajority();
        return true;
    }
    bool isSame() {
        int i;
        for(i=1;i<data.size();i++)
            if(examples[data[i]].label!=examples[data[0]].label)
                return false;
        label=examples[data[0]].label;
        return true;
    }
    int getBest() {
        int i,id=criteriaSet[0];
        double maxGain=0,gain;
        for(i=0;i<criteriaSet.size();i++) {
            gain=Gain(criteriaSet[i]);
            if(gain>maxGain) {
                maxGain=gain;
                id=criteriaSet[i];
            }
        }
        childCriteria=id;
        return id;
    }
};
