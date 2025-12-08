#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <gl/freeglut.h>
#include <math.h>

float PI = 3.141592f;

// punctul de referinta (centrul obiectului)
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;

// unghiurile de rotatie pentru survolare
float alpha = 0.3f;    // unghi fata de planul XY (inaltime)
float beta = 0.0f;    // unghi in planul XY (stanga-dreapta)

// raza sferei de survolare (distanta observator–obiect)
float dist = 20.0f;

// coordonatele observatorului (calculate din alpha, beta, dist)
float Obsx, Obsy, Obsz;

// Proiectie si fereastra

void reshapeAndProjection(int w, int h)
{
    if (h == 0)
        h = 1;

    float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(60.0f, ratio, 0.1f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Desenarea obiectului 3D
// Folosim cel putin doua primitive GLUT: glutSolidSphere, glutSolidCube, glutSolidCone

void desenOmZapada()
{
    glPushMatrix();

    // coborare plan
    glTranslatef(0.0f, 0.0f, -2.0f);

        // Sfera de jos ( glutSolidSphere ) 
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 2.0f);
        glutSolidSphere(2.0, 40, 40);
        glPopMatrix();

        // Sfera din mijloc
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 4.5f);
        glutSolidSphere(1.5, 40, 40);
        glPopMatrix();

        // Sfera de sus ( capul ) 
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 6.3f);
        glutSolidSphere(1.0, 40, 40);
        glPopMatrix();

        // Nas ( glutSolidCone ) 
        glColor3f(1.0f, 0.5f, 0.0f);
        glPushMatrix();
        glTranslatef(0.8f, 0.0f, 6.3f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glutSolidCone(0.2, 1.0, 20, 20);
        glPopMatrix();

        // OCHI
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.8f, 0.3f, 6.8f);
        glutSolidSphere(0.12, 10, 10);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.8f, -0.3f, 6.8f);
        glutSolidSphere(0.12, 10, 10);
        glPopMatrix();

        // Palarie (glutSolidCube)
        // Borul palariei
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 7.1f);
        glScalef(2.0f, 2.0f, 0.2f);
        glutSolidCube(1.0);
        glPopMatrix();

        // Corpul palariei
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 7.8f);
        glScalef(1.0f, 1.0f, 1.5f);
        glutSolidCube(0.8);
        glPopMatrix();

        // Butoni
        // buton 1 (jos)
        glPushMatrix();
            glTranslatef(1.4f, 0.0f, 4.0f);
            glutSolidSphere(0.12, 10, 10);
        glPopMatrix();

        // buton 2 (mijloc)
        glPushMatrix();
            glTranslatef(1.5f, 0.0f, 4.6f);
            glutSolidSphere(0.12, 10, 10);
        glPopMatrix();

        // buton 3 (sus)
        glPushMatrix();
            glTranslatef(1.3f, 0.0f, 5.2f);
            glutSolidSphere(0.12, 10, 10);
        glPopMatrix();

    glPopMatrix();
}

// Initializare
void init(void)
{
    glClearColor(0.6f, 0.9f, 0.95f, 0.0f);  // cer
    glEnable(GL_DEPTH_TEST);

    // Lumina simpla pentru efect 3D mai bun
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = { 0.0f, 0.0f, 20.0f, 1.0f }; // pozitia luminii ( deasupra ) 
    GLfloat light_diff[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // lumina difuza -culoarea alb
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

// Afisare

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    // calculam pozitia observatorului pe sfera de raza "dist"
    Obsx = Refx + dist * cosf(alpha) * cosf(beta);
    Obsy = Refy + dist * cosf(alpha) * sinf(beta);
    Obsz = Refz + dist * sinf(alpha);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // camera: observator -> punct de referinta (centru obiect) > vector "up" (axa Z)
    gluLookAt(Obsx, Obsy, Obsz, Refx, Refy, Refz, 0.0f, 0.0f, 1.0f);

    // desen om de zapada
    desenOmZapada();

    glutSwapBuffers();
    glFlush();
}

// Control taste
// schimbare pozitie observator
void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
    case GLUT_KEY_LEFT:
        beta -= 0.05f;
        break;
    case GLUT_KEY_RIGHT:
        beta += 0.05f;
        break;
    case GLUT_KEY_UP:
        alpha += 0.05f;
        // NU lasam sa treaca de ~ +90°
        if (alpha > 1.55f)
            alpha = 1.55f;
        break;
    case GLUT_KEY_DOWN:
        alpha -= 0.05f;
        // NU lasam sa treaca de ~ -90°
        if (alpha < -1.55f)
            alpha = -1.55f;
        break;
    }
}


//  Modificam raza sferei de survolare cu "+" si "-"
void processNormalKeys(unsigned char key, int x, int y)
{
    switch (key) {

    case '+':   // Shift + 
    case '=':   // pentru +
        dist -= 1.0f;
        if (dist < 3.0f) dist = 3.0f;
        break;

    case '-':   // pentru - 
        dist += 1.0f;
        if (dist > 100.0f) dist = 100.0f;
        break;
    }

    glutPostRedisplay();
}

// Main

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema 4 - Obiect 3D + Survolare");

    init();

    glutReshapeFunc(reshapeAndProjection);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

    glutMainLoop();
    return 0;
}
