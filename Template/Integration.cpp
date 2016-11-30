#include<bits/stdc++.h>
using namespace std;

class Term {
public:
    int degree;
    double coeff;
    Term(int _degree,double _coeff) { degree=_degree;coeff=_coeff; }
    Term integration() { return Term(degree+1,coeff/(degree+1)); }
    double val(double x) {
        int i;
        double ans=coeff;
        for(i=0;i<degree;i++) ans*=x;
        return ans;
    }
};

void squarePoly(vector<Term> &polynomial,vector<Term> &sPolynomial) {
    int i,j,degree,N=polynomial.size();
    double coeff;
    sPolynomial.clear();
    for(i=0;i<N;i++)
        for(j=0;j<N;j++) {
            degree=polynomial[i].degree+polynomial[j].degree;
            coeff=polynomial[i].coeff*polynomial[j].coeff;
            sPolynomial.push_back(Term(degree,coeff));
        }
}

void Integrate(vector<Term> &polynomial,vector<Term> &iPolynomial) {
    int N=polynomial.size(),i;
    iPolynomial.clear();
    for(i=0;i<N;i++) iPolynomial.push_back(polynomial[i].integration());
}

double getIntegratedVal(vector<Term> &polynomial,double a,double b) {
    int i,N=polynomial.size();
    double ans=0;
    for(i=0;i<N;i++) ans+=polynomial[i].val(b)-polynomial[i].val(a);
    return ans;
}

int main() {
    int n,i;
    double coeff,a,b;
    vector<Term>poly,iPoly;
    while(true) {
        scanf("%d",&n);
        if(n==0) break;
        poly.clear();
        for(i=0;i<=n;i++) {
            scanf("%lf",&coeff);
            poly.push_back(Term(n-i,coeff));
        }
        scanf("%lf %lf",&a,&b);
        Integrate(poly,iPoly);
        printf("%.3lf\n",getIntegratedVal(iPoly,a,b)/(b-a));
    }
    return 0;
}
