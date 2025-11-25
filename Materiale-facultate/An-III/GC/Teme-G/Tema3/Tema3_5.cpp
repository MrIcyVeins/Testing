#include <windows.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

// animatie (deplasare pe OY si rotatie)
static GLdouble i = 0.0;
static GLdouble j = 0.0;
static GLdouble alpha = 0.02;
static GLdouble step = 0.02;

// texturi
GLuint satTexID = 0; // satelit
GLuint asteroidTexID = 0; // asteroid

// incarcare BMP 24-bit
GLuint loadBMP(const char* fileName)
{
    // deschide fisierul in mod "rb" ( read binary )
    FILE* f = NULL;
    fopen_s(&f, fileName, "rb");
    // tratam eroarea
    if (!f)
    {
        printf("Fisierul nu se poate deschide: %s\n", fileName);
        return 0;
    }

    // dimensiuni pentru imagine BMP
    unsigned char header[54];
    fread(header, 1, 54, f);

    // extrage latime, inaltime si dataOffSet ( unde incep pixelii in fisier )
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int dataOffset = *(int*)&header[10];

    // verificam daca BMP este de 24-bit
    short bitsPerPixel = *(short*)&header[28];
    if (bitsPerPixel != 24)
    {
        printf("Imaginea %s NU este 24-bit!\n", fileName);
        fclose(f);
        return 0;
    }

    // fiecare linie BMP trebuie sa aiba marime multiplu de 4 bytes 
    int rowPadded = (width * 3 + 3) & (~3);     // fortam rotunjirea la multiplu de 4
    int imageSize = width * height * 3;

    // alocam memorie pentru imagine
    unsigned char* data = (unsigned char*)malloc(imageSize);
    unsigned char* row = (unsigned char*)malloc(rowPadded);

    // mutam cursorul la inceputul pixelilor
    fseek(f, dataOffset, SEEK_SET);

    // BMP-ul e stocat de jos in sus
    for (int y = 0; y < height; y++)
    {
        fread(row, 1, rowPadded, f);
        int dstIndex = (height - 1 - y) * width * 3;
        for (int x = 0; x < width * 3; x++)
            data[dstIndex + x] = row[x];
    }

    fclose(f);
    free(row);

    GLuint texID;
    // textura OpenGL
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // selectam parametrii texturii
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // stocam imaginea
    glTexImage2D(GL_TEXTURE_2D, 0,
        GL_RGB, width, height,
        0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);  // forma BMP = BGR

    // eliberam memoria
    free(data);
    // id-ul texturii
    return texID;
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // fundal negru

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // activam modul de texturare GL_TEXTURE_2D
    glEnable(GL_TEXTURE_2D);

    // incarcare imagini
    asteroidTexID = loadBMP("asteroid.bmp");
    satTexID = loadBMP("satelit.bmp");

    if (asteroidTexID == 0)
        printf("Atentie: asteroid.bmp nu a fost incarcata corect!\n");
    if (satTexID == 0)
        printf("Atentie: satelit.bmp nu a fost incarcata corect!\n");
}

void dreptunghi(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // deplasare verticala
    glTranslated(400.0, i, 0.0);

    // ASTEROID
    glBindTexture(GL_TEXTURE_2D, asteroidTexID);
    glColor3f(1.0f, 1.0f, 1.0f);

    int rx = 40; // jumatate latime
    int ry = 24; // jumatate inaltime

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-rx, -ry);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(rx, -ry);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(rx, ry);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-rx, ry);
    glEnd();

    // SATELIT 
    glPushMatrix();
    glRotated(j, 0.0, 0.0, 1.0);
    glTranslated(0.0, 90.0, 0.0);  // raza orbitei

    glBindTexture(GL_TEXTURE_2D, satTexID);
    glColor3f(1.0f, 1.0f, 1.0f);

    int hsx = 25;
    int hsy = 18;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-hsx, -hsy);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(hsx, -hsy);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(hsx, hsy);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-hsx, hsy);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20.0, 780.0, 0.0, 600.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// miscare in sus/jos + rotatie
void miscad(void)
{
    i = i + step;
    if (i > 550.0)
        step = -alpha;
    else if (i < 0.0)
        step = alpha;

    j = j + step;
    glutPostRedisplay();
}

void miscas(void)
{
    i = i + step;
    if (i < 0.0)
        step = alpha;
    else if (i > 550.0)
        step = -alpha;

    j = j + step;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            step = -alpha;
            glutIdleFunc(miscas);
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            step = alpha;
            glutIdleFunc(miscad);
        }
        break;
    default:
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tema 3 - Asteroid si satelit texturat");

    init();
    glutDisplayFunc(dreptunghi);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
