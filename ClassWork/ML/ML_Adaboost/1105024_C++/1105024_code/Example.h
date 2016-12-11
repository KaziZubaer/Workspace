#define NCRITERIA 8
#define MAXN 10000

struct Example {
    int val[NCRITERIA];
    int label;
    double weight;
    bool selected;
} examples[MAXN];
