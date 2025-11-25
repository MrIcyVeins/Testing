#include <GL/glut.h>

int winWidth = 800;
int winHeight = 600;

//  FUNDAL GRADIENT (1)
void drawBackground()
{
    glBegin(GL_QUADS);

    glColor3f(0.05f, 0.07f, 0.20f);  // jos
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);

    glColor3f(0.20f, 0.10f, 0.35f);  // sus
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);

    glEnd();
}

// Font text
void drawText(float x, float y, const char* t)
{
    glRasterPos2f(x, y);
    while (*t)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *t++);
}

// POLIGON CONVEX (1)
void drawConvexPolygon()
{
    glColor3f(0.9f, 0.3f, 0.3f);

    glBegin(GL_POLYGON);
    glVertex2f(-0.85f, -0.7f);
    glVertex2f(-0.65f, -0.45f);
    glVertex2f(-0.50f, -0.50f);
    glVertex2f(-0.45f, -0.75f);
    glVertex2f(-0.70f, -0.85f);
    glEnd();
}

// POLIGON CONCAV (1)
void drawConcavePolygon()
{
    glColor3f(0.2f, 0.8f, 0.9f);

    glBegin(GL_POLYGON);
    glVertex2f(0.25f, -0.45f);
    glVertex2f(0.65f, -0.45f);
    glVertex2f(0.70f, -0.65f);
    glVertex2f(0.55f, -0.70f);  // concavitate
    glVertex2f(0.65f, -0.90f);
    glVertex2f(0.30f, -0.90f);
    glEnd();
}

// TRIUNGHI ABC (GL_FILL) (2)
void drawTriangleABC()
{
    glPolygonMode(GL_FRONT, GL_FILL);
    glColor3f(0.1f, 0.9f, 0.4f);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.70f, 0.50f); // A
    glVertex2f(-0.30f, 0.50f); // B
    glVertex2f(-0.50f, 0.70f); // C
    glEnd();

    // litere
    glColor3f(1, 1, 1);
    drawText(-0.74f, 0.47f, "A");
    drawText(-0.30f, 0.46f, "B");
    drawText(-0.52f, 0.73f, "C");
}

// TRIUNGHI DEF (GL_LINE) (2)
void drawTriangleDEF()
{
    glPolygonMode(GL_BACK, GL_LINE);
    glColor3f(1.0f, 0.9f, 0.2f);

    glBegin(GL_TRIANGLES);
    glVertex2f(0.30f, 0.35f); // D
    glVertex2f(0.53f, 0.60f); // F
    glVertex2f(0.75f, 0.35f); // E
    glEnd();

    glColor3f(1, 1, 1);
    drawText(0.26f, 0.32f, "D");
    drawText(0.51f, 0.63f, "F");
    drawText(0.75f, 0.32f, "E");
}

// Afiseaza
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    drawBackground();

    // Poligoane (jos)
    drawConvexPolygon();
    drawConcavePolygon();

    // Triunghiuri (sus)
    glFrontFace(GL_CCW);
    drawTriangleABC();
    drawTriangleDEF();

    glutSwapBuffers();
}

// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Tema 2 - Primitive grafice");

    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
