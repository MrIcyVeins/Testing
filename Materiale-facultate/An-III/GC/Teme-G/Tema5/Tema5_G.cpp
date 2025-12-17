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

//  Tema 5: iluminare / umbra / ceata
int lumini = 1;

GLfloat fogColor[4] = { 0.6f, 0.9f, 0.95f, 1.0f };

// Umbra
enum { X, Y, Z, W };
enum { A, B, C, D };

// pozitia luminii
GLfloat lightPos[4] = { 10.0f, -8.0f, 16.0f, 1.0f };

// matricea umbrei pe sol
GLfloat floorShadow[4][4];

// control inaltime sol + bias umbra
float groundZ = -2.0f;
float shadowBias = 0.01f;

// plane + shadowmatrix
void findplane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
    GLfloat vec0[3], vec1[3];

    vec0[X] = v1[X] - v0[X];
    vec0[Y] = v1[Y] - v0[Y];
    vec0[Z] = v1[Z] - v0[Z];

    vec1[X] = v2[X] - v0[X];
    vec1[Y] = v2[Y] - v0[Y];
    vec1[Z] = v2[Z] - v0[Z];

    plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
    plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
    plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
    plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}

void shadowmatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4])
{
    GLfloat dot = groundplane[X] * lightpos[X] +
        groundplane[Y] * lightpos[Y] +
        groundplane[Z] * lightpos[Z] +
        groundplane[W] * lightpos[W];

    shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
    shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
    shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
    shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

    shadowMat[0][1] = 0.f - lightpos[Y] * groundplane[X];
    shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
    shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
    shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

    shadowMat[0][2] = 0.f - lightpos[Z] * groundplane[X];
    shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
    shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
    shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

    shadowMat[0][3] = 0.f - lightpos[W] * groundplane[X];
    shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
    shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
    shadowMat[3][3] = dot - lightpos[W] * groundplane[W];
}

void setupCeataScena()
{
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP2);

    glFogf(GL_FOG_DENSITY, 0.025f);

    glHint(GL_FOG_HINT, GL_DONT_CARE);
}

void setupLuminaScena()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // valori default (se reseteaza la fiecare frame)
    GLfloat ambientL[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat diffuseL[4] = { 1.00f, 1.00f, 1.00f, 1.0f };
    GLfloat specularL[4] = { 1.00f, 1.00f, 1.00f, 1.0f };

    GLfloat matSpec[4] = { 0.35f, 0.35f, 0.35f, 1.0f };
    GLfloat shininess = 20.0f;

    // spotlight default
    GLfloat spotDir[3] = { -0.4f, 0.3f, -1.0f };
    GLfloat cutoff = 55.0f;
    GLfloat exponent = 8.0f;

    // atenuare default (fara efect puternic)
    GLfloat attConst = 1.0f;
    GLfloat attLin = 0.0f;
    GLfloat attQuad = 0.0f;

    // 4 variante
    switch (lumini)
    {
    case 1:
        // (1) lumina ambientala mai puternica
        ambientL[0] = ambientL[1] = ambientL[2] = 0.55f;
        break;

    case 2:
        // (2) lumina difuza calda
        diffuseL[0] = 1.0f; diffuseL[1] = 0.85f; diffuseL[2] = 0.65f;
        break;

    case 3:
        // (3) spotlight mai concentrat (se vede ca un "spot" pe scena)
        cutoff = 25.0f;
        exponent = 20.0f;
        break;

    case 4:
        // (4) atenuare cu distanta (lumina scade pe masura ce te indepartezi)
        attConst = 0.8f;
        attLin = 0.06f;
        attQuad = 0.01f;
        break;
    }

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientL);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseL);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularL);

    // Aplicarea efectiva a parametrilor modificati (spot + atenuare) se face aici
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, exponent);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, attConst);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, attLin);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, attQuad);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void desenPlatou()
{
    GLfloat sol[4] = { 0.88f, 0.90f, 0.93f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sol);
    glColor3f(sol[0], sol[1], sol[2]);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-35.0f, -35.0f, groundZ);
    glVertex3f(35.0f, -35.0f, groundZ);
    glVertex3f(35.0f, 35.0f, groundZ);
    glVertex3f(-35.0f, 35.0f, groundZ);
    glEnd();
}

void desenUmbraProiectata(void (*drawObject)())
{
    //glDisable(GL_FOG);

    glEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glDisable(GL_LIGHTING);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glMultMatrixf((GLfloat*)floorShadow);
    glTranslatef(0.0f, 0.0f, shadowBias);
    drawObject();
    glPopMatrix();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glColor4f(0.0f, 0.0f, 0.0f, 0.30f);

    float zOverlay = groundZ + shadowBias;

    glBegin(GL_QUADS);
    glVertex3f(-35.0f, -35.0f, zOverlay);
    glVertex3f(35.0f, -35.0f, zOverlay);
    glVertex3f(35.0f, 35.0f, zOverlay);
    glVertex3f(-35.0f, 35.0f, zOverlay);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glDisable(GL_STENCIL_TEST);

    //glEnable(GL_FOG);
}

// Proiectie si fereastra
void reshapePerspective(int w, int h)
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
    glTranslatef(0.0f, 0.0f, groundZ + 3.0f);

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
void initScena(void)
{
    glClearColor(0.6f, 0.9f, 0.95f, 0.0f);  // fundal albastru
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
    glClearStencil(0);

    // matrice umbra pentru planul solului (3 puncte)
    GLfloat plane[4];
    GLfloat v0[3] = { -35.f, -35.f, groundZ };
    GLfloat v1[3] = { 35.f, -35.f, groundZ };
    GLfloat v2[3] = { 35.f,  35.f, groundZ };
    findplane(plane, v0, v1, v2);
    shadowmatrix(floorShadow, plane, lightPos);

    setupCeataScena();
    glDisable(GL_CULL_FACE);
}

// Afisare
void randareCadru(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    // calculam pozitia camerei pe sfera
    Obsx = Refx + dist * cosf(alpha) * cosf(beta);
    Obsy = Refy + dist * cosf(alpha) * sinf(beta);
    Obsz = Refz + dist * sinf(alpha);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(Obsx, Obsy, Obsz, Refx, Refy, Refz, 0.0f, 0.0f, 1.0f);

    setupLuminaScena();
    setupCeataScena();

    desenPlatou();
    desenUmbraProiectata(desenInghetata);
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
    glutPostRedisplay();
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

    if (key == '1') lumini = 1;
    if (key == '2') lumini = 2;
    if (key == '3') lumini = 3;
    if (key == '4') lumini = 4;

    glutPostRedisplay();
}

// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // IMPORTANT: stencil pentru umbra
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);

    glutInitWindowSize(900, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema5 - model 3D inghetata");

    initScena();

    glutReshapeFunc(reshapePerspective);
    glutDisplayFunc(randareCadru);
    glutIdleFunc(randareCadru);

    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

    glutMainLoop();
    return 0;
}
