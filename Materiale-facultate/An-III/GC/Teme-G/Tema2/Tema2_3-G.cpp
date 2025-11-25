#include <GL/glut.h>
#include <math.h>

// dimensiune fereastra
int winWidth = 800;
int winHeight = 600;

// display lists
GLuint palmTreeList;
GLuint treasureList;

const float PI = 3.1415926f;

// functie pentru cercuri
void drawCircle(float cx, float cy, float r, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; ++i)
    {
        float a = 2.0f * PI * i / segments;
        float x = cx + r * cosf(a);
        float y = cy + r * sinf(a);
        glVertex2f(x, y);
    }
    glEnd();
}

// Cer
void drawSky()
{
    glBegin(GL_QUADS);
    // jos cer – aproape de linia apei
    glColor3f(0.45f, 0.70f, 0.95f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);

    // sus – mai inchis
    glColor3f(0.15f, 0.35f, 0.75f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // soare
    glColor3f(0.98f, 0.92f, 0.65f);
    drawCircle(-0.7f, 0.75f, 0.12f, 40);
}

//  Insula - partea deasupra apei
void drawIslandAbove()
{
    // nisip deasupra apei
    glColor3f(0.90f, 0.80f, 0.55f);

    glBegin(GL_POLYGON);
    glVertex2f(-0.45f, 0.0f);
    glVertex2f(-0.30f, 0.15f);
    glVertex2f(0.00f, 0.22f);
    glVertex2f(0.30f, 0.15f);
    glVertex2f(0.45f, 0.0f);
    glEnd();

    // detalii nisip
    glColor3f(0.97f, 0.88f, 0.65f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.20f, 0.05f);
    glVertex2f(0.00f, 0.12f);
    glVertex2f(0.18f, 0.05f);
    glVertex2f(0.00f, 0.02f);
    glEnd();
}

// Insula sub apa
void drawIslandUnder()
{
    // baza insulei de sub apa
    glColor3f(0.55f, 0.45f, 0.30f);

    glBegin(GL_POLYGON);
    glVertex2f(-0.55f, 0.0f);
    glVertex2f(-0.75f, -0.40f);
    glVertex2f(-0.60f, -0.80f);
    glVertex2f(0.60f, -0.80f);
    glVertex2f(0.75f, -0.40f);
    glVertex2f(0.55f, 0.0f);
    glEnd();

    // straturi de sedimentatie
    glColor3f(0.40f, 0.32f, 0.22f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.55f, -0.25f);
    glVertex2f(-0.30f, -0.40f);
    glVertex2f(0.10f, -0.45f);
    glVertex2f(0.45f, -0.35f);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.50f, -0.60f);
    glVertex2f(-0.15f, -0.68f);
    glVertex2f(0.25f, -0.65f);
    glVertex2f(0.55f, -0.55f);
    glEnd();
}

// Apa
void drawWaterOverlay()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    // la suprafata – mai deschis
    glColor4f(0.10f, 0.45f, 0.75f, 0.55f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    // in adanc – mai închis
    glColor4f(0.05f, 0.18f, 0.35f, 0.75f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    glDisable(GL_BLEND);

    // linia apei
    glColor3f(0.95f, 0.98f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glEnd();
}

// Palmier
void buildPalmTreeList()
{
    palmTreeList = glGenLists(1);
    glNewList(palmTreeList, GL_COMPILE);

    // trunchi inclinat
    glColor3f(0.55f, 0.35f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(-0.03f, 0.0f);
    glVertex2f(0.03f, 0.0f);
    glVertex2f(0.06f, 0.5f);
    glVertex2f(0.00f, 0.5f);
    glEnd();

    // frunze – triunghiuri
    glColor3f(0.10f, 0.55f, 0.25f);
    glBegin(GL_TRIANGLES);
    // frunza sus
    glVertex2f(0.03f, 0.50f);
    glVertex2f(-0.18f, 0.68f);
    glVertex2f(0.24f, 0.70f);

    // stanga-jos
    glVertex2f(0.03f, 0.48f);
    glVertex2f(-0.22f, 0.50f);
    glVertex2f(-0.02f, 0.40f);

    // dreapta-jos
    glVertex2f(0.03f, 0.48f);
    glVertex2f(0.08f, 0.40f);
    glVertex2f(0.26f, 0.52f);

    // frunza în diagonala
    glVertex2f(0.03f, 0.50f);
    glVertex2f(-0.10f, 0.62f);
    glVertex2f(0.02f, 0.60f);
    glEnd();

    glEndList();
}

// Cufar
void buildTreasureList()
{
    treasureList = glGenLists(1);
    glNewList(treasureList, GL_COMPILE);

    // Cufar
    glColor3f(0.40f, 0.22f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, -0.10f);
    glVertex2f(0.15f, -0.10f);
    glVertex2f(0.15f, 0.08f);
    glVertex2f(-0.15f, 0.08f);
    glEnd();

    // Cufar - capac
    glColor3f(0.45f, 0.25f, 0.12f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, 0.08f);
    glVertex2f(0.15f, 0.08f);
    glVertex2f(0.10f, 0.20f);
    glVertex2f(-0.20f, 0.20f);
    glEnd();

    // Margine
    glColor3f(0.75f, 0.65f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, -0.02f);
    glVertex2f(0.15f, -0.02f);
    glVertex2f(0.15f, 0.01f);
    glVertex2f(-0.15f, 0.01f);
    glEnd();

    // Comoara - disc aur
    glColor3f(0.95f, 0.85f, 0.25f);
    drawCircle(-0.05f, 0.05f, 0.03f, 18);
    drawCircle(0.02f, 0.02f, 0.025f, 18);
    drawCircle(0.08f, 0.06f, 0.02f, 18);

    // Monede
    drawCircle(0.16f, -0.05f, 0.015f, 14);
    drawCircle(0.10f, -0.08f, 0.012f, 14);
    drawCircle(-0.10f, -0.09f, 0.012f, 14);

    glEndList();
}

// Cufar + detalii
void drawUnderwaterDetails()
{
    // Cufar + aur
    glPushMatrix();
    glTranslatef(0.05f, -0.45f, 0.0f);
    glScalef(0.7f, 0.7f, 1.0f);
    glCallList(treasureList);
    glPopMatrix();

    // Pietre
    glColor3f(0.30f, 0.30f, 0.35f);
    drawCircle(-0.35f, -0.70f, 0.06f, 20);
    drawCircle(-0.20f, -0.72f, 0.04f, 20);
    drawCircle(0.35f, -0.68f, 0.05f, 20);

    // Bule de aer 
    glColor3f(0.85f, 0.92f, 1.0f);
    drawCircle(0.20f, -0.20f, 0.015f, 16);
    drawCircle(0.22f, -0.10f, 0.010f, 16);
    drawCircle(0.24f, -0.02f, 0.008f, 16);
}

// Afiseaza
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Fundal cer
    drawSky();

    // Sub apa
    drawIslandUnder();
    drawUnderwaterDetails();

    // Strat sub apa (între 0 și -1)
    drawWaterOverlay();

    // Insula
    drawIslandAbove();

    glPushMatrix();
    glTranslatef(0.0f, 0.18f, 0.0f);  // palmier pe insula
    glScalef(0.8f, 0.8f, 1.0f);
    glCallList(palmTreeList);
    glPopMatrix();

    glutSwapBuffers();
}

// Redimensionare
void reshape(int w, int h)
{
    winWidth = w;
    winHeight = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // spatiu 2D: coordonate in [-1,1]
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Initializare
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND); // pentru apa semitransparenta
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    buildPalmTreeList();
    buildTreasureList();
}

// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Tema2 - Punctul 3");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
