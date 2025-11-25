/* O mini-animatie.
Elemente de noutate:
- functii specifice pentru miscare (glTranslate, glRotate, glScale)
- stivele de matrice (GL_MODELVIEW, GL_PROJECTION) si manevrarea acestora (glPush, glPop)
- trebuie "incarcata" matricea identica in vf. stivei (glLoadIdentity())
- importante: glutPostRedisplay(); glutIdleFunc() - pentru "tranzitii" line
*/
#include<windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>

static GLdouble i = 0.0;
static GLdouble j = 0.0;
static GLdouble alpha = 0.1;
static GLdouble step = 0.1;
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}

void dreptunghi(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // in varful stivei este incarcata matricea identica

	// Deplasare pe OY
	glTranslated(400.0, i, 0.0); // generata o matrice 4x4 T, corespunzatoare translatiei, inm. cu id

	glPushMatrix(); // in stiva: 2 matrice, ambele T
	glRotated(j, 0.0, 0.0, 1.0); // generata o matrice 4x4 R, corespunzatoare rotatiei in stiva: varf: T*R, pozitia 2: T
	glColor3f(1.0, 0.0, 0.0);
	glRecti(-5, 30, 5, 40); // dreptunghiul rosu (asupra sa actioneaza rotatie, apoi translatie)
	glPopMatrix(); // in varful stivei: T
	glColor3f(0.0, 0.0, 1.0);
	glRecti(-20, -12, 20, 12); // dreptunghiul albastru, asupra sa actioneaza translatie
	glPopMatrix();
	glutSwapBuffers();

	glFlush();
}

void dreptunghi2(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // comentati(!)
	// glPushMatrix ( );

	// deplasare pe OY
	glTranslated(400.0, i, 0.0); // generata o matrice 4x4 T, corespunzatoare translatiei

	glPushMatrix(); // in stiva: 2 matrice, ambele T
	glRotated(j, 0.0, 0.0, 1.0); // generata o matrice 4x4 R, corespunzatoare rotatiei
	// in stiva: varf: T*R, pozitia 2: T
	glColor3f(1.0, 0.0, 0.0);
	glRecti(-5, 30, 5, 40); // dreptunghiul rosu (asupra sa actioneaza rotatie, apoi translatie)
	glPopMatrix(); // in varful stivei: T
	
	// glPopMatrix ( ); // decomentat acest glPopMatrix + mai sus si mai jos glPush, glTranslate
	// glPushMatrix ( );
	// glTranslated (i, 200.0, 0.0);  // varianta echivalenta, miscarea ptr dr. albastru
	
	glColor3f(0.0, 0.0, 1.0);
	glRecti(-20, -12, 20, 12); // dreptunghiul albastru, asupra sa actioneaza translatie
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0, 780.0, 0.0, 600.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void miscad(void)
{
	i = i + step;
	if (i > 550.0)
		step = -alpha;
	else if (i < 0.0)
		step = alpha;
	j = j + step;
	glutPostRedisplay();
}

void miscas(void)
{
	i = i + step;
	if (i < 0.0)
		step = alpha;
	else if (i > 550.0)
		step = -alpha;
	j = j + step;
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			step = -alpha; glutIdleFunc(miscas);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			step = alpha; glutIdleFunc(miscad);
		break;
	default:
		break;
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tema 3 - Dreptunghi cu satelit");
	init();
	glutDisplayFunc(dreptunghi);
	//glutDisplayFunc(dreptunghi2);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMainLoop();
}