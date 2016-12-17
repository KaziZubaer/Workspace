#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define PI (2*acos(0.0))
#define STRIPE 20
#define SLICE 100
#define RAD(x) (((x)*PI)/180)
#define DEG(x) (((x)*180)/PI)

struct point {
	double x,y,z;
};

double cameraHeight,cameraAngle,zoom,inR,outR;
int drawgrid,drawaxes;
struct point zAxis;

struct point Add(struct point a,struct point b) {
	struct point ret;
	ret.x=a.x+b.x;
	ret.y=a.y+b.y;
	ret.z=a.z+b.z;
	return ret;
}

struct point Sub(struct point a,struct point b) {
	struct point ret;
	ret.x=a.x-b.x;
	ret.y=a.y-b.y;
	ret.z=a.z-b.z;
	return ret;
}

double Magnitude(struct point p) {
	return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
}

struct point Scale(struct point p,double c) {
	struct point ret;
	double m=sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
	ret.x=(p.x*c)/m;
	ret.y=(p.y*c)/m;
	ret.z=(p.z*c)/m;
	return ret;
}

struct point Rotate(struct point v,struct point p,double a) {
	struct point n,ret;
	n.x=p.y*v.z-p.z*v.y;
	n.y=p.z*v.x-p.x*v.z;
	n.z=p.x*v.y-p.y*v.x;
	n=Scale(n,sin(a));
	v=Scale(v,cos(a));
	ret=Add(n,v);
	ret=Scale(ret,1);
	return ret;
}

void drawAxes() {
	if(drawaxes==1) {
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES); {
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		} glEnd();
	}
}

void drawGrid() {
	int i;
	if(drawgrid==1) {
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES); {
			for(i=-8;i<=8;i++) {
				
				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		} glEnd();
	}
}

void drawTorus() {
	int i,j;
	double radius=(outR-inR)*0.5,m;
	struct point pts[200][200];
	//Generate points
	for(i=0;i<=SLICE;i++) {
		for(j=0;j<=STRIPE;j++) {
			m=radius*cos(((double)j/(double)STRIPE)*2*PI)+radius+inR;
			pts[i][j].x=1;
			pts[i][j].y=pts[i][j].z=0;
			pts[i][j]=Rotate(pts[i][j],zAxis,((double)i/(double)SLICE)*2*PI);
			pts[i][j]=Scale(pts[i][j],m);
			pts[i][j].z=radius*sin(((double)j/(double)STRIPE)*2*PI);
		}
	}
	//Draw Torus
	for(i=0;i<SLICE;i++) {
		for(j=0;j<STRIPE;j++) {
			if(j&1) glColor3f(1.0,1.0,1.0);
			else glColor3f(0,0,0);
			glBegin(GL_QUADS); {
				glVertex3f(pts[i][j].x,pts[i][j].y,pts[i][j].z);
				glVertex3f(pts[i][j+1].x,pts[i][j+1].y,pts[i][j+1].z);
				glVertex3f(pts[i+1][j+1].x,pts[i+1][j+1].y,pts[i+1][j+1].z);
				glVertex3f(pts[i+1][j].x,pts[i+1][j].y,pts[i+1][j].z);
			} glEnd();
		}
	}
}

void keyboardListener(unsigned char key, int x,int y) {
	switch(key){
		case '1':
			if(inR<outR) inR++;
			break;
		case '2':
			if(inR>0) inR--;
			break;
		case '3':
			outR++;
			break;
		case '4':
			if(outR>inR) outR--;
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y) {
	switch(key) {
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;
		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;
		case GLUT_KEY_PAGE_UP:
			if(zoom>0) zoom--;
			break;
		case GLUT_KEY_PAGE_DOWN:
			zoom++;
			break;
		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y) {	//x, y is the x-y of the screen (2D)
	switch(button) {
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN) {		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN) {		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawgrid=1-drawgrid;
			}
			break;
		default:
			break;
	}
}

void display() {

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
	gluLookAt(zoom*cos(cameraAngle), zoom*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
	drawGrid();
	drawAxes();

    drawTorus();

	glutSwapBuffers();
}

void animate() {
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init() {
	//codes for initialization
	drawgrid=0;
	drawaxes=1;

	cameraHeight=150.0;
	cameraAngle=1.0;
	zoom=200;

	zAxis.x=0;
	zAxis.y=0;
	zAxis.z=1;

	outR=60;
	inR=20;

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
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv) {
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Torus");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
