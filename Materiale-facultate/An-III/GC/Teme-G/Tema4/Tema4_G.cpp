#include <windows.h>
#include <gl/freeglut.h>
#include <math.h>

float PI = 3.141592f;

// punct de referinta (centru scena)
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;

// unghiuri de survolare
float alpha = 0.4f;   // sus-jos
float beta = 0.7f;   // stanga-dreapta

// raza sferei de survolare
float dist = 18.0f;

// observator
float Obsx, Obsy, Obsz;


// Proiectie si fereastra

void reshapeAndProjection(int w, int h)
{
    if (h == 0) h = 1;
    float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(60.0f, ratio, 0.1f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// 1)
// Desen inghetata
void desenInghetata()
{
    glPushMatrix();

    // mutare plan
    glTranslatef(0.0f, 0.0f, -1.0f);

    // Cornet ( con - glutSolidCone )
    glColor3f(0.9f, 0.7f, 0.4f);     // maro
    glPushMatrix();
    glRotatef(180.0f, 1, 0, 0);  // varful conului in jos
    glutSolidCone(0.8f, 3.0f, 30, 10);
    glPopMatrix();

    // Bile de inghetata ( sfera - glutSolidSphere ) 
    // Bila 1 ( jos )
    glColor3f(1.0f, 1.0f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.9f);
    glutSolidSphere(1.0f, 30, 30);
    glPopMatrix();

    // Bila 2 ( mijloc )
    glColor3f(1.0f, 0.6f, 0.7f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.9f);
    glutSolidSphere(0.9f, 30, 30);
    glPopMatrix();

    // Bila 3 ( sus )
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 2.8f);
    glColor3f(0.6f, 1.0f, 0.7f);
    glutSolidSphere(0.85f, 30, 30);

    // Bucati ciocolata ( glutSolidSphere ) 
    glColor3f(0.45f, 0.25f, 0.1f); // maro
    // bucata 1 – aproape varf
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.82f);
    glutSolidSphere(0.07f, 10, 10);
    glPopMatrix();
    // buata 2 – fata-dreapta
    glPushMatrix();
    glTranslatef(0.7f, 0.0f, 0.4f);
    glutSolidSphere(0.07f, 10, 10);
    glPopMatrix();
    // bucata 3 – fata-stanga
    glPushMatrix();
    glTranslatef(-0.7f, 0.0f, 0.45f);
    glutSolidSphere(0.07f, 10, 10);
    glPopMatrix();
    // bucata 4 – lateral dreapta
    glPushMatrix();
    glTranslatef(0.0f, 0.7f, 0.45f);
    glutSolidSphere(0.07f, 10, 10);
    glPopMatrix();
    // bucata 5 – lateral stanga
    glPushMatrix();
    glTranslatef(0.0f, -0.7f, 0.45f);
    glutSolidSphere(0.07f, 10, 10);
    glPopMatrix();
    // bucata 6 – pe diagonala
    glPushMatrix();
    glTranslatef(0.5f, 0.5f, 0.45f);
    glutSolidSphere(0.07f, 10, 10);
    glPopMatrix();

    glPopMatrix(); // end bila 3

    glPopMatrix(); // end model
}


// Initializare
void init(void)
{
    glClearColor(0.6f, 0.9f, 0.95f, 0.0f);  // fundal albastru
    glEnable(GL_DEPTH_TEST);
}

// Afisare
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    // calculam pozitia camerei pe sfera
    Obsx = Refx + dist * cosf(alpha) * cosf(beta);
    Obsy = Refy + dist * cosf(alpha) * sinf(beta);
    Obsz = Refz + dist * sinf(alpha);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(Obsx, Obsy, Obsz, Refx, Refy, Refz, 0.0f, 0.0f, 1.0f);

    desenInghetata();

    glutSwapBuffers();
    glFlush();
}

// 2)
// schimbare pozitie observator
void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
    case GLUT_KEY_LEFT:
        beta -= 0.05;
        break;
    case GLUT_KEY_RIGHT:
        beta += 0.05;
        break;
    case GLUT_KEY_UP:
        alpha += 0.05;
        break;
    case GLUT_KEY_DOWN:
        alpha -= 0.05;
        break;
    }
}

// 3)
// Survolare cu "-" si "+"
void processNormalKeys(unsigned char key, int x, int y)
{
    if (key == '+' || key == '=') {
        dist = fmax(6.0f, dist - 1.0f);   // apropiere
    }
    else if (key == '-') {
        dist = fmin(100.0f, dist + 1.0f); // departare
    }
    glutPostRedisplay();
}


// Main

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema4 - model 3D inghetata");

    init();

    glutReshapeFunc(reshapeAndProjection);
    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

    glutMainLoop();
    return 0;
}
