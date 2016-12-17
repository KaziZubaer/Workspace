#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define PI (2*acos(0.0))
#define MAX 25
#define RESOLUTION 20
#define ANGRES 2
#define RAD(x) (((x)*PI)/180)

struct point {
	double x,y,z;
};

int drawgrid,drawaxes;
double cubeRad;
struct point cameraPos,u,r,l;

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
	n=Scale(n,sin(RAD(a)));
	v=Scale(v,cos(RAD(a)));
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
				if(i==0) continue;	//SKIP the MAIN axes
				
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

void drawSquare(double a, struct point pos) {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS); {
		glVertex3f(pos.x, pos.y, pos.z);
		glVertex3f(pos.x+a, pos.y, pos.z);
		glVertex3f(pos.x+a, pos.y+a, pos.z);
		glVertex3f(pos.x, pos.y+a, pos.z);
	} glEnd();
}

void drawFourthCylinder(double radius, double length) {
	int i,translate = MAX-cubeRad;
	struct point points[100];
	//generate points
	for(i=0;i<=RESOLUTION;i++) {
		points[i].x=radius*cos(((double)i/(double)RESOLUTION)*0.5*PI);
		points[i].y=radius*sin(((double)i/(double)RESOLUTION)*0.5*PI);
	}
	//draw quads using generated points
	for(i=0;i<RESOLUTION;i++) {
        glColor3f((double)i/(double)RESOLUTION,(double)i/(double)RESOLUTION,(double)i/(double)RESOLUTION);
		glBegin(GL_QUADS); {
			glVertex3f(points[i].x+translate,points[i].y+translate,0-translate);
			glVertex3f(points[i+1].x+translate,points[i+1].y+translate,0-translate);
			glVertex3f(points[i+1].x+translate,points[i+1].y+translate,length-translate);
			glVertex3f(points[i].x+translate,points[i].y+translate,length-translate);
		} glEnd();
	}
}

void drawEighthSphere(double radius) {
	struct point points[100][100];
	int i,j,translate = MAX-cubeRad;
	double h,r;
	//generate points
	for(i=0;i<=RESOLUTION;i++) {
		h=radius*sin(((double)i/(double)RESOLUTION)*0.5*PI);
		r=radius*cos(((double)i/(double)RESOLUTION)*0.5*PI);
		for(j=0;j<=RESOLUTION;j++) {
			points[i][j].x=r*cos(((double)j/(double)RESOLUTION)*0.5*PI);
			points[i][j].y=r*sin(((double)j/(double)RESOLUTION)*0.5*PI);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<RESOLUTION;i++) {
        glColor3f((double)i/(double)RESOLUTION,(double)i/(double)RESOLUTION,(double)i/(double)RESOLUTION);
		for(j=0;j<RESOLUTION;j++) {
			glBegin(GL_QUADS); {
				glVertex3f(points[i][j].x+translate,points[i][j].y+translate,points[i][j].z+translate);
				glVertex3f(points[i][j+1].x+translate,points[i][j+1].y+translate,points[i][j+1].z+translate);
				glVertex3f(points[i+1][j+1].x+translate,points[i+1][j+1].y+translate,points[i+1][j+1].z+translate);
				glVertex3f(points[i+1][j].x+translate,points[i+1][j].y+translate,points[i+1][j].z+translate);
			} glEnd();
		}
	}
}

void drawSphereCube() {
	struct point squarePos;
	squarePos.x = -(MAX-cubeRad);
	squarePos.y = -(MAX-cubeRad);
	squarePos.z = MAX;

	//Draw squares
	drawSquare(2*(MAX-cubeRad), squarePos);
	glPushMatrix(); {
        glRotatef(90,1,0,0);
		drawSquare(2*(MAX-cubeRad), squarePos);
		glRotatef(90,1,0,0);
		drawSquare(2*(MAX-cubeRad), squarePos);
		glRotatef(90,1,0,0);
		drawSquare(2*(MAX-cubeRad), squarePos);
    } glPopMatrix();
    glPushMatrix(); {
		glRotatef(90,0,1,0);
		drawSquare(2*(MAX-cubeRad), squarePos);
		glRotatef(180,1,0,0);
		drawSquare(2*(MAX-cubeRad), squarePos);
	} glPopMatrix();

	//Draw cylinders
	drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
	drawEighthSphere(cubeRad);
	glPushMatrix(); {
		glRotatef(90,0,0,1);
		drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
		drawEighthSphere(cubeRad);
		glRotatef(90,0,0,1);
		drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
		drawEighthSphere(cubeRad);
		glRotatef(90,0,0,1);
		drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
		drawEighthSphere(cubeRad);
	} glPopMatrix();

	glPushMatrix(); {
		glRotatef(90,0,1,0);
		drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
		drawEighthSphere(cubeRad);
		glPushMatrix(); {
			glRotatef(90,1,0,0);
			drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
			drawEighthSphere(cubeRad);
			glRotatef(90,1,0,0);
			drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
			drawEighthSphere(cubeRad);
			glRotatef(90,1,0,0);
			drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
			drawEighthSphere(cubeRad);
		} glPopMatrix();
	} glPopMatrix();
	
	glPushMatrix(); {
		glRotatef(270,0,1,0);
		drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
		glPushMatrix(); {
			glRotatef(90,1,0,0);
			drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
			glRotatef(90,1,0,0);
			drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
			glRotatef(90,1,0,0);
			drawFourthCylinder(cubeRad, 2*(MAX-cubeRad));
		} glPopMatrix();
	} glPopMatrix();
}

void keyboardListener(unsigned char key, int x,int y) {
	switch(key){
		case '1':
			l=Rotate(l,u,ANGRES);
			r=Rotate(r,u,ANGRES);
			break;
		case '2':
			l=Rotate(l,u,-ANGRES);
			r=Rotate(r,u,-ANGRES);
			break;
		case '3':
			u=Rotate(u,r,-ANGRES);
			l=Rotate(l,r,-ANGRES);
			break;
		case '4':
			u=Rotate(u,r,ANGRES);
			l=Rotate(l,r,ANGRES);
			break;
		case '5':
			u=Rotate(u,l,-ANGRES);
			r=Rotate(r,l,-ANGRES);
			break;
		case '6':
			u=Rotate(u,l,ANGRES);
			r=Rotate(r,l,ANGRES);
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y) {
	switch(key) {
		case GLUT_KEY_DOWN:
			cameraPos=Sub(cameraPos,l);
			break;
		case GLUT_KEY_UP:
			cameraPos=Add(cameraPos,l);
			break;
		case GLUT_KEY_RIGHT:
			cameraPos=Sub(cameraPos,r);
			break;
		case GLUT_KEY_LEFT:
			cameraPos=Add(cameraPos,r);
			break;
		case GLUT_KEY_PAGE_UP:
			cameraPos=Add(cameraPos,u);
			break;
		case GLUT_KEY_PAGE_DOWN:
			cameraPos=Sub(cameraPos,u);
			break;
		case GLUT_KEY_INSERT:
			break;
		case GLUT_KEY_HOME:
			if(cubeRad<MAX) cubeRad++;
			break;
		case GLUT_KEY_END:
			if(cubeRad>0) cubeRad--;
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
	gluLookAt(	cameraPos.x,cameraPos.y,cameraPos.z, 	
				cameraPos.x+l.x,cameraPos.y+l.y,cameraPos.z+l.z, 
				u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
	drawAxes();
	drawGrid();

    drawSphereCube();

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
	cubeRad=10;

	cameraPos.x = 0;
	cameraPos.y = 100;
	cameraPos.z = 0;

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

	glutCreateWindow("Sphere To Cube");

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
