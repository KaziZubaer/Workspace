#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define PI (2*acos(0.0))
#define RADIUS 30
#define WIDTH 10
#define RESOLUTION 80
#define ANGRES 3
#define RAD(x) (((x)*PI)/180)
#define DEG(x) (((x)*180)/PI)

struct point {
	double x,y,z;
};

double cameraHeight,cameraAngle,wheelAngle,zoom;
int drawgrid,drawaxes;
struct point pos,front,zAxis;

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

void drawRectangle(double a,double b) {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS); {
		glVertex3f(-a*0.5, -b*0.5, 0);
		glVertex3f(a*0.5, -b*0.5, 0);
		glVertex3f(a*0.5, b*0.5, 0);
		glVertex3f(-a*0.5, b*0.5, 0);
	} glEnd();
}

void drawCylinder(double radius, double length) {
	int i;
	double shade;
	struct point points[200];
	//generate points
	for(i=0;i<=RESOLUTION;i++) {
		points[i].x=radius*cos(((double)i/(double)RESOLUTION)*2*PI+wheelAngle);
		points[i].y=radius*sin(((double)i/(double)RESOLUTION)*2*PI+wheelAngle);
	}
	//draw quads using generated points
	for(i=0;i<RESOLUTION;i++) {
		if(i<RESOLUTION/2) shade=2*(double)i/(double)RESOLUTION;
		else shade=2*(double)(RESOLUTION-i)/(double)RESOLUTION;
        glColor3f(shade,shade,shade);
		glBegin(GL_QUADS); {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,length);
			glVertex3f(points[i].x,points[i].y,length);
		} glEnd();
	}
}

void drawWheel() {
	double ang=acos(front.x);
	if(front.y<0) ang+=2*(PI-ang);

	glTranslatef(pos.x,pos.y,pos.z);
	glRotatef(DEG(ang),0,0,1);
	glPushMatrix(); {
		glTranslatef(0,-WIDTH*0.5,RADIUS);
		glRotatef(-90,1,0,0);
		drawCylinder(RADIUS,WIDTH);
	} glPopMatrix();
	glTranslatef(0,0,RADIUS);
	glRotatef(DEG(wheelAngle),0,1,0);
	drawRectangle(2*RADIUS,WIDTH);
	glRotatef(90,0,1,0);
	drawRectangle(2*RADIUS,WIDTH);
}

void keyboardListener(unsigned char key, int x,int y) {
	switch(key){
		case 'w':
			wheelAngle+=RAD(3);
			if(wheelAngle>2*PI) wheelAngle-=2*PI;
			pos=Add(pos,Scale(front,RADIUS*RAD(3)));
			break;
		case 's':
			wheelAngle-=RAD(3);
			if(wheelAngle<0) wheelAngle+=2*PI;
			pos=Sub(pos,Scale(front,RADIUS*RAD(3)));
			break;
		case 'a':
			front=Rotate(front,zAxis,3);
			break;
		case 'd':
			front=Rotate(front,zAxis,-3);
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

    drawWheel();

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

	pos.x=0;
	pos.y=0;
	pos.z=0;

	front.x=1;
	front.y=0;
	front.z=0;

	wheelAngle=0;
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

	glutCreateWindow("Wheel");

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
