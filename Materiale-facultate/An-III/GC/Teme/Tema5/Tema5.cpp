#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <gl/freeglut.h>
#include <math.h>

// Variabile pentru camera
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;
float alpha = 0.3f; // unghi fata de planul XY (inaltime)
float beta = 0.0f; // unghi in planul XY (stanga-dreapta)
// raza sferei de survolare (distanta observator–obiect)
float dist = 20.0f;
// coordonatele observatorului (calculate din alpha, beta, dist)
float Obsx, Obsy, Obsz;

// CERINTA 2 - parametrii de iluminare
// 1 - Lumina ambientala ( GL_AMBIENT )
// 2 - Lumina difuza ( GL_DIFFUSE ) 
// 3 - Lumina speculara ( GL_SPECULAR ) 
// 4 - Proprietate a materialului - stralucirea ( GL_SHININESS ) 
int lightVariant = 1; 

// Ceata
GLfloat fogColor[4] = { 0.6f, 0.9f, 0.95f, 1.0f };

// Umbra
enum { X, Y, Z, W };
enum { A, B, C, D };

// pozitia luminii
GLfloat lightPos[4] = { 8.0f, -10.0f, 18.0f, 1.0f };

// matricea umbrei pe sol
GLfloat floorShadow[4][4];

// plane + shadowmatrix
void findplane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
    GLfloat vec0[3], vec1[3];
    vec0[X] = v1[X] - v0[X]; vec0[Y] = v1[Y] - v0[Y]; vec0[Z] = v1[Z] - v0[Z];
    vec1[X] = v2[X] - v0[X]; vec1[Y] = v2[Y] - v0[Y]; vec1[Z] = v2[Z] - v0[Z];

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

// Proiectie
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

// Desenarea obiectului 3D
// Folosim cel putin doua primitive GLUT: glutSolidSphere, glutSolidCube, glutSolidCone
void desenOmZapada()
{
    glPushMatrix();

    // Omul sta pe sol: sfera jos centru z=2, raza=2 => baza atinge z=0
    GLfloat alb[4] = { 1,1,1,1 };
    GLfloat negru[4] = { 0.1f,0.1f,0.1f,1 };
    GLfloat portocaliu[4] = { 1.0f,0.5f,0.0f,1 };

    // Sfera de jos ( glutSolidSphere ) 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, alb);
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
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, portocaliu);
    glPushMatrix();
    glTranslatef(0.8f, 0.0f, 6.3f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.2, 1.0, 20, 20);
    glPopMatrix();

    // ochi + palarie + butoni
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, negru);

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

// Sol
void desenSol()
{
    // sol - gri - ca sa nu se piarda in ceata
    GLfloat sol[4] = { 0.88f, 0.90f, 0.93f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sol);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-50.0f, -50.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, 0.0f);
    glVertex3f(50.0f, 50.0f, 0.0f);
    glVertex3f(-50.0f, 50.0f, 0.0f);
    glEnd();
}

// CERINTA 1 + 2: iluminare 
void aplicaIluminare()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);

    GLfloat ambientL[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseL[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularL[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    GLfloat matSpec[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat shininess = 32.0f;

    // 4 modificari (cate una dominanta)
    switch (lightVariant)
    {
    case 1: // Ambient
        ambientL[0] = ambientL[1] = ambientL[2] = 0.55f;
        break;

    case 2: // Difuz colorat (warm)
        diffuseL[0] = 1.0f; diffuseL[1] = 0.85f; diffuseL[2] = 0.6f;
        break;

    case 3: // Specular material mai mare
        matSpec[0] = matSpec[1] = matSpec[2] = 1.0f;
        break;

    case 4: // Shininess mai mare
        shininess = 8.0f;
        break;
    }

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientL);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseL);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularL);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

// CERINTA 4: ceata
void aplicaCeata()
{
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);

    // control ceata
    glFogf(GL_FOG_START, 25.0f);
    glFogf(GL_FOG_END, 120.0f);

    glHint(GL_FOG_HINT, GL_DONT_CARE);
}

// CERINTA 3: umbra
void desenUmbra()
{
    //  
    glDisable(GL_FOG);

    // Salvam starea minima 
    glEnable(GL_STENCIL_TEST);

    // 1) SCRIEM in STENCIL zona umbrei (fara culoare)
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glDisable(GL_LIGHTING);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glMultMatrixf((GLfloat*)floorShadow);
    glTranslatef(0.0f, 0.0f, 0.001f);
    desenOmZapada(); // doar pentru stencil
    glPopMatrix();

    // 2) INTUNECAM SOLUL unde stencil==1 (umbra urmareste culoarea solului)
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // "factor" de umbra: un overlay negru cu alpha => inchide culoarea solului existent
    glColor4f(0.0f, 0.0f, 0.0f, 0.35f);

    // desenam un quad peste sol (doar in zona stencil)
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, -50.0f, 0.002f);
    glVertex3f(50.0f, -50.0f, 0.002f);
    glVertex3f(50.0f, 50.0f, 0.002f);
    glVertex3f(-50.0f, 50.0f, 0.002f);
    glEnd();

    // restore
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glDisable(GL_STENCIL_TEST);

    glEnable(GL_FOG);
}

// Initializare
void init()
{
    glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    // stencil
    glClearStencil(0);

    // matrice umbra pe planul solului (3 puncte pe z=0)
    GLfloat plane[4];
    GLfloat v0[3] = { -50.f, -50.f, 0.f };
    GLfloat v1[3] = { 50.f, -50.f, 0.f };
    GLfloat v2[3] = { 50.f,  50.f, 0.f };
    findplane(plane, v0, v1, v2);
    shadowmatrix(floorShadow, plane, lightPos);

    // ceata activa
    aplicaCeata();
}

// Afisare
void display()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // camera
    Obsx = Refx + dist * cosf(alpha) * cosf(beta);
    Obsy = Refy + dist * cosf(alpha) * sinf(beta);
    Obsz = Refz + dist * sinf(alpha);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(Obsx, Obsy, Obsz, Refx, Refy, Refz, 0.0f, 0.0f, 1.0f);

    aplicaIluminare();
    //glDisable(GL_LIGHTING);
    aplicaCeata();

    // ordine: sol > umbra > obiect
    desenSol();
    desenUmbra();
    desenOmZapada();

    glutSwapBuffers();
    glFlush();
    glDisable(GL_LIGHTING);
}

// Control taste
void processSpecialKeys(int key, int xx, int yy)
{
    switch (key) {
    case GLUT_KEY_LEFT:  beta -= 0.05f; break;
    case GLUT_KEY_RIGHT: beta += 0.05f; break;
    case GLUT_KEY_UP:
        alpha += 0.05f;
        if (alpha > 1.55f) alpha = 1.55f;
        break;
    case GLUT_KEY_DOWN:
        alpha -= 0.05f;
        if (alpha < -1.55f) alpha = -1.55f;
        break;
    }
    glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '+':
    case '=':
        dist -= 1.0f; if (dist < 3.0f) dist = 3.0f;
        break;
    case '-':
        dist += 1.0f; if (dist > 100.0f) dist = 100.0f;
        break;

    // CERINTA 2
    case '1': lightVariant = 1; break;
    case '2': lightVariant = 2; break;
    case '3': lightVariant = 3; break;
    case '4': lightVariant = 4; break;

    case 27:
        exit(0);
    }

    glutPostRedisplay();
}

// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // Stencil pentru umbra
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema 5 - Iluminare + Umbra + Ceata");

    init();

    glutReshapeFunc(reshapeAndProjection);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

    glutMainLoop();
    return 0;
}
