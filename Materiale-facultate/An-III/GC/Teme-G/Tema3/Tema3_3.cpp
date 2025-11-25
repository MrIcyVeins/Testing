#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>

#define PI     3.1415926f
#define TWO_PI (2.0f * PI)

// dimensiunea ferestrei
int winWidth = 800;
int winHeight = 600;

// liste de display
GLuint starList;    // stea (hexagon galben)
GLuint planetList;  // planeta (cerc albastru)
GLuint moonList;    // satelit (cerc mic gri)

// unghiuri (in grade)
float anglePlanet = 0.0f;
float angleMoon = 0.0f;

// pasi de rotatie
float stepPlanet = 0.004f;  // planeta – lent
float stepMoon = 0.04f;   // satelit – rapid

// raze orbite
float planetRadius = 200.0f;
float moonRadius = 60.0f;

void makeDisplayLists()
{
    int i;
    float t;

    // rezervam 3 liste la rand
    starList = glGenLists(3);
    planetList = starList + 1;
    moonList = starList + 2;

    // STEAUA – hexagon galben, centru in (0,0), raza 40
    glNewList(starList, GL_COMPILE);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (i = 0; i < 6; i++)
    {
        t = TWO_PI * i / 6.0f;
        glVertex2f(40.0f * cosf(t), 40.0f * sinf(t));
    }
    glEnd();
    glEndList();

    // PLANETA – cerc albastru, raza 20
    glNewList(planetList, GL_COMPILE);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (i = 0; i < 40; i++)
    {
        t = TWO_PI * i / 40.0f;
        glVertex2f(20.0f * cosf(t), 20.0f * sinf(t));
    }
    glEnd();
    glEndList();

    // SATELIT – cerc mic gri, raza 8
    glNewList(moonList, GL_COMPILE);
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POLYGON);
    for (i = 0; i < 30; i++)
    {
        t = TWO_PI * i / 30.0f;
        glVertex2f(8.0f * cosf(t), 8.0f * sinf(t));
    }
    glEnd();
    glEndList();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400.0, 400.0, -300.0, 300.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    makeDisplayLists();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // STEAUA in centru
    glPushMatrix();
    glCallList(starList);
    glPopMatrix();

    // PLANETA – orbiteaza in jurul stelei
    glPushMatrix();
    glRotatef(anglePlanet, 0.0f, 0.0f, 1.0f);
    glTranslatef(planetRadius, 0.0f, 0.0f);
    glCallList(planetList);

    // SATELIT – orbiteaza in jurul planetei
    glPushMatrix();
    glRotatef(angleMoon, 0.0f, 0.0f, 1.0f);
    glTranslatef(moonRadius, 0.0f, 0.0f);
    glCallList(moonList);
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

// animatie: modificam doar unghiurile
void animatie()
{
    anglePlanet += stepPlanet;
    if (anglePlanet > 360.0f)
        anglePlanet -= 360.0f;

    angleMoon += stepMoon;
    if (angleMoon > 360.0f)
        angleMoon -= 360.0f;

    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400.0, 400.0, -300.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema 3 - mini sistem solar");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(animatie);

    glutMainLoop();
    return 0;
}
