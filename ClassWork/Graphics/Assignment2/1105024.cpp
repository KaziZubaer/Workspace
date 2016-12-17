#include <bits/stdc++.h>
#define RAD(x) (((x)*PI)/180.0)
using namespace std;

const double PI=acos(-1.0);

class Matrix {
public:
    int dim[2],label;
    vector< vector<double> >a;
    Matrix() {}
    Matrix(int d0,int d1) {
        vector<double>temp;
        int i;
        dim[0]=d0;
        dim[1]=d1;
        for(i=0;i<d1;i++) temp.push_back(0);
        for(i=0;i<d0;i++) a.push_back(temp);
    }
    void SetDim(int d0,int d1) {
        vector<double>temp;
        int i;
        a.clear();
        dim[0]=d0;
        dim[1]=d1;
        for(i=0;i<d1;i++) temp.push_back(0);
        for(i=0;i<d0;i++) a.push_back(temp);
    }
    Matrix (const Matrix &m) {
        this->SetDim(m.dim[0],m.dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                a[i][j]=m.a[i][j];
    }
    void Clear() {
        int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                a[i][j]=0;
    }
    Matrix operator + (const Matrix &m) {
    	assert(dim[0]==m.dim[0]&&dim[1]==m.dim[1]);
    	Matrix ret=Matrix(dim[0],dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                ret.a[i][j]=a[i][j]+m.a[i][j];
	    return ret;
    }
    Matrix operator - (const Matrix &m) {
    	assert(dim[0]==m.dim[0]&&dim[1]==m.dim[1]);
    	Matrix ret=Matrix(dim[0],dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                ret.a[i][j]=a[i][j]-m.a[i][j];
	    return ret;
    }
    Matrix operator * (double c) {
    	Matrix ret=Matrix(dim[0],dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                ret.a[i][j]=a[i][j]*c;
	    return ret;
    }
    Matrix operator * (const Matrix &m) {
    	assert(dim[1]==m.dim[0]);
    	Matrix ret=Matrix(dim[0],m.dim[1]);
    	int i,j,k;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<m.dim[1];j++)
                for(k=0;k<dim[1];k++)
                    ret.a[i][j]+=a[i][k]*m.a[k][j];
	    return ret;
    }
};

class Point : public Matrix {
public:
    Point() {
        this->SetDim(4,1);
    }
    void operator = (const Matrix &m) {
        assert(m.dim[0]==4&&m.dim[1]==1);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                a[i][j]=m.a[i][j];
    }
    double dot (const Point &p) {
    	assert(dim[0]==p.dim[0]);
    	int i,j;
    	double ans=0;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                ans+=a[i][j]*p.a[i][j];
	    return ans;
    }
    void Read() {
        int i;
        for(i=0;i<3;i++) cin>>a[i][0];
        a[3][0]=1;
    }
    void MakeUnit() {
        int i;
        double m=0;
        for(i=0;i<3;i++) m+=a[i][0]*a[i][0];
        m=sqrt(m);
        for(i=0;i<3;i++) a[i][0]/=m;
        a[3][0]=1;
    }
    void Normalize() {
        int i;
        for(i=0;i<3;i++) a[i][0]/=a[3][0];
    }
    void Show(FILE *outputFile) {
        int i;
        for(i=0;i<3;i++) fprintf(outputFile,"%.7lf ",a[i][0]);
        fprintf(outputFile,"\n");
    }
};

Point Cross(Point a,Point b) {
    Point Res;
    Res.a[0][0]=a.a[1][0]*b.a[2][0]-a.a[2][0]*b.a[1][0];
    Res.a[1][0]=a.a[2][0]*b.a[0][0]-a.a[0][0]*b.a[2][0];
    Res.a[2][0]=a.a[0][0]*b.a[1][0]-a.a[1][0]*b.a[0][0];
    Res.a[3][0]=1;
    return Res;
}

Point Rotate(Point X,Point A,double ang) {
    Point W,Xp;
    A.MakeUnit();
    W=Cross(A,X);
    Xp=X-A*(A.dot(X)-1);
    X=A*(A.dot(X)-1)+Xp*cos(RAD(ang))+W*sin(RAD(ang));
    return X;
}

Matrix GetRotationMatrix(Point A,double ang) {
    Matrix R(4,4);
    Point W,X;
    int i;

    X.a[0][0]=1; X.a[1][0]=0; X.a[2][0]=0; X.a[3][0]=1;
    X=Rotate(X,A,ang);
    for(i=0;i<3;i++) R.a[i][0]=X.a[i][0];

    X.a[0][0]=0; X.a[1][0]=1; X.a[2][0]=0; X.a[3][0]=1;
    X=Rotate(X,A,ang);
    for(i=0;i<3;i++) R.a[i][1]=X.a[i][0];

    X.a[0][0]=0; X.a[1][0]=0; X.a[2][0]=1; X.a[3][0]=1;
    X=Rotate(X,A,ang);
    for(i=0;i<3;i++) R.a[i][2]=X.a[i][0];

    for(i=0;i<3;i++) R.a[3][i]=R.a[i][3]=0;
    R.a[3][3]=1;
    return R;
}

void ApplyTransform(Point &p,vector<Matrix>&trans,int exact=-1) {
    int i;
    if(exact==-1) {
        for(i=trans.size()-1;i>=2;i--) {
            p=trans[i]*p;
            p.Normalize();
        }
    }
    else {
        p=trans[exact]*p;
        p.Normalize();
    }
}

vector<Matrix>transStk;

int main() {
    int i;
    double x,y,z,ang,fovY,fovX,asp,yLim,xLim,near,far;
    string cmd;
    FILE *stage[3];
    Matrix I(4,4),R(4,4);
    Point p,eye,lookAt,r,l,u;

    freopen("scene.txt","r",stdin);
    stage[0]=fopen("stage1.txt","w");
    stage[1]=fopen("stage2.txt","w");
    stage[2]=fopen("stage3.txt","w");

    for(i=0;i<4;i++) I.a[i][i]=1;
    transStk.push_back(I);

    eye.Read();
    lookAt.Read();
    u.Read();

    l=lookAt-eye;
    l.MakeUnit();
    r=Cross(l,u);
    r.MakeUnit();
    u=Cross(r,l);

    R.Clear();
    R.a[0][0]=r.a[0][0]; R.a[0][1]=r.a[1][0]; R.a[0][2]=r.a[2][0];
    R.a[1][0]=u.a[0][0]; R.a[1][1]=u.a[1][0]; R.a[1][2]=u.a[2][0];
    R.a[2][0]=-l.a[0][0]; R.a[2][1]=-l.a[1][0]; R.a[2][2]=-l.a[2][0];
    R.a[3][3]=1;
    transStk.push_back(R);

    R.Clear();
    R.a[0][0]=1; R.a[1][1]=1; R.a[2][2]=1;
    R.a[0][3]=-eye.a[0][0]; R.a[1][3]=-eye.a[1][0]; R.a[2][3]=-eye.a[2][0]; R.a[3][3]=1;
    transStk[1]=transStk[1]*R;

    cin>>fovY>>asp>>near>>far;
    fovX=fovY*asp;
    yLim=near*tan(RAD(fovY/2));
    xLim=near*tan(RAD(fovX/2));

    R.Clear();
    R.a[0][0]=near/xLim;
    R.a[1][1]=near/yLim;
    R.a[2][2]=-(far+near)/(far-near);
    R.a[2][3]=-(2*far*near)/(far-near);
    R.a[3][2]=-1;
    transStk[0]=R;

    transStk.push_back(I);

    while(true) {
        cin>>cmd;
        if(cmd=="triangle") {
            for(i=0;i<3;i++) {
                p.Read();
                ApplyTransform(p,transStk);
                p.Show(stage[0]);
                ApplyTransform(p,transStk,1);
                p.Show(stage[1]);
                ApplyTransform(p,transStk,0);
                p.Show(stage[2]);
            }
            for(i=0;i<3;i++) fprintf(stage[i],"\n");
        }
        else if(cmd=="scale") {
            cin>>x>>y>>z;
            R.Clear();
            R.a[0][0]=x; R.a[1][1]=y; R.a[2][2]=z; R.a[3][3]=1;
            transStk[transStk.size()-1]=transStk[transStk.size()-1]*R;
        }
        else if(cmd=="translate") {
            cin>>x>>y>>z;
            R.Clear();
            R.a[0][0]=1; R.a[1][1]=1; R.a[2][2]=1;
            R.a[0][3]=x; R.a[1][3]=y; R.a[2][3]=z; R.a[3][3]=1;
            transStk[transStk.size()-1]=transStk[transStk.size()-1]*R;
        }
        else if(cmd=="rotate") {
            cin>>ang;
            p.Read();
            R=GetRotationMatrix(p,ang);
            transStk[transStk.size()-1]=transStk[transStk.size()-1]*R;
        }
        else if(cmd=="push") {
            transStk.push_back(I);
        }
        else if(cmd=="pop") {
            transStk.pop_back();
        }
        else if(cmd=="end") {
            break;
        }
    }
    return 0;
}
