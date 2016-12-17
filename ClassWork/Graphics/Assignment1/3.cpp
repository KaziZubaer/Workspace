#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define PI (2*acos(0.0))
#define RADIUSARM 10
#define SLICEARM 15
#define STACKARM 15
#define PALMW 20
#define PALMH 15
#define RADIUSFING 2
#define SLICEFING 5
#define STACKFING 10
#define RAD(x) (((x)*PI)/180)
#define DEG(x) (((x)*180)/PI)

double cameraHeight,cameraAngle,zoom,superArmFront,superArmSide,foreArm,finger[3];
int drawgrid,drawaxes;

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

void drawHand() {
	glRotatef(superArmSide,0,1,0);
	glRotatef(superArmFront,1,0,0);
	glTranslatef(0,0,-3*RADIUSARM);
	glPushMatrix(); {
		glScalef(1,1,3);
		glutWireSphere(RADIUSARM,SLICEARM,STACKARM);
	} glPopMatrix();

	glTranslatef(0,0,-3*RADIUSARM);
	glRotatef(foreArm,1,0,0);
	glTranslatef(0,0,-2*RADIUSARM);
	glPushMatrix(); {
		glScalef(1,1,2);
		glutWireSphere(RADIUSARM,SLICEARM,STACKARM);
	} glPopMatrix();

	glTranslatef(0,0,-2*RADIUSARM);
	glBegin(GL_TRIANGLES); {
		glVertex3f(0,0,0);
		glVertex3f(-PALMW*0.5,0,-PALMH);
		glVertex3f(PALMW*0.5,0,-PALMH);
	} glEnd();

	glTranslatef(0,0,-PALMH);
	glPushMatrix(); {
		glRotatef(finger[0],1,0,0);
		glTranslatef(-PALMW*0.5,0,-2*RADIUSFING);
		glScalef(1,1,2);	
		glutWireSphere(RADIUSFING,SLICEFING,STACKFING);	
	} glPopMatrix();
	glPushMatrix(); {
		glRotatef(finger[1],1,0,0);
		glTranslatef(0,0,-2*RADIUSFING);
		glScalef(1,1,2);	
		glutWireSphere(RADIUSFING,SLICEFING,STACKFING);	
	} glPopMatrix();
	glRotatef(finger[2],1,0,0);
	glTranslatef(PALMW*0.5,0,-2*RADIUSFING);
	glScalef(1,1,2);	
	glutWireSphere(RADIUSFING,SLICEFING,STACKFING);	
}

void keyboardListener(unsigned char key, int x,int y) {
	switch(key){
		case 'q':
			if(superArmSide<90) superArmSide+=3;
			break;
		case 'w':
			if(superArmSide>0) superArmSide-=3;
			break;
		case '1':
			if(superArmFront>-45) superArmFront-=3;
			break;
		case '2':
			if(superArmFront<45) superArmFront+=3;
			break;
		case '3':
			if(foreArm>-90) foreArm-=3;
			break;
		case '4':
			if(foreArm<0) foreArm+=3;
			break;
		case '5':
			if(finger[0]>-90) finger[0]-=3;
			break;
		case '6':
			if(finger[0]<0) finger[0]+=3;
			break;
		case '7':
			if(finger[1]>-90) finger[1]-=3;
			break;
		case '8':
			if(finger[1]<0) finger[1]+=3;
			break;
		case '9':
			if(finger[2]>-90) finger[2]-=3;
			break;
		case '0':
			if(finger[2]<0) finger[2]+=3;
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

    drawHand();

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

	finger[0]=0;
	finger[1]=0;
	finger[2]=0;

	foreArm=0;

	superArmSide=0;
	superArmFront=0;

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

	glutCreateWindow("Hand");

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
