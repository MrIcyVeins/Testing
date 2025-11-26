#include <GL/freeglut.h>
#include <math.h>

// Variabile
float unghiPlaneta = 0.0f;
float unghiSatelit = 0.0f;

// Viteze rotatie
float pasPlaneta = 0.02f;    // viteza rotatie planeta
float pasSatelit = 0.05f;    // viteza rotatie satelit

// Liste
GLuint listaStea;
GLuint listaPlaneta;
GLuint listaSatelit;

// Raza orbitelor
float razaPlaneta = 150.0f;
float razaSatelit = 40.0f;

// Initializare
void init(void)
{
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400.0f, 400.0f, -300.0f, 300.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Liste display
    listaStea = glGenLists(1);
    glNewList(listaStea, GL_COMPILE);
    glColor3f(1.0f, 1.0f, 0.2f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 30; i++)
    {
        float a = i * 2.0f * 3.1415926f / 30;
        glVertex2f(30.0f * cosf(a), 30.0f * sinf(a));
    }
    glEnd();
    glEndList();

    listaPlaneta = glGenLists(1);
    glNewList(listaPlaneta, GL_COMPILE);
    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 30; i++)
    {
        float a = i * 2.0f * 3.1415926f / 30;
        glVertex2f(15.0f * cosf(a), 15.0f * sinf(a));
    }
    glEnd();
    glEndList();

    listaSatelit = glGenLists(1);
    glNewList(listaSatelit, GL_COMPILE);
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 20; i++)
    {
        float a = i * 2.0f * 3.1415926f / 20;
        glVertex2f(6.0f * cosf(a), 6.0f * sinf(a));
    }
    glEnd();
    glEndList();
}

// Display
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Stea
    glPushMatrix();
    glCallList(listaStea);
    glPopMatrix();

    // Planeta
    glPushMatrix();
    glRotatef(unghiPlaneta, 0.0f, 0.0f, 1.0f);
    glTranslatef(razaPlaneta, 0.0f, 0.0f);
    glCallList(listaPlaneta);

    // Satelit
    glPushMatrix();
    glRotatef(unghiSatelit, 0.0f, 0.0f, 1.0f);
    glTranslatef(razaSatelit, 0.0f, 0.0f);
    glCallList(listaSatelit);
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

void idle(void)
{
    unghiPlaneta += pasPlaneta;
    unghiSatelit += pasSatelit;

    if (unghiPlaneta > 360.0f) unghiPlaneta -= 360.0f;
    if (unghiSatelit > 360.0f) unghiSatelit -= 360.0f;

    glutPostRedisplay();
}

// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tema3 - mini sistem solar");

    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
