#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>

// dimensiunea ferestrei
int latimeFereastra  = 800;
int inaltimeFereastra = 600;

// axa X (linie rosie) la Y constant
float yAxa = 100.0f;

// patrat
float laturaPatrat = 40.0f;
float jumatateLatura;

// pentru rostogolire folosim raza cercului
float razaPatrat;          
float micOffset = 2.0f;    // ridicare peste axa

// pozitia centrului patratului pe X si Y
float pozX;
float pozY;

// unghiul de rotatie in grade
float unghi = 0.0f;

// directia: +1 spre dreapta, -1 spre stanga
int directie = 1;

// viteza
float viteza = 0.05f;

// factor pentru calculul unghiului in functie de distanta parcursa
float factorUnghi;


void initializare()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)latimeFereastra, 0.0, (GLdouble)inaltimeFereastra);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    jumatateLatura = laturaPatrat / 2.0f;

    // raza
    razaPatrat = jumatateLatura * 1.4142135f;

    // centrul patratului: deasupra axei X
    pozY = yAxa + razaPatrat + micOffset;
    pozX = jumatateLatura + 20.0f;

    // factorul care leaga distanta de rotatie (s / R), transformat in grade
    factorUnghi = 180.0f / (3.1415926f * razaPatrat);
}

void deseneazaAxaX()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
        glVertex2f(0.0f, yAxa);
        glVertex2f((float)latimeFereastra, yAxa);
    glEnd();

    glLineWidth(1.0f);
}

void deseneazaPatrat()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-jumatateLatura, -jumatateLatura);
        glVertex2f( jumatateLatura, -jumatateLatura);
        glVertex2f( jumatateLatura,  jumatateLatura);
        glVertex2f(-jumatateLatura,  jumatateLatura);
    glEnd();
}

void afisare()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // axa X
    deseneazaAxaX();

    // patratul care se rostogoleste
    glPushMatrix();
        glTranslatef(pozX, pozY, 0.0f);
        glRotatef(unghi, 0.0f, 0.0f, 1.0f);
        deseneazaPatrat();
    glPopMatrix();

    glutSwapBuffers();
}

void redimensionare(int w, int h)
{
    latimeFereastra  = w;
    inaltimeFereastra = h;

    glViewport(0, 0, (GLsizei)latimeFereastra, (GLsizei)inaltimeFereastra);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)latimeFereastra, 0.0, (GLdouble)inaltimeFereastra);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void animatie()
{
    // deplasare pe X
    float dx = directie * viteza;
    pozX += dx;

    // limitele astfel incat patratul sa nu iasa din ecran
    float minX = jumatateLatura;
    float maxX = (float)latimeFereastra - jumatateLatura;

    // se loveste de perete si se intoarce
    if (pozX > maxX)
    {
        pozX = maxX;
        directie = -1;
    }
    else if (pozX < minX)
    {
        pozX = minX;
        directie = 1;
    }

    // rostogolire: legam unghiul de distanta
    // daca mergem spre dreapta, vrem sens clockwise
    // daca mergem spre stanga, invers
    unghi -= dx * factorUnghi;

    if (unghi > 360.0f)  unghi -= 360.0f;
    if (unghi < -360.0f) unghi += 360.0f;

    glutPostRedisplay();
}

void mouseClick(int buton, int stare, int x, int y)
{
    if (buton == GLUT_LEFT_BUTTON && stare == GLUT_DOWN)
    {
        // schimbam directia la fiecare click
        directie = -directie;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(latimeFereastra, inaltimeFereastra);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema3 - rostogolire");

    initializare();
    glutDisplayFunc(afisare);
    glutReshapeFunc(redimensionare);
    glutMouseFunc(mouseClick);
    glutIdleFunc(animatie);

    glutMainLoop();
    return 0;
}
