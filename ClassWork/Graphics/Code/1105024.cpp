#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <algorithm>
#include <GL/glut.h>
#include "bitmap_image.hpp"
using namespace std;

#define SQTYPE 1
#define TRTYPE 2
#define SPTYPE 3

#define INF 1e18
#define INFCHK 1e17
#define EPS 1e-8
#define PI (2*acos(0.0))
#define RAD(x) (((x)*PI)/180)

#define SLICE 20
#define STACK 20
#define K 300
#define ANGRES 2
#define TRANSRES 5
#define TEXTURE_SIZE 48
#define CHECKER_BOX_SIZE 20
#define CHECKER_BOARD_SIZE 240
#define NEARPLANE 1
#define FARPLANE 1000

int REC_LEVEL=1,SCREEN_SIZE=200,LIGHT_CNT=0,OBJ_CNT=0,TR_CNT=0,SP_CNT=0,SQ_CNT=0;

struct Tuple3 {
	double x,y,z;
};
typedef struct Tuple3 PT,Color,Vec;

struct Triangle {
	PT a,b,c;
};
typedef struct Triangle TR;

struct Square {
	PT pos;
	double a;
};
typedef struct Square SQ;

struct Sphere {
	PT c;
	double r;
};
typedef struct Sphere SP;

struct Object {
	int typ,idx,shiny;
	double amb,diff,spec,refl;
	Vec norm;
	Color cl;
};
typedef Object OB;

SQ squares[100];
SP spheres[100];
TR triangles[100];
OB obj[100];
PT cameraPos,lights[10];
struct Tuple3 dummy;
Vec u,r,l;
Color texture[CHECKER_BOX_SIZE][CHECKER_BOX_SIZE];

void Show(struct Tuple3 a) {
	cout<<a.x<<", "<<a.y<<", "<<a.z<<endl;
}

struct Tuple3 add(struct Tuple3 a,struct Tuple3 b) {
	struct Tuple3 ret;
	ret.x=a.x+b.x;
	ret.y=a.y+b.y;
	ret.z=a.z+b.z;
	return ret;
}

struct Tuple3 sub(struct Tuple3 a,struct Tuple3 b) {
	struct Tuple3 ret;
	ret.x=a.x-b.x;
	ret.y=a.y-b.y;
	ret.z=a.z-b.z;
	return ret;
}

struct Tuple3 mult(struct Tuple3 a,double c) {
	struct Tuple3 ret;
	ret.x=a.x*c;
	ret.y=a.y*c;
	ret.z=a.z*c;
	return ret;
}

Vec scale(Vec v,double c) {
	double m=sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
	Vec ret=mult(v,c/m);
	return ret;
}

PT rotate(Vec v,PT p,double a) {
	PT n,ret;
	n.x=p.y*v.z-p.z*v.y;
	n.y=p.z*v.x-p.x*v.z;
	n.z=p.x*v.y-p.y*v.x;
	n=scale(n,sin(RAD(a)));
	v=scale(v,cos(RAD(a)));
	ret=add(n,v);
	ret=scale(ret,1);
	return ret;
}

Vec cross(Vec a,Vec b) {
	Vec ret;
	ret.x=a.y*b.z-a.z*b.y;
	ret.y=a.z*b.x-a.x*b.z;
	ret.z=a.x*b.y-a.y*b.x;
	return ret;
}

double dot(Vec a,Vec b) {
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

double dist2(PT a,PT b) {
	Vec v=sub(a,b);
	return dot(v,v);
}

double det(PT a,PT b,PT c) {
    PT d=cross(b,c);
    return dot(a,d);
}

bool insideTriangle(PT a,PT b,PT c,PT p) {
    Vec d1,d2;
    double D,x,y;
    d1=sub(b,a);
    d2=sub(c,a);
    p=sub(p,a);
    D=d1.x*d2.y-d2.x*d1.y;
    if(abs(D)<EPS) return false;
    x=(p.x*d2.y-d2.x*p.y)/D;
    y=(d1.x*p.y-p.x*d1.y)/D;
    return x>=-EPS&&y>=-EPS&&x+y<=1+EPS;
}

double intersectLinePlane(PT o,PT d1,PT d2,PT p,PT v) {
    double D;
    PT tempA,tempB,tempC;
    tempA.x=d1.x;tempA.y=d2.x;tempA.z=-v.x;
    tempB.x=d1.y;tempB.y=d2.y;tempB.z=-v.y;
    tempC.x=d1.z;tempC.y=d2.z;tempC.z=-v.z;
    D=det(tempA,tempB,tempC);
    if(abs(D)<EPS) return INF;
    tempA.x=d1.x;tempA.y=d2.x;tempA.z=p.x-o.x;
    tempB.x=d1.y;tempB.y=d2.y;tempB.z=p.y-o.y;
	tempC.x=d1.z;tempC.y=d2.z;tempC.z=p.z-o.z;
    return det(tempA,tempB,tempC)/D;
}

void drawSphere(PT pos,double r,Color cl) {
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	glColor3f(cl.x,cl.y,cl.z);
	glutSolidSphere(r,STACK,SLICE);
	glPopMatrix();
}

void drawTriangle(int idx,Color cl) {
	glColor3f(cl.x,cl.y,cl.z);
	glBegin(GL_TRIANGLES);
	glVertex3f(triangles[idx].a.x,triangles[idx].a.y,triangles[idx].a.z);
	glVertex3f(triangles[idx].b.x,triangles[idx].b.y,triangles[idx].b.z);
	glVertex3f(triangles[idx].c.x,triangles[idx].c.y,triangles[idx].c.z);
	glEnd(); 
}

void drawSquare(PT pos, double a,Color cl) {
	glColor3f(cl.x,cl.y,cl.z);
	glBegin(GL_QUADS); {
		glVertex3f(pos.x-a/2,pos.y-a/2,pos.z);
		glVertex3f(pos.x-a/2,pos.y+a/2,pos.z);
		glVertex3f(pos.x+a/2,pos.y+a/2,pos.z);
		glVertex3f(pos.x+a/2,pos.y-a/2, pos.z);
	} glEnd();
}

void drawChecker() {
	int i,j;
	Color cl;
	PT p;
	for(i=-(CHECKER_BOARD_SIZE/CHECKER_BOX_SIZE);i<(CHECKER_BOARD_SIZE/CHECKER_BOX_SIZE);i++)
		for(j=-(CHECKER_BOARD_SIZE/CHECKER_BOX_SIZE);j<(CHECKER_BOARD_SIZE/CHECKER_BOX_SIZE);j++) {
			p.x=i*CHECKER_BOX_SIZE+CHECKER_BOX_SIZE/2;
			p.y=j*CHECKER_BOX_SIZE+CHECKER_BOX_SIZE/2;
			p.z=0;
			cl.x=cl.y=cl.z=((i&1)^(j&1));
			drawSquare(p,CHECKER_BOX_SIZE,cl);
		}
}

Vec getNormAt(int idx,PT p) {
	if(idx==0) return u;
	if(idx>1&&obj[idx].typ==SPTYPE) {
		Vec v=scale(sub(p,spheres[obj[idx].idx].c),1);
		return v;
	}
	return obj[idx].norm;
}

Color getColorAt(int idx,PT p) {
	if(idx==1) {
		Color ret;
		ret.x=ret.y=ret.z=0;
		int i=(int)(p.x-(-CHECKER_BOARD_SIZE));
		int j=(int)(p.y-(-CHECKER_BOARD_SIZE));
		if(i<0||j<0) return ret;
		i%=CHECKER_BOX_SIZE;
		j%=CHECKER_BOX_SIZE;
		ret=texture[i][j];
		return ret;
	}
	else return obj[idx].cl; 
}

void getIntersectionWith(int idx,PT p,Vec v,double &t,Vec &norm) {
	if(idx==1) {
		if(abs(v.z)<EPS) t=INF;
		else t=(-p.z)/v.z;
		if(t<0) t=INF;
		else if(abs(p.x+v.x*t)>CHECKER_BOARD_SIZE) t=INF;
		else if(abs(p.y+v.y*t)>CHECKER_BOARD_SIZE) t=INF;
		norm=getNormAt(idx,dummy);
	}
	else if(obj[idx].typ==SQTYPE) {
		int id=obj[idx].idx;
		if(abs(v.z)<EPS) t=INF;
		else t=(squares[id].pos.z-p.z)/v.z;
		if(t<0) t=INF;
		else if(abs(p.x+v.x*t-squares[id].pos.x)>squares[id].a/2) t=INF;
		else if(abs(p.y+v.y*t-squares[id].pos.y)>squares[id].a/2) t=INF;
		norm=getNormAt(idx,dummy);
	}
	else if(obj[idx].typ==SPTYPE) {
		int id=obj[idx].idx;
		PT b=sub(p,spheres[id].c);
		double A=dot(v,v),B=2*dot(v,b),C=dot(b,b)-spheres[id].r*spheres[id].r,D,t1,t2;
		D=B*B-4*A*C;
		if(D<EPS) t=INF;
		else {
			t1=(-B+sqrt(D))/(2*A);
			t2=(-B-sqrt(D))/(2*A);
			if(t2<t1) swap(t1,t2);
			if(t2<0) t=INF;
			else if(t1<0) t=t2;
			else t=t1;
			PT iP=add(p,mult(v,t));
			norm=getNormAt(idx,iP);
		}
	}
	else if(obj[idx].typ==TRTYPE) {
		int id=obj[idx].idx;
		t=intersectLinePlane(triangles[id].b,
							sub(triangles[id].a,triangles[id].b),
							sub(triangles[id].c,triangles[id].b),
							p,v);
		if(t<0||t>INFCHK) t=INF;
		else {
			PT iP=add(p,mult(v,t));
			if(!insideTriangle(triangles[id].b,triangles[id].c,triangles[id].a,iP)) t=INF;
			norm=getNormAt(idx,dummy);
		}
	}
	else {
		assert(false);	
	}
}

void getIntersection(int idx,PT p,Vec v,int &nexIdx,PT &iP,Vec &norm) {
	int i;
	double t=INF,tempT;
	Vec tempNorm;
	for(i=1;i<OBJ_CNT;i++) {
		if(i==idx) continue;
		getIntersectionWith(i,p,v,tempT,tempNorm);
		if(tempT<INFCHK&&tempT<t) {
			t=tempT;
			nexIdx=i;
			norm=tempNorm;
		}
	}
	if(t>INFCHK) nexIdx=-1;
	else {
		iP=mult(v,t);
		iP=add(p,iP);
	}
}

Color getColor(int idx,Vec fromDir,PT p,Vec toDir,int level) {
	Color ret,temp;
	int i,nexIdx,tempIdx;
	PT norm=getNormAt(idx,p),nexP,tempP;
	Vec v,nexFromDir,nexToDir,nexNorm,tempNorm;
	double t,cosVal,refl,amb,diff,spec,coeff;
	/**Get Ambient**/
	amb=obj[idx].amb;
	/**Get Diffuse and Spec**/
	if(level>0) {
		diff=0;
		spec=0;
		for(i=0;i<LIGHT_CNT;i++) {
			v=scale(sub(lights[i],p),1);
			getIntersection(idx,p,v,tempIdx,tempP,tempNorm);
			if(tempIdx==-1||dist2(p,tempP)>dist2(p,lights[i])) {
				cosVal=dot(norm,v);
				if(cosVal>0) {
					diff+=cosVal;
					v=sub(mult(norm,2*cosVal),v);
					cosVal=dot(fromDir,v);
					if(cosVal>0) spec+=pow(cosVal,obj[idx].shiny);
				}
			}
		}
		diff*=obj[idx].diff;
		spec*=obj[idx].spec;
		coeff=amb+diff;
	
		ret=getColorAt(idx,p);
		ret=mult(ret,coeff);
	
		ret.x+=spec;
		ret.y+=spec;
		ret.z+=spec;
	}
	
	else {
		ret.x=ret.y=ret.z=0;
	}
	
	/**Get Relfected Ones**/
	getIntersection(idx,p,toDir,nexIdx,nexP,nexNorm);
	cout<<"Intersecting point: ";
	Show(nexP);
	if(nexIdx!=-1&&level<REC_LEVEL) {
		nexFromDir=mult(toDir,-1);
		cosVal=dot(nexFromDir,nexNorm);
		nexToDir=sub(mult(nexNorm,2*cosVal),nexFromDir);
		temp=getColor(nexIdx,nexFromDir,nexP,nexToDir,level+1);
		temp=mult(temp,obj[idx].refl);
		ret=add(ret,temp);
	}
	return ret;
}

void TakePicture() {
	int i,j;
	double ii,jj;
	Color cl;
	Vec tempU,tempR,tempL;
	bitmap_image image(SCREEN_SIZE,SCREEN_SIZE);
    for(i=0;i<SCREEN_SIZE;i++) {
    	ii=i-SCREEN_SIZE/2;
    	ii/=SCREEN_SIZE;
		tempU=scale(u,ii);
		tempU=add(l,tempU);
        for(j=0;j<SCREEN_SIZE;j++) {
			jj=j-SCREEN_SIZE/2;
			jj/=SCREEN_SIZE;
			tempR=scale(r,jj);
			tempL=scale(add(tempU,tempR),1);
			if(abs(tempU.z-l.z)>EPS||abs(tempR.x)>EPS) continue;
			cl=getColor(0,dummy,cameraPos,tempL,0);
			cout<<"Look color: ";
			Show(cl);
            image.set_pixel(SCREEN_SIZE-j-1,SCREEN_SIZE-i-1,min(255.0,K*cl.x),min(255.0,K*cl.y),min(255.0,K*cl.z));
        }
    }
    image.save_image("rayTracedImage.bmp");
}

void keyboardListener(unsigned char key, int x,int y) {
	switch(key){
		case '1':
			l=rotate(l,u,PI/180);
			r=rotate(r,u,PI/180);
			break;
		case '2':
			l=rotate(l,u,-PI/180);
			r=rotate(r,u,-PI/180);
			break;
		case '3':
			u=rotate(u,r,-PI/180);
			l=rotate(l,r,-PI/180);
			break;
		case '4':
			u=rotate(u,r,PI/180);
			l=rotate(l,r,PI/180);
			break;
		case '5':
			u=rotate(u,l,-PI/180);
			r=rotate(r,l,-PI/180);
			break;
		case '6':
			u=rotate(u,l,PI/180);
			r=rotate(r,l,PI/180);
			break;
		case '0':
			Show(cameraPos);
			TakePicture();
			cout<<"Done"<<endl;
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y) {
	switch(key) {
		case GLUT_KEY_DOWN:
			cameraPos=sub(cameraPos,scale(l,TRANSRES));
			break;
		case GLUT_KEY_UP:
			cameraPos=add(cameraPos,scale(l,TRANSRES));
			break;
		case GLUT_KEY_RIGHT:
			cameraPos=sub(cameraPos,scale(r,TRANSRES));
			break;
		case GLUT_KEY_LEFT:
			cameraPos=add(cameraPos,scale(r,TRANSRES));
			break;
		case GLUT_KEY_PAGE_UP:
			cameraPos=add(cameraPos,scale(u,TRANSRES));
			break;
		case GLUT_KEY_PAGE_DOWN:
			cameraPos=sub(cameraPos,scale(u,TRANSRES));
			break;
		default:
			break;
	}
}

void display() {
	int i;
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//Camera
	gluLookAt(	cameraPos.x,cameraPos.y,cameraPos.z, 	
				cameraPos.x+l.x,cameraPos.y+l.y,cameraPos.z+l.z, 
				u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ add your objects from here
	****************************/
	//add objects
	drawChecker();	
	for(i=2;i<OBJ_CNT;i++) {
		if(obj[i].typ==SPTYPE) drawSphere(spheres[obj[i].idx].c,spheres[obj[i].idx].r,obj[i].cl);
		else if(obj[i].typ==SQTYPE) drawSquare(squares[obj[i].idx].pos,squares[obj[i].idx].a,obj[i].cl);
		else if(obj[i].typ==TRTYPE) drawTriangle(obj[i].idx,obj[i].cl);
		else assert(false);
	}
	Color white;
	white.x=white.y=white.z=1;
	for(i=0;i<LIGHT_CNT;i++) drawSphere(lights[i],2,white);
	glutSwapBuffers();
}

void animate() {
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void Triangulate(double h,int signXa,int signYa,int signXb,int signYb) {
	triangles[TR_CNT].a.x=squares[SQ_CNT].pos.x+signXa*squares[SQ_CNT].a/2;
	triangles[TR_CNT].a.y=squares[SQ_CNT].pos.y+signYa*squares[SQ_CNT].a/2;
	triangles[TR_CNT].a.z=squares[SQ_CNT].pos.z;
			
	triangles[TR_CNT].b.x=squares[SQ_CNT].pos.x+signXb*squares[SQ_CNT].a/2;
	triangles[TR_CNT].b.y=squares[SQ_CNT].pos.y+signYb*squares[SQ_CNT].a/2;
	triangles[TR_CNT].b.z=squares[SQ_CNT].pos.z;
			
	triangles[TR_CNT].c=squares[SQ_CNT].pos;
	triangles[TR_CNT].c.z=squares[SQ_CNT].pos.z+h;
			
	obj[OBJ_CNT].cl=obj[OBJ_CNT-1].cl;obj[OBJ_CNT].amb=obj[OBJ_CNT-1].amb;obj[OBJ_CNT].diff=obj[OBJ_CNT-1].diff;
	obj[OBJ_CNT].spec=obj[OBJ_CNT-1].spec;obj[OBJ_CNT].refl=obj[OBJ_CNT-1].refl;obj[OBJ_CNT].shiny=obj[OBJ_CNT-1].shiny;
	obj[OBJ_CNT].typ=TRTYPE;obj[OBJ_CNT].idx=TR_CNT;
	Vec a=sub(triangles[TR_CNT].a,triangles[TR_CNT].b);
	Vec b=sub(triangles[TR_CNT].c,triangles[TR_CNT].b);
	obj[OBJ_CNT].norm=scale(cross(a,b),1);
	TR_CNT++;
	OBJ_CNT++;
}

void PopulateObjects() {
	int i,n;
	string typ;
	double h;
	/**Dummy or eye**/
	obj[0].amb=obj[0].diff=obj[0].spec=0;
	obj[0].refl=1;
	obj[0].shiny=0;
	obj[0].cl.x=obj[0].cl.y=obj[0].cl.y=0;
	/**The checker board**/
	obj[1].amb=obj[1].diff=obj[1].spec=obj[1].refl=0.25;	
	obj[1].norm.x=obj[1].norm.y=0;
	obj[1].norm.z=1;
	obj[1].shiny=1;
	OBJ_CNT=2;
	
	cin>>n;
	for(i=0;i<n;i++) {
		cin>>typ;
		if(typ=="sphere") {	
			cin>>spheres[SP_CNT].c.x>>spheres[SP_CNT].c.y>>spheres[SP_CNT].c.z;
			cin>>spheres[SP_CNT].r;
			cin>>obj[OBJ_CNT].cl.x>>obj[OBJ_CNT].cl.y>>obj[OBJ_CNT].cl.z;
			cin>>obj[OBJ_CNT].amb>>obj[OBJ_CNT].diff>>obj[OBJ_CNT].spec>>obj[OBJ_CNT].refl;
			cin>>obj[OBJ_CNT].shiny;
			obj[OBJ_CNT].typ=SPTYPE;
			obj[OBJ_CNT].idx=SP_CNT++;
			OBJ_CNT++;
		}
		else if(typ=="pyramid") {
			cin>>squares[SQ_CNT].pos.x>>squares[SQ_CNT].pos.y>>squares[SQ_CNT].pos.z;
			cin>>squares[SQ_CNT].a;
			cin>>h;
			cin>>obj[OBJ_CNT].cl.x>>obj[OBJ_CNT].cl.y>>obj[OBJ_CNT].cl.z;
			cin>>obj[OBJ_CNT].amb>>obj[OBJ_CNT].diff>>obj[OBJ_CNT].spec>>obj[OBJ_CNT].refl;
			cin>>obj[OBJ_CNT].shiny;
			obj[OBJ_CNT].typ=SQTYPE;
			obj[OBJ_CNT].idx=SQ_CNT;
			obj[OBJ_CNT].norm.x=0;obj[OBJ_CNT].norm.y=0;obj[OBJ_CNT].norm.z=-1;
			OBJ_CNT++;
			
			Triangulate(h,-1,-1,-1,1);
			Triangulate(h,-1,1,1,1);
			Triangulate(h,1,1,1,-1);
			Triangulate(h,1,-1,-1,-1);
			
			SQ_CNT++;
		}
		else assert(false);
	}
}

void GetTexture() {
	int i,j;
	Color tempText[TEXTURE_SIZE][TEXTURE_SIZE];
	unsigned char _r,_g,_b;
	bitmap_image tex=bitmap_image("texture.bmp");
	for(i=0;i<TEXTURE_SIZE;i++) {
		for(j=0;j<TEXTURE_SIZE;j++) {
			tex.get_pixel(i,j,_r,_g,_b);
			tempText[i][j].x=(double)_r/255.0;
			tempText[i][j].y=(double)_g/255.0;
			tempText[i][j].z=(double)_b/255.0;
		}
	}
	for(i=0;i<CHECKER_BOX_SIZE;i++) {
		for(j=0;j<CHECKER_BOX_SIZE;j++) {
			texture[i][j].x=tempText[(int)((i*1.0/CHECKER_BOX_SIZE)*TEXTURE_SIZE)][(int)((j*1.0/CHECKER_BOX_SIZE)*TEXTURE_SIZE)].x;
			texture[i][j].y=tempText[(int)((i*1.0/CHECKER_BOX_SIZE)*TEXTURE_SIZE)][(int)((j*1.0/CHECKER_BOX_SIZE)*TEXTURE_SIZE)].y;
			texture[i][j].z=tempText[(int)((i*1.0/CHECKER_BOX_SIZE)*TEXTURE_SIZE)][(int)((j*1.0/CHECKER_BOX_SIZE)*TEXTURE_SIZE)].z;
		}
	}
}

void init() {
	//codes for initialization

	cameraPos.x = 20;
	cameraPos.y = 180;
	cameraPos.z = 20;

	l.x=0;
	l.y=-1;
	l.z=0;

	u.x=0;
	u.y=0;
	u.z=1;

	r.x=1;
	r.y=0;
	r.z=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(45, 1, NEARPLANE, FARPLANE);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv) {
	int i;
	
	freopen("description.txt","r",stdin);
	cin>>REC_LEVEL;
	cin>>SCREEN_SIZE;
	PopulateObjects();
	cin>>LIGHT_CNT;
	for(i=0;i<LIGHT_CNT;i++) cin>>lights[i].x>>lights[i].y>>lights[i].z;
	GetTexture();
	
	glutInit(&argc,argv);
	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Ray Trace");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
