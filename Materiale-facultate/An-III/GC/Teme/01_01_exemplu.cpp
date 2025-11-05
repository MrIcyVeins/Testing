#include <windows.h>  // sunt mentionate fisiere (biblioteci) care urmeaza sa fie incluse 
#include <gl/freeglut.h> // nu trebuie uitat freeglut.h (sau glut.h sau gl.h & glu.h)
//#include <cmath> 

void init(void)  // initializare fereastra de vizualizare
{
	glClearColor(1.0, 1.0, 1.0, 0.0); // precizeaza culoarea de fond a ferestrei de vizualizare
	glMatrixMode(GL_PROJECTION);  // se precizeaza este vorba de o reprezentare 2D, realizata prin proiectie ortogonala
	gluOrtho2D(0.0, 400.0, 0.0, 300.0); // sunt indicate coordonatele extreme ale ferestrei de vizualizare
}

//void desenGrila()
//{
//	glColor3f(0.9, 0.9, 0.9); // gri deschis
//	glLineWidth(1);
//	glBegin(GL_LINES);
//	for (int i = 0; i <= 400; i += 20) { // linii verticale
//		glVertex2i(i, 0); glVertex2i(i, 300);
//	}
//	for (int j = 0; j <= 300; j += 20) { // linii orizontale
//		glVertex2i(0, j); glVertex2i(400, j);
//	}
//	glEnd();
//}

void desen(void) // procedura desenare  
{
	glClear(GL_COLOR_BUFFER_BIT); // curata bufferul inainte de desenare
	//desenGrila();  // desen pentru grila

	glColor3f(0, 255, 0); // culoarea punctelor: verde
	glPointSize(5);

	glBegin(GL_POINTS); // reprezinta puncte
		glVertex2i(20, 20);
		glVertex2i(21, 21);
		glVertex2i(22, 22);
		glVertex2i(23, 23);
		glVertex2i(24, 24);
		glVertex2i(100, 100);
	glEnd();

	glColor3f(0, 75, 130); // culoarea liniei frante: mov
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(100, 5);
		glVertex2i(200, 250);
		glVertex2i(300, 5);
	glEnd();

	// Cerinta 3) segmente de dreapta ( data/text )
	glColor3f(0, 0, 0);
    glBegin(GL_LINES);
        // "T"
        glVertex2i(10, 280); glVertex2i(30, 280);   // segment superior
        glVertex2i(20, 280); glVertex2i(20, 250);   // segment mijloc
        // "E"
        glVertex2i(40, 250); glVertex2i(40, 280);   // segment stang
        glVertex2i(40, 280); glVertex2i(60, 280);   // segment superior
        glVertex2i(40, 265); glVertex2i(55, 265);   // segment mijloc
        glVertex2i(40, 250); glVertex2i(60, 250);   // segment jos
        // "M"
        glVertex2i(70, 250); glVertex2i(70, 280);   // segment stanga
        glVertex2i(70, 280); glVertex2i(80, 260);   // diagonala stanga
        glVertex2i(80, 260); glVertex2i(90, 280);   // diagonala dreapta
        glVertex2i(90, 280); glVertex2i(90, 250);   // segment dreapta
        // "A"
        glVertex2i(100, 250); glVertex2i(110, 280); // segment stanga
        glVertex2i(120, 250); glVertex2i(110, 280); // segment dreapta
        glVertex2i(103, 265); glVertex2i(118, 265); // segment mijloc
        // "1"
        glVertex2i(135, 250); glVertex2i(135, 280); // segment mijloc
        glVertex2i(130, 250); glVertex2i(140, 250); // segment inferior
        glVertex2i(135, 280); glVertex2i(132, 270); // diagonala
        // Initiale
        // "A"
        glVertex2i(10, 210); glVertex2i(20, 240);   // segment stang
        glVertex2i(30, 210); glVertex2i(20, 240);   // segment drept
        glVertex2i(14, 225); glVertex2i(26, 225);   // segment mijloc
        // "G"
        glVertex2i(40, 240); glVertex2i(60, 240);   // segment superior
        glVertex2i(40, 240); glVertex2i(40, 210);   // segment stanga
        glVertex2i(40, 210); glVertex2i(60, 210);   // segment inferior
        glVertex2i(50, 225); glVertex2i(60, 225);   // segment mijloc
        glVertex2i(60, 225); glVertex2i(60, 210);   // segment inferior dr

    glEnd();

    // puncte pentru initiale
    glPointSize(3);
    glBegin(GL_POINTS);
            glVertex2i(32, 210);
            glVertex2i(62, 210);
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