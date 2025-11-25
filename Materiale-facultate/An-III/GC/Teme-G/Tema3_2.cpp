#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>

#define PI 3.1415926f

// pozitie pe axa Ox
float xPos = -200.0f;
// unghi rotatie
float angle = 0.0f;
// directia (1 = dreapta, -1 = stanga)
int dir = 1;

// viteze
float speed = 0.03f;     // deplasare
float rotSpeed = 0.10f;  // rotire

// dimensiunea patratului
float side = 20.0f;

// linia pe care merge patratul
float groundY = -30.0f;

// limitele ferestrei logice
float leftX = -240.0f;
float rightX = 240.0f;

void init()
{
    glClearColor(1, 1, 1, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(leftX, rightX, -80.0, 80.0);

    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // linia (axa)
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(leftX, groundY);
    glVertex2f(rightX, groundY);
    glEnd();

    // calcul patrat pe linie
    float a = side / 2.0f;
    float rad = angle * PI / 180.0f;
    float c = cosf(rad);
    float s = sinf(rad);

    // cei 4 colturi in centru (0,0)
    float x1 = -a, y1 = -a;
    float x2 = a, y2 = -a;
    float x3 = a, y3 = a;
    float x4 = -a, y4 = a;

    // rotim si cautam y minim
    float yr1 = x1 * s + y1 * c;
    float yr2 = x2 * s + y2 * c;
    float yr3 = x3 * s + y3 * c;
    float yr4 = x4 * s + y4 * c;

    float minY = yr1;
    if (yr2 < minY) minY = yr2;
    if (yr3 < minY) minY = yr3;
    if (yr4 < minY) minY = yr4;

    // vrem ca minY sa fie chiar groundY
    float yOffset = groundY - minY;

    glPushMatrix();
    glTranslatef(xPos, yOffset, 0);
    glRotatef(angle, 0, 0, 1);

    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-a, -a);
    glVertex2f(a, -a);
    glVertex2f(a, a);
    glVertex2f(-a, a);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

// miscare + rotire
void idleFunc()
{
    // deplasare
    xPos = xPos + dir * speed;

    float minX = leftX + side / 2.0f;
    float maxX = rightX - side / 2.0f;

    // schimbam directia la margini
    if (xPos > maxX)
    {
        xPos = maxX;
        dir = -1;
    }
    else if (xPos < minX)
    {
        xPos = minX;
        dir = 1;
    }

    // rotire: directia depinde de dir
    // spre dreapta -> unghi scade
    // spre stanga  -> unghi creste
    angle = angle - dir * rotSpeed;

    if (angle > 360.0f) angle -= 360.0f;
    if (angle < 0.0f)   angle += 360.0f;

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema3 - Patrat rostogolire");

    init();
    glutDisplayFunc(display);
    glutIdleFunc(idleFunc);

    glutMainLoop();
    return 0;
}
