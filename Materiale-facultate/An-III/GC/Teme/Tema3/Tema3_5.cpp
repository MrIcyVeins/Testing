#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Variabile
float unghiPlaneta = 0.0f;
float unghiSatelit = 0.0f;

// Viteze rotatie (mai mici)
float pasPlaneta = 0.005f;   // viteza rotatie planeta
float pasSatelit = 0.02f;    // viteza rotatie satelit

// Liste
GLuint listaStea;
GLuint listaPlaneta;
GLuint listaSatelit;

// Texturi
GLuint texSoare = 0;
GLuint texPlaneta = 0;

// Incarcare textura din fisiere BMP (24-bit, GL_BGR_EXT)
GLuint loadBMP(const char* fileName)
{
    FILE* f = NULL;
    fopen_s(&f, fileName, "rb");
    if (!f)
    {
        printf("Fisierul nu se poate deschide: %s\n", fileName);
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

    // zone de memorie pentru datele imaginilor
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

    // sursa este GL_BGR_EXT
    glTexImage2D(GL_TEXTURE_2D, 0,
        GL_RGB, width, height,
        0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

    free(data);

    return texID;
}

// Initializare
void init(void)
{
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400.0f, 400.0f, -300.0f, 300.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // incarcam texturile
    texSoare = loadBMP("sun.bmp");
    texPlaneta = loadBMP("earth.bmp");

    if (texSoare == 0)
        printf("Atentie: textura sun.bmp nu a fost incarcata corect!\n");
    if (texPlaneta == 0)
        printf("Atentie: textura earth.bmp nu a fost incarcata corect!\n");

    // Liste display
    // Soare texturat pe un disc
    listaStea = glGenLists(1);
    glNewList(listaStea, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texSoare);
    glColor3f(1.0f, 1.0f, 1.0f); // lasam culoarea texturii

    float razaSoare = 30.0f;
    glBegin(GL_TRIANGLE_FAN);
    // centru – coordonate de textura (0.5, 0.5)
    glTexCoord2f(0.5f, 0.5f);
    glVertex2f(0.0f, 0.0f);

    for (int i = 0; i <= 30; i++)
    {
        float a = i * 2.0f * 3.1415926f / 30.0f;
        float cx = razaSoare * cosf(a);
        float cy = razaSoare * sinf(a);

        // mapam cercul in [0,1]x[0,1]
        float s = (cosf(a) + 1.0f) * 0.5f;
        float t = (sinf(a) + 1.0f) * 0.5f;

        glTexCoord2f(s, t);
        glVertex2f(cx, cy);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEndList();

    // Planeta – patrat texturat
    listaPlaneta = glGenLists(1);
    glNewList(listaPlaneta, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texPlaneta);
    glColor3f(1.0f, 1.0f, 1.0f);

    float hp = 15.0f; // jumatate din latura planetei

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-hp, -hp);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(hp, -hp);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(hp, hp);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-hp, hp);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEndList();

    // Satelit – disc gri, fara textura
    listaSatelit = glGenLists(1);
    glNewList(listaSatelit, GL_COMPILE);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 20; i++)
    {
        float a = i * 2.0f * 3.1415926f / 20.0f;
        glVertex2f(6.0f * cosf(a), 6.0f * sinf(a));
    }
    glEnd();
    glEndList();
}

// Display
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Stea texturata in centru
    glPushMatrix();
    glCallList(listaStea);
    glPopMatrix();

    // Planeta
    glPushMatrix();
    glRotatef(unghiPlaneta, 0.0f, 0.0f, 1.0f);
    glTranslatef(150.0f, 0.0f, 0.0f);

    glCallList(listaPlaneta);

    // Satelit (fara textura)
    glPushMatrix();
    glRotatef(unghiSatelit, 0.0f, 0.0f, 1.0f);
    glTranslatef(40.0f, 0.0f, 0.0f);
    glCallList(listaSatelit);
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

void idle(void)
{
    unghiPlaneta += pasPlaneta;
    unghiSatelit += pasSatelit;

    if (unghiPlaneta > 360.0f) unghiPlaneta -= 360.0f;
    if (unghiSatelit > 360.0f) unghiSatelit -= 360.0f;

    glutPostRedisplay();
}

// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tema3 - mini sistem solar + texturi");

    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
