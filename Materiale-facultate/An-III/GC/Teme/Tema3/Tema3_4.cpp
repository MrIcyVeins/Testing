#include <GL/glut.h>
#include <cmath>

GLuint idTextura;
const float latura = 30.0f; // latura pătratului

// Textur simpla - tabla de sah
void creeazaTextura()
{
    const int LATIME = 64;
    const int INALTIME = 64;

    // tabla
    unsigned char date[INALTIME][LATIME][3];

    for (int y = 0; y < INALTIME; ++y)
    {
        for (int x = 0; x < LATIME; ++x)
        {
            // 1. Patrat din 8 in 8 pixeli
            int tileX = x / 8;  // coloana patrat
            int tileY = y / 8;  // linie patrat

            // 2. Suma para = culoare verde / suma impara culoare rosie
            bool patratAlb = ((tileX + tileY) % 2 == 0);

            // 3. Culoare in functie de patrat
            unsigned char r, g, b;

            if (patratAlb)
            {
                r = 255; // rosu
                g = 80;  // verde
                b = 160; // albastru
            }
            else
            {
                r = 80;
                g = 255;
                b = 160;
            }

            // 4. Punem valorile în tablou
            date[y][x][0] = r;
            date[y][x][1] = g;
            date[y][x][2] = b;
        }
    }

    // Variabile de textura
    glGenTextures(1, &idTextura);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        LATIME, INALTIME, 0, GL_RGB, GL_UNSIGNED_BYTE, date);
}

// Patrat centrat in origine (l = 30)
void deseneazaPatratColorat()
{
    float jumatateLatura = latura / 2.0f;

    glBegin(GL_QUADS);
    // Culori diferite pe varfuri
    glColor3f(1.0f, 0.0f, 0.0f);  glVertex2f(-jumatateLatura, -jumatateLatura);
    glColor3f(0.0f, 1.0f, 0.0f);  glVertex2f(jumatateLatura, -jumatateLatura);
    glColor3f(0.0f, 0.0f, 1.0f);  glVertex2f(jumatateLatura, jumatateLatura);
    glColor3f(1.0f, 1.0f, 0.0f);  glVertex2f(-jumatateLatura, jumatateLatura);
    glEnd();
}

// Patrat texturat
void deseneazaPatratTexturat()
{
    float jumatateLatura = latura / 2.0f;

    glBindTexture(GL_TEXTURE_2D, idTextura);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-jumatateLatura, -jumatateLatura);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(jumatateLatura, -jumatateLatura);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(jumatateLatura, jumatateLatura);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-jumatateLatura, jumatateLatura);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void afisare()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 1) Patrat de referinta
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    float jumatateLatura = latura / 2.0f;
    glBegin(GL_LINE_LOOP);
    glVertex2f(-jumatateLatura, -jumatateLatura);
    glVertex2f(jumatateLatura, -jumatateLatura);
    glVertex2f(jumatateLatura, jumatateLatura);
    glVertex2f(-jumatateLatura, jumatateLatura);
    glEnd();
    glPopMatrix();

    // Factori de transformare
    float factorScalareX = 2.0f, factorScalareY = 0.5f;
    float translatieX = 40.0f, translatieY = 40.0f;

    // 2) Varianta cu amestec de culori:
    // Ordine: Scalare > Translatie (în spatiul obiectului: întai S, apoi T)
    glPushMatrix();
    glTranslatef(translatieX, translatieY, 0.0f);   // translatia T
    glScalef(factorScalareX, factorScalareY, 1.0f); // scalarea S
    deseneazaPatratColorat();
    glPopMatrix();

    // 3) Varianta texturata:
    // Ordine: Translatie > Scalare (aplicate invers în matrice)
    glPushMatrix();
    glScalef(factorScalareX, factorScalareY, 1.0f); // scalarea S
    glTranslatef(translatieX, translatieY, 0.0f);   // translatia T
    deseneazaPatratTexturat();
    glPopMatrix();

    glutSwapBuffers();
}

void redimensionare(int latime, int inaltime)
{
    glViewport(0, 0, latime, inaltime);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200.0, 200.0, -200.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void initializare()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    creeazaTextura();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tema 3 - Transformari");

    initializare();

    glutDisplayFunc(afisare);
    glutReshapeFunc(redimensionare);

    glutMainLoop();
    return 0;
}
