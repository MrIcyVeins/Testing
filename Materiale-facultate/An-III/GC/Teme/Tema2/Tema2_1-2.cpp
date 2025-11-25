#include <GL/glut.h>

// Dimensiunea ferestrei
int winWidth = 800;
int winHeight = 600;

// deseneaza text
void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    while (*text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
}

// 1)
// Definirea fundalului
void fundalGradient()
{
    glBegin(GL_QUADS); // deseneaza un patrat

    // stânga-jos – albastru închis
    glColor3f(0.1f, 0.1f, 0.3f);
    glVertex2f(-1.0f, -1.0f);

    // dreapta-jos – violet
    glColor3f(0.3f, 0.1f, 0.5f);
    glVertex2f(1.0f, -1.0f);

    // dreapta-sus – albastru deschis
    glColor3f(0.6f, 0.8f, 1.0f);
    glVertex2f(1.0f, 1.0f);

    // stânga-sus – turcoaz
    glColor3f(0.2f, 0.7f, 0.7f);
    glVertex2f(-1.0f, 1.0f);

    glEnd();
}

// poligon CONVEX
void desenPoligonConvex()
{
    glColor3f(1.0f, 0.6f, 0.1f);   // portocaliu
    glBegin(GL_POLYGON);
    glVertex2f(-0.8f, -0.4f);
    glVertex2f(-0.6f, -0.1f);
    glVertex2f(-0.3f, -0.25f);
    glVertex2f(-0.3f, -0.6f);
    glVertex2f(-0.6f, -0.7f);
    glEnd();
}

// poligon CONCAV
void desenPoligonConcav()
{
    glColor3f(0.1f, 0.9f, 0.4f);   // verde
    glBegin(GL_POLYGON);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.6f, -0.2f);
    glVertex2f(0.6f, -0.5f);
    glVertex2f(0.4f, -0.5f);
    glVertex2f(0.4f, -0.7f);   // concavitate
    glVertex2f(0.2f, -0.7f);
    glEnd();
}

// 2)

// triunghi ABC - fata
void desenTriunghiABC()
{
    // modul pentru fata = plin
    glPolygonMode(GL_FRONT, GL_FILL);

    glColor3f(1.0f, 0.0f, 0.0f); // rosu
    glBegin(GL_TRIANGLES);

    // CCW/counterclockwise => mod CCW pentru fata => fata
    glVertex2f(-0.8f, 0.2f); // A
    glVertex2f(-0.2f, 0.2f); // B
    glVertex2f(-0.5f, 0.9f); // C

    glEnd();

    glColor3f(1, 1, 1);
    drawText(-0.82f, 0.15f, "A");
    drawText(-0.22f, 0.15f, "B");
    drawText(-0.52f, 0.92f, "C");
}


// triunghi DEF - spate
void desenTriunghiDEF()
{
    // modul pentru spate = linii
    glPolygonMode(GL_BACK, GL_LINE);

    glColor3f(0.0f, 1.0f, 0.0f); // verde
    glBegin(GL_TRIANGLES);

    // puncte CW/clockwise => mod CCW pentru fata => spate
    glVertex2f(0.2f, 0.2f); // D
    glVertex2f(0.5f, 0.9f); // E
    glVertex2f(0.8f, 0.2f); // F

    glEnd();

    // litere
    glColor3f(1, 1, 1);
    drawText(0.18f, 0.15f, "D");
    drawText(0.48f, 0.92f, "E");
    drawText(0.78f, 0.15f, "F");
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 2)
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // 1)
    // Deseneaza fundalul
    fundalGradient();

    // Deseneaza poligonul convex
    desenPoligonConvex();

    // Deseneaza poligonul concav
    desenPoligonConcav();

    // 2)
    glFrontFace(GL_CCW);        // CCW/counterclockwise = fata
    //glEnable(GL_CULL_FACE);   // activam cull face

    desenTriunghiABC(); // fata => GL_FILL
    desenTriunghiDEF(); // spate => GL_LINE

    glDisable(GL_CULL_FACE);

    glutSwapBuffers();
}

// Main

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Tema 2 - Georgescu Andrei");


    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
