#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI     3.1415926f
#define TWO_PI (2.0f * PI)

// dimensiunea ferestrei
int winWidth = 800;
int winHeight = 600;

// liste de display
GLuint sunList;     // soare
GLuint planetList;  // planeta
GLuint moonList;    // satelit

// texturi
GLuint sunTexID = 0;
GLuint planetTexID = 0;

// unghiuri
float anglePlanet = 0.0f;
float angleMoon = 0.0f;

// pasi de rotatie
float stepPlanet = 0.004f;   // planeta – miscare lenta
float stepMoon = 0.04f;    // satelit – rapid

// raze orbite
float planetRadius = 200.0f;
float moonRadius = 60.0f;


// Incarcare imagine textura ( GL_BGR_EXT ) 
GLuint loadBMP(const char* fileName)
{
    FILE* f = NULL;
    fopen_s(&f, fileName, "rb");
    if (!f)
    {
        printf("Fisierul nu se poate deschide %s\n", fileName);
        return 0;
    }

    unsigned char header[54];
    fread(header, 1, 54, f);

    // dimensiuni imagine
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // offset-ul la care incepe efectiv imaginea in fisier
    int dataOffset = *(int*)&header[10];

    // numar de biti pe pixel (trebuie sa fie 24)
    short bitsPerPixel = *(short*)&header[28];
    if (bitsPerPixel != 24)
    {
        printf("Imaginea %s NU este 24-bit!\n", fileName);
        fclose(f);
        return 0;
    }

    // fiecare linie este aliniata la multiplu de 4 bytes
    int rowPadded = (width * 3 + 3) & (~3);
    int imageSize = width * height * 3;

    unsigned char* data = (unsigned char*)malloc(imageSize);
    unsigned char* row = (unsigned char*)malloc(rowPadded);

    // mergem la inceputul datelor in fisier
    fseek(f, dataOffset, SEEK_SET);

    // BMP-ul stocheaza liniile de jos in sus
    for (int y = 0; y < height; y++)
    {
        fread(row, 1, rowPadded, f);

        int dstIndex = (height - 1 - y) * width * 3;
        for (int x = 0; x < width * 3; x++)
        {
            data[dstIndex + x] = row[x];
        }
    }

    fclose(f);
    free(row);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // formatul sursei este GL_BGR_EXT, nu GL_RGB ( se pastreaza culoarea imaginii ) 
    glTexImage2D(GL_TEXTURE_2D, 0,
        GL_RGB, width, height,
        0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

    free(data);

    return texID;
}

// liste display 
void makeDisplayLists()
{
    int   i;
    float t;

    // rezervam 3 liste la rand
    sunList = glGenLists(3);
    planetList = sunList + 1;
    moonList = sunList + 2;

    // SOARE
    glNewList(sunList, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, sunTexID);
    glColor3f(1.0f, 1.0f, 1.0f);   // lasam textura cu culorile ei

    float hs = 50.0f; // jumatate din latura soarelui

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-hs, -hs);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(hs, -hs);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(hs, hs);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-hs, hs);
    glEnd();
    glEndList();

    // PLANETA 
    glNewList(planetList, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, planetTexID);
    glColor3f(1.0f, 1.0f, 1.0f);

    float hp = 20.0f; // jumatate din latura (40/2)

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-hp, -hp);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(hp, -hp);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(hp, hp);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-hp, hp);
    glEnd();
    glEndList();

    // SATELIT
    glNewList(moonList, GL_COMPILE);
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    for (i = 0; i < 30; i++)
    {
        t = TWO_PI * i / 30.0f;
        glVertex2f(8.0f * cosf(t), 8.0f * sinf(t));
    }
    glEnd();
    glEndList();
}

// Initializare
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // fundal negru

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400.0, 400.0, -300.0, 300.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // incarcam texturile
    sunTexID = loadBMP("sun.bmp");
    planetTexID = loadBMP("earth.bmp");

    if (sunTexID == 0)
        printf("Atentie: textura sun.bmp nu a fost incarcata corect!\n");
    if (planetTexID == 0)
        printf("Atentie: textura earth.bmp nu a fost incarcata corect!\n");

    glEnable(GL_TEXTURE_2D);   // activam texturi 2D

    makeDisplayLists();
}

// Afisare
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // SOARE in centru (texturat)
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sunTexID);
    glCallList(sunList);
    glPopMatrix();

    // PLANETA – orbiteaza in jurul soarelui
    glPushMatrix();
    glRotatef(anglePlanet, 0.0f, 0.0f, 1.0f);
    glTranslatef(planetRadius, 0.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, planetTexID);
    glCallList(planetList);

    // SATELIT – orbiteaza in jurul planetei
    glPushMatrix();
    glRotatef(angleMoon, 0.0f, 0.0f, 1.0f);
    glTranslatef(moonRadius, 0.0f, 0.0f);

    glDisable(GL_TEXTURE_2D);
    glCallList(moonList);
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

// Animatie
void animatie()
{
    anglePlanet += stepPlanet;
    if (anglePlanet > 360.0f)
        anglePlanet -= 360.0f;

    angleMoon += stepMoon;
    if (angleMoon > 360.0f)
        angleMoon -= 360.0f;

    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400.0, 400.0, -300.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema3 - Mini sistem solar cu texturi dinamice");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(animatie);

    glutMainLoop();
    return 0;
}
