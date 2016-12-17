#include <bits/stdc++.h>
#include "bitmap_image.hpp"

#define RAD(x) (((x)*PI)/180.0)
#define NEX(x) ((x)==2?0:(x)+1)
#define INF 10
#define EPS 1e-10

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
    Matrix (const Matrix &m) {
        this->SetDim(m.dim[0],m.dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                a[i][j]=m.a[i][j];
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
    double dot (const Matrix &m) {
    	assert(dim[0]==m.dim[0]&&dim[1]==m.dim[1]);
    	int i,j;
    	double ans=0;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                ans+=a[i][j]*m.a[i][j];
	    return ans;
    }
    void ShowMat() {
        int i,j;
	    for(i=0;i<dim[0];i++) {
            for(j=0;j<dim[1];j++)
                cout<<a[i][j]<<" ";
            cout<<endl;
        }
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
        for(i=0;i<4;i++) a[i][0]/=a[3][0];
    }
    void Show(FILE *outputFile=NULL) {
        int i;
        if(outputFile!=NULL) {
            for(i=0;i<3;i++) fprintf(outputFile,"%.7lf ",a[i][0]);
            fprintf(outputFile,"\n");
        }
        else {
            for(i=0;i<3;i++) printf("%.7lf ",a[i][0]);
            printf("\n");
        }
    }
};

class Triangle {
public:
    double x[3],y[3],z[3];
    int colorComponent[3];
    Triangle() {}
    Triangle(const Triangle &t) {
        int i;
        for(i=0;i<3;i++) {
            x[i]=t.x[i];
            y[i]=t.y[i];
            z[i]=t.z[i];
            colorComponent[i]=t.colorComponent[i];
        }
    }
    Triangle(Point a,Point b,Point c,int red,int green,int blue) {
        x[0]=a.a[0][0]; y[0]=a.a[1][0]; z[0]=a.a[2][0];
        x[1]=b.a[0][0]; y[1]=b.a[1][0]; z[1]=b.a[2][0];
        x[2]=c.a[0][0]; y[2]=c.a[1][0]; z[2]=c.a[2][0];

        colorComponent[0]=red;
        colorComponent[1]=green;
        colorComponent[2]=blue;
    }
};

void CopyPoint(Point &a,Point &b,Point &c,const Triangle &t) {
    a.a[0][0]=t.x[0]; a.a[1][0]=t.y[0]; a.a[2][0]=t.z[0];
    b.a[0][0]=t.x[1]; b.a[1][0]=t.y[1]; b.a[2][0]=t.z[1];
    c.a[0][0]=t.x[2]; c.a[1][0]=t.y[2]; c.a[2][0]=t.z[2];
}

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
    W=Cross(A,X);
    Xp=X-A*(A.dot(X)-1);
    X=A*(A.dot(X)-1)+Xp*cos(RAD(ang))+W*sin(RAD(ang));
    return X;
}

Matrix GetRotationMatrix(Point A,double ang) {
    Matrix R(4,4);
    Point W,X,Xp;
    int i;

    A.MakeUnit();

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

bool cmpZ(Point a,Point b) {
    return a.a[2][0]>b.a[2][0];
}

Point ZIntersect(Point a,Point b,double z) {
    int i;
    double t=(z-a.a[2][0])/(b.a[2][0]-a.a[2][0]);
    Point ans;
    for(i=0;i<3;i++) ans.a[i][0]=a.a[i][0]+(b.a[i][0]-a.a[i][0])*t;
    ans.a[3][0]=1;
    return ans;
}

void ShowTriangle(Point a,Point b,Point c) {
    a.Show();
    b.Show();
    c.Show();
    cout<<endl;
}

vector<Matrix>transStk;
vector<Triangle>processed;
stack<Triangle>unprocessed;
int color[1000][1000][3],screenW,screenH;
double dx,dy,zBuffer[1000][1000];

void ScanTriangle(Triangle &t) {
    int i,j,k,ySt,yEn,xSt,xEn;
    double st=INF,en=-INF,s,y,zSt,zEn,xStP,ddz,x,z;
    Point a,b,c;
    for(i=0;i<3;i++) {
        t.x[i]+=1;
        t.y[i]+=1;
        t.z[i]+=1;
        st=min(st,t.y[i]);
        en=max(en,t.y[i]);
    }
    st=max(st,0.00);
    en=min(en,2.00);
    ySt=(int)(st/dy);
    yEn=(int)(en/dy);
    for(i=ySt,y=(ySt+0.5)*dy;i<=yEn&&i<screenH;i++,y+=dy) {
        st=INF;
        en=-INF;
        for(j=0;j<3;j++) {
            s=(t.y[NEX(j)]-t.y[j]);
            if(abs(s)<EPS) {
                if((int)(t.y[j]/dy)!=i) continue;
                if(min(t.x[j],t.x[NEX(j)])<st) {
                    st=min(t.x[j],t.x[NEX(j)]);
                    zSt=t.x[j]<t.x[NEX(j)]?t.z[j]:t.z[NEX(j)];
                }
                if(max(t.x[j],t.x[NEX(j)])>en) {
                    en=max(t.x[j],t.x[NEX(j)]);
                    zEn=t.x[j]>t.x[NEX(j)]?t.z[j]:t.z[NEX(j)];
                }
            }
            else {
                s=(y-t.y[j])/s;
                if(s<0||s>1) continue;
                if(t.x[j]+(t.x[NEX(j)]-t.x[j])*s<st) {
                    st=t.x[j]+(t.x[NEX(j)]-t.x[j])*s;
                    zSt=t.z[j]+(t.z[NEX(j)]-t.z[j])*s;
                }
                if(t.x[j]+(t.x[NEX(j)]-t.x[j])*s>en) {
                    en=t.x[j]+(t.x[NEX(j)]-t.x[j])*s;
                    zEn=t.z[j]+(t.z[NEX(j)]-t.z[j])*s;
                }
            }
        }
        if(en>st) ddz=(zEn-zSt)/(en-st);
        else continue;
        xStP=st;
        st=max(st,0.00);
        en=min(en,2.00);
        xSt=(int)(st/dx);
        xEn=(int)(en/dx);
        ddz*=dx;
        for(j=xSt,x=(xSt+0.5)*dx,z=zSt+0.5*ddz;j<=xEn&&j<screenW;j++,x+=dx,z+=ddz)
            if(zBuffer[i][j]>z) {
                zBuffer[i][j]=z;
                for(k=0;k<3;k++)
                    color[i][j][k]=t.colorComponent[k];
            }
    }
}

int main() {
    int i,j,red,green,blue;
    double x,y,z,ang,fovY,fovX,asp,yLim,xLim,near,far;
    string cmd;
    FILE *stage[3];
    Matrix I(4,4),R(4,4);
    Point p[3],pivot,eye,lookAt,r,l,u,pp;
    Triangle t;

    freopen("scene.txt","r",stdin);
    stage[0]=fopen("zStage1.txt","w");
    stage[1]=fopen("zStage2.txt","w");
    stage[2]=fopen("zStage3.txt","w");

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

    cin>>screenW>>screenH;
    cin>>red>>green>>blue;
    for(i=0;i<screenH;i++)
        for(j=0;j<screenW;j++) {
            color[i][j][0]=red;
            color[i][j][1]=green;
            color[i][j][2]=blue;
            zBuffer[i][j]=10;
        }
    dx=2.0/screenW;
    dy=2.0/screenH;
    transStk.push_back(I);

    /***Produce output of 3 stages***/
    while(true) {
        cin>>cmd;
        if(cmd=="triangle") {
            for(i=0;i<3;i++) {
                p[i].Read();
                ApplyTransform(p[i],transStk);
                p[i].Show(stage[0]);
                ApplyTransform(p[i],transStk,1);
                p[i].Show(stage[1]);
            }
            fprintf(stage[0],"\n");
            fprintf(stage[1],"\n");
            cin>>red>>green>>blue;
            sort(p,p+3,cmpZ);
            if(p[2].a[2][0]>-near||p[0].a[2][0]<-far) continue;
            unprocessed.push(Triangle(p[0],p[1],p[2],red,green,blue));
            while(unprocessed.size()) {
                t=unprocessed.top();
                unprocessed.pop();
                CopyPoint(p[0],p[1],p[2],t);
                sort(p,p+3,cmpZ);
                if(p[0].a[2][0]<=-near&&p[2].a[2][0]>=-far) {
                    for(i=0;i<3;i++) {
                        ApplyTransform(p[i],transStk,0);
                        p[i].Show(stage[2]);
                    }
                    fprintf(stage[2],"\n");
                    processed.push_back(Triangle(p[0],p[1],p[2],t.colorComponent[0],t.colorComponent[1],t.colorComponent[2]));
                }
                else {
                    if(p[0].a[2][0]>-near&&p[1].a[2][0]>-near) {
                        unprocessed.push(Triangle(
                                            ZIntersect(p[0],p[2],-near),
                                            ZIntersect(p[1],p[2],-near),
                                            p[2],
                                            t.colorComponent[0],
                                            t.colorComponent[1],
                                            t.colorComponent[2]));
                    }
                    else if(p[0].a[2][0]>-near) {
                        unprocessed.push(Triangle(
                                            ZIntersect(p[0],p[1],-near),
                                            p[1],
                                            p[2],
                                            t.colorComponent[0],
                                            t.colorComponent[1],
                                            t.colorComponent[2]));
                        unprocessed.push(Triangle(
                                            ZIntersect(p[0],p[1],-near),
                                            ZIntersect(p[0],p[2],-near),
                                            p[2],
                                            t.colorComponent[0],
                                            t.colorComponent[1],
                                            t.colorComponent[2]));
                    }
                    else if(p[1].a[2][0]<-far) {
                        unprocessed.push(Triangle(
                                            p[0],
                                            ZIntersect(p[0],p[1],-far),
                                            ZIntersect(p[0],p[2],-far),
                                            t.colorComponent[0],
                                            t.colorComponent[1],
                                            t.colorComponent[2]));
                    }
                    else {
                        unprocessed.push(Triangle(
                                            p[0],
                                            ZIntersect(p[0],p[2],-far),
                                            p[1],
                                            t.colorComponent[0],
                                            t.colorComponent[1],
                                            t.colorComponent[2]));
                        unprocessed.push(Triangle(
                                            ZIntersect(p[0],p[2],-far),
                                            p[1],
                                            ZIntersect(p[1],p[2],-far),
                                            t.colorComponent[0],
                                            t.colorComponent[1],
                                            t.colorComponent[2]));
                    }
                }
            }
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
            pivot.Read();
            R=GetRotationMatrix(pivot,ang);
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

    /***Hidden surface removal**/
    for(i=0;i<processed.size();i++) ScanTriangle(processed[i]);

    /***Produce the image***/
    bitmap_image image(screenW,screenH);
    for(i=0;i<screenH;i++)
        for(j=0;j<screenW;j++)
            image.set_pixel(j,screenH-i-1,color[i][j][0],color[i][j][1],color[i][j][2]);
    image.save_image("zOut.bmp");

    return 0;
}
