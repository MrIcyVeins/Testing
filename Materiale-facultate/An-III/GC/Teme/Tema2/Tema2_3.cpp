#include <GL/glut.h>
#include <math.h>

// dimensiune fereastra
int winWidth = 900, winHeight = 600;

// display lists
GLuint listaCladire;
GLuint listaLampa;

// functie text
void drawText(float x, float y, const char* t)
{
    glRasterPos2f(x, y);
    while (*t)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *t++);
}

// functie ajutatoare: cerc 2D (TRIANGLE_FAN)
void drawCircle(float cx, float cy, float r, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; ++i)
    {
        float a = 2.0f * 3.1415926f * i / segments;
        glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
    }
    glEnd();
}

// lista pentru cladire
void buildlistaCladire()
{
    listaCladire = glGenLists(1);
    glNewList(listaCladire, GL_COMPILE);

    // corp cladire
    glColor3f(0.25f, 0.28f, 0.40f);
    glBegin(GL_QUADS);
        glVertex2f(-0.04f, 0.0f);
        glVertex2f( 0.04f, 0.0f);
        glVertex2f( 0.04f, 0.5f);
        glVertex2f(-0.04f, 0.5f);
    glEnd();

    // ferestre
    glColor3f(1.0f, 0.95f, 0.7f);
    float w = 0.015f, h = 0.03f;

    for (float y = 0.08f; y < 0.45f; y += 0.10f)
    {
        glBegin(GL_QUADS);
            glVertex2f(-0.03f, y);
            glVertex2f(-0.03f + w, y);
            glVertex2f(-0.03f + w, y + h);
            glVertex2f(-0.03f, y + h);
        glEnd();

        glBegin(GL_QUADS);
            glVertex2f(0.03f - w, y);
            glVertex2f(0.03f, y);
            glVertex2f(0.03f, y + h);
            glVertex2f(0.03f - w, y + h);
        glEnd();
    }

    glEndList();
}

// lista pentru felinar
void buildlistaLampa()
{
    listaLampa = glGenLists(1);
    glNewList(listaLampa, GL_COMPILE);

    // stalp
    glColor3f(0.8f, 0.8f, 0.9f);
    glBegin(GL_LINES);
        glVertex2f(0.0f, -0.1f);
        glVertex2f(0.0f,  0.1f);
    glEnd();

    // bara orizontala
    glBegin(GL_LINES);
        glVertex2f(-0.03f, 0.09f);
        glVertex2f( 0.03f, 0.09f);
    glEnd();

    // bec
    glColor3f(1.0f, 0.95f, 0.8f);
    drawCircle(0.0f, 0.11f, 0.015f, 20);

    glEndList();
}

// cer + stele + luna
void desenCer()
{
    // Cer
    glBegin(GL_QUADS);
        glColor3f(0.20f, 0.25f, 0.45f); // jos
        glVertex2f(-1.0f, 0.0f);
        glVertex2f( 1.0f, 0.0f);
        glColor3f(0.10f, 0.15f, 0.30f); // sus
        glVertex2f( 1.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
    glEnd();

    // stele
    glPointSize(2.0f);
    glColor3f(0.95f, 0.95f, 1.0f);
    glBegin(GL_POINTS);
        glVertex2f(-0.9f, 0.9f);
        glVertex2f(-0.7f, 0.8f);
        glVertex2f(-0.5f, 0.85f);
        glVertex2f(-0.2f, 0.82f);
        glVertex2f( 0.0f, 0.88f);
        glVertex2f( 0.3f, 0.86f);
        glVertex2f( 0.6f, 0.9f);
        glVertex2f( 0.8f, 0.83f);
        glVertex2f( 0.9f, 0.87f);
    glEnd();

    // luna
    glColor3f(0.98f, 0.97f, 0.85f);
    drawCircle(-0.7f, 0.75f, 0.10f, 40);
}

// oras (teren + cladiri)
void desenOras()
{
    // banda
    glBegin(GL_QUADS);
        glColor3f(0.15f, 0.15f, 0.25f);
        glVertex2f(-1.0f, -0.05f);
        glVertex2f( 1.0f, -0.05f);
        glVertex2f( 1.0f,  0.0f);
        glVertex2f(-1.0f,  0.0f);
    glEnd();

    // Sir cladiri
    for (float x = -1.05f; x <= 1.05f; x += 0.12f)
    {
        glPushMatrix();
            glTranslatef(x, -0.05f, 0.0f);

            // inaltime usor variata
            float hScale = 0.6f + 0.4f * (0.5f + 0.5f * sinf(5.0f * x));
            glScalef(1.0f, hScale, 1.0f);

            glCallList(listaCladire);
        glPopMatrix();
    }
}

// Trotuar
void desenTrotuar()
{
    // banda trotuar
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.6f);
        glVertex2f(-1.0f, -0.35f);
        glVertex2f( 1.0f, -0.35f);
        glVertex2f( 1.0f, -0.25f);
        glVertex2f(-1.0f, -0.25f);
    glEnd();

    // balustrada
    glBegin(GL_QUADS);
        glColor3f(0.35f, 0.35f, 0.45f);
        glVertex2f(-1.0f, -0.25f);
        glVertex2f( 1.0f, -0.25f);
        glVertex2f( 1.0f, -0.20f);
        glVertex2f(-1.0f, -0.20f);
    glEnd();
}

// Felinare
void desenLampi()
{
    for (float x = -1.0f; x <= 1.0f; x += 0.25f)
    {
        glPushMatrix();
            glTranslatef(x, -0.20f, 0.0f);
            glScalef(0.4f, 0.4f, 1.0f);
            glCallList(listaLampa);
        glPopMatrix();
    }
}

// Apa
void desenApa()
{
    glBegin(GL_QUADS);
        glColor3f(0.10f, 0.20f, 0.40f); // sus
        glVertex2f(-1.0f, -0.35f);
        glVertex2f( 1.0f, -0.35f);
        glColor3f(0.03f, 0.08f, 0.18f); // jos
        glVertex2f( 1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
    glEnd();
}


void desenBarca()
{
    glPushMatrix();
    // Pozitie pe apa
    glTranslatef(0.3f, -0.55f, 0.0f);
    glScalef(0.4f, 0.4f, 1.0f);

    // Urme pe apa
    glColor3f(0.9f, 0.95f, 1.0f);
    glBegin(GL_TRIANGLES);
    // Spuma
    glVertex2f(-0.9f, -0.25f);
    glVertex2f(-0.4f, -0.22f);
    glVertex2f(-0.9f, -0.35f);
    glVertex2f(-0.7f, -0.20f);
    glVertex2f(-0.3f, -0.18f);
    glVertex2f(-0.7f, -0.30f);
    glEnd();

    // Linii pe apa 
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.8f, -0.28f); glVertex2f(-0.5f, -0.26f);
    glVertex2f(-0.75f, -0.32f); glVertex2f(-0.45f, -0.30f);
    glEnd();
    glLineWidth(1.0f);

    // Corp
    glColor3f(0.70f, 0.75f, 0.82f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.4f, -0.25f);  // spate jos
    glVertex2f(0.5f, -0.25f);   // aproape de prova jos
    glVertex2f(0.9f, -0.18f);   // varf prova
    glVertex2f(0.5f, -0.10f);   // aproape de prova sus
    glVertex2f(-0.4f, -0.10f);  // spate sus
    glEnd();

    // Banda lateral
    glColor3f(0.45f, 0.50f, 0.60f);
    glBegin(GL_QUADS);
    glVertex2f(-0.35f, -0.22f);
    glVertex2f(0.55f, -0.22f);
    glVertex2f(0.55f, -0.19f);
    glVertex2f(-0.35f, -0.19f);
    glEnd();

    // Punte
    glColor3f(0.85f, 0.87f, 0.92f);
    glBegin(GL_QUADS);
    glVertex2f(-0.25f, -0.10f);
    glVertex2f(0.45f, -0.10f);
    glVertex2f(0.35f, 0.05f);
    glVertex2f(-0.20f, 0.05f);
    glEnd();

    // Cabina
    glColor3f(0.20f, 0.25f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(-0.10f, -0.05f);
    glVertex2f(0.30f, -0.05f);
    glVertex2f(0.22f, 0.08f);
    glVertex2f(-0.05f, 0.08f);
    glEnd();

    // Geamuri
    glLineWidth(1.5f);
    glColor3f(0.65f, 0.75f, 0.90f);
    glBegin(GL_LINES);
    glVertex2f(-0.05f, 0.00f); glVertex2f(0.25f, 0.00f);
    glVertex2f(-0.03f, 0.03f); glVertex2f(0.23f, 0.03f);
    glEnd();
    glLineWidth(1.0f);

    // Radar
    glColor3f(0.8f, 0.8f, 0.9f);
    glBegin(GL_LINES);
    glVertex2f(0.05f, 0.08f);
    glVertex2f(0.05f, 0.20f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-0.02f, 0.20f);
    glVertex2f(0.12f, 0.20f);
    glEnd();

    glPopMatrix();
}


// afisare
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    desenCer();
    desenOras();
    desenTrotuar();
    desenLampi();
    desenApa();
    desenBarca();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.95f, 0.9f, "Tema 2 - Oras 2D");

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    winWidth = w;
    winHeight = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    buildlistaCladire();
    buildlistaLampa();
}

// main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Tema 2 - Oras 2D");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
