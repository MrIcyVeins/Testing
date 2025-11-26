
#include <GL/glut.h>
#include <cmath>

GLuint texID;
const float side = 30.0f; // latura

// Textura simpla
void createTexture()
{
    const int W = 64, H = 64;
    unsigned char data[W * H * 3];

    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            int idx = (y * W + x) * 3;
            bool checker = ((x / 8) % 2) ^ ((y / 8) % 2);
            unsigned char c = checker ? 255 : 80;
            data[idx + 0] = c;
            data[idx + 1] = checker ? 80 : 255;
            data[idx + 2] = 160;
        }
    }

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        W, H, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

// Patrat centrat in origine ( l = 30 )
void drawSquareColored()
{
    float h = side / 2.0f;

    glBegin(GL_QUADS);
    // Culori diferite pe varfuri
    glColor3f(1.0f, 0.0f, 0.0f);  glVertex2f(-h, -h);
    glColor3f(0.0f, 1.0f, 0.0f);  glVertex2f(h, -h);
    glColor3f(0.0f, 0.0f, 1.0f);  glVertex2f(h, h);
    glColor3f(1.0f, 1.0f, 0.0f);  glVertex2f(-h, h);
    glEnd();
}

void drawSquareTextured()
{
    float h = side / 2.0f;

    glBindTexture(GL_TEXTURE_2D, texID);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-h, -h);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(h, -h);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(h, h);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-h, h);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 1) Patratul de baza
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    float h = side / 2.0f;
    glBegin(GL_LINE_LOOP);
    glVertex2f(-h, -h);
    glVertex2f(h, -h);
    glVertex2f(h, h);
    glVertex2f(-h, h);
    glEnd();
    glPopMatrix();

    // Factori ceruti
    float sx = 2.0f, sy = 0.5f;
    float tx = 40.0f, ty = 40.0f;

    // 2) Varianta cu amestec de culori:
    //    Scalare > Translatie
    glPushMatrix();
    glTranslatef(tx, ty, 0.0f);  // translatia T
    glScalef(sx, sy, 1.0f);      // scalarea S
    drawSquareColored();
    glPopMatrix();

    // 3) Varianta texturata:
    //    Translatie > Scalare
    glPushMatrix();
    glScalef(sx, sy, 1.0f);      // scalarea S
    glTranslatef(tx, ty, 0.0f);  // translația T
    drawSquareTextured();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200.0, 200.0, -200.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    createTexture();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tema 3 - Transformari");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
