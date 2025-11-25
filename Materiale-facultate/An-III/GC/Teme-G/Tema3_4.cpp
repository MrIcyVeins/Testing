#include <windows.h>
#include <GL/freeglut.h>

// latura patratului
float side = 30.0f;

// pattern simplu pentru "texturare"
GLubyte stipplePattern[128];

// functie care deseneaza un patrat centrat in origine
void drawSquarePlain()
{
    float h = side / 2.0f;

    glBegin(GL_QUADS);
    glVertex2f(-h, -h);
    glVertex2f(h, -h);
    glVertex2f(h, h);
    glVertex2f(-h, h);
    glEnd();
}

// patrat cu amestec de culori (culori diferite in colturi)
void drawSquareColored()
{
    float h = side / 2.0f;

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); // rosu
    glVertex2f(-h, -h);

    glColor3f(0.0f, 1.0f, 0.0f); // verde
    glVertex2f(h, -h);

    glColor3f(0.0f, 0.0f, 1.0f); // albastru
    glVertex2f(h, h);

    glColor3f(1.0f, 1.0f, 0.0f); // galben
    glVertex2f(-h, h);
    glEnd();
}

// patrat "texturat" cu stipple
void drawSquareTextured()
{
    float h = side / 2.0f;

    glColor3f(0.8f, 0.8f, 0.8f);

    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(stipplePattern);

    glBegin(GL_QUADS);
    glVertex2f(-h, -h);
    glVertex2f(h, -h);
    glVertex2f(h, h);
    glVertex2f(-h, h);
    glEnd();

    glDisable(GL_POLYGON_STIPPLE);
}

void init(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // o fereastra 2D simpla
    gluOrtho2D(-200.0, 200.0, -200.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // umplem patternul pentru stipple (alternanta de linii)
    for (int i = 0; i < 128; i++)
    {
        if (i % 2 == 0)
            stipplePattern[i] = 0xAA; // 10101010
        else
            stipplePattern[i] = 0x55; // 01010101
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 1. PATRATUL DE BAZA – centrat in origine, negru
    glColor3f(0.0f, 0.0f, 0.0f);
    drawSquarePlain();

    // 2. VARIANTA 1: intai SCALARE, apoi TRANSLATIE
    glPushMatrix();
    glTranslatef(60.0f, 60.0f, 0.0f); // translatia
    glScalef(2.0f, 0.5f, 1.0f);         // scalarea
    drawSquareColored();               // patrat cu amestec de culori
    glPopMatrix();

    // 3. VARIANTA 2: intai TRANSLATIE, apoi SCALARE
    glPushMatrix();
    glScalef(2.0f, 0.5f, 1.0f);         // scalarea
    glTranslatef(60.0f, 60.0f, 0.0f); // translatia
    drawSquareTextured();              // patrat texturat
    glPopMatrix();

    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200.0, 200.0, -200.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema3 - transformari = scalare + translatie");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
