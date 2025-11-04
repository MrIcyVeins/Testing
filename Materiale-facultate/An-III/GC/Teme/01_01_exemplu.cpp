#include <windows.h>  // sunt mentionate fisiere (biblioteci) care urmeaza sa fie incluse 
#include <gl/freeglut.h> // nu trebuie uitat freeglut.h (sau glut.h sau gl.h & glu.h)

void init(void)  // initializare fereastra de vizualizare
{
	glClearColor(1.0, 1.0, 1.0, 0.0); // precizeaza culoarea de fond a ferestrei de vizualizare
	glMatrixMode(GL_PROJECTION);  // se precizeaza este vorba de o reprezentare 2D, realizata prin proiectie ortogonala
	gluOrtho2D(0.0, 400.0, 0.0, 300.0); // sunt indicate coordonatele extreme ale ferestrei de vizualizare
}
void desen(void) // procedura desenare  
{
	glColor3f(0, 255, 0); // culoarea punctelor: albastru
	glPointSize(5);
	glBegin(GL_POINTS); // reprezinta puncte
		glVertex2i(20, 20);
		glVertex2i(21, 21);
		glVertex2i(22, 22);
		glVertex2i(23, 23);
		glVertex2i(24, 24);
		glVertex2i(100, 100);
	glEnd();

	glColor3f(75, 0, 130); // culoarea liniei frante: rosu
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(100, 10);
		glVertex2i(200, 250);
		glVertex2i(300, 10);
	glEnd();

	glFlush(); // proceseaza procedurile OpenGL cat mai rapid
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv); // initializare GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // se utilizeaza un singur buffer | modul de colorare RedGreenBlue (= default)
	glutInitWindowPosition(0, 0); // pozitia initiala a ferestrei de vizualizare (in coordonate ecran)
	glutInitWindowSize(1200, 600); // dimensiunile ferestrei 
	glutCreateWindow("Puncte & Segmente"); // creeaza fereastra, indicand numele ferestrei de vizualizare - apare in partea superioara
	init(); // executa procedura de initializare
	glClear(GL_COLOR_BUFFER_BIT); // reprezentare si colorare fereastra de vizualizare
	glutDisplayFunc(desen); // procedura desen este invocata ori de cate ori este nevoie
	glutMainLoop(); // ultima instructiune a programului, asteapta (eventuale) noi date de intrare
}