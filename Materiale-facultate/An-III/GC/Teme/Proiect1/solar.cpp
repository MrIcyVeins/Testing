#include <GL/glut.h>
#include <math.h>
#include <stdlib.h> 
#include <time.h>     // time
#include <stdio.h>    // printf, FILE

#define PI 3.14159265f

// Structura care descrie o planetă
struct Planeta {
    const char* nume;
    float razaOrbitei;
    float razaPlanetei;   // dimensiunea planetei
    float unghiDeg;       // unghiul curent
    float viteza;         // viteza de rotatie
};

// Planete: Mercur, Venus, Pamant, Marte, Jupiter, Saturn, Uranus, Neptun
Planeta planete[] = {
    {"Mercur", 0.15f, 0.020f,  10.0f, 0.40f},
    {"Venus",  0.22f, 0.028f,  60.0f, 0.30f},
    {"Pamant", 0.29f, 0.030f, 120.0f, 0.25f},
    {"Marte",  0.36f, 0.025f, 190.0f, 0.20f}, // gap
    {"Jupiter",0.55f, 0.060f, 250.0f, 0.15f},
    {"Saturn", 0.72f, 0.055f, 310.0f, 0.12f}, // gap
    {"Uranus", 0.89f, 0.040f,  45.0f, 0.09f},
    {"Neptun", 1.06f, 0.040f, 135.0f, 0.07f}
};

const int nrPlanete = sizeof(planete) / sizeof(Planeta);

// texturi pentru planete
GLuint texturiPlanete[nrPlanete];
const char* fisiereTexturiPlanete[nrPlanete] = {
    "mercury.bmp",
    "venus.bmp",
    "earth.bmp",
    "mars.bmp",
    "jupiter.bmp",
    "saturn.bmp",
    "uranus.bmp",
    "neptune.bmp"
};

// textura soare
GLuint texturaSoare;
const char* fisierTexturaSoare = "sun.bmp";

// stele fundal
const int NR_STELE = 150;
float steaX[NR_STELE];
float steaY[NR_STELE];

// dimensiunea ferestrei
int latFereastra = 900;
int inaltimeFereastra = 900;

//
//  Incarcare imagini BMP
// ======================================================================
GLuint incarcaBMP(const char* numeFisier)
{
    FILE* f = NULL;
#ifdef _MSC_VER
    fopen_s(&f, numeFisier, "rb");
#else
    f = fopen(numeFisier, "rb");
#endif
    if (!f)
    {
        printf("Fisierul nu se poate deschide: %s\n", numeFisier);
        return 0;
    }

    unsigned char header[54];
    if (fread(header, 1, 54, f) != 54)
    {
        printf("Fisier BMP invalid (header prea scurt): %s\n", numeFisier);
        fclose(f);
        return 0;
    }

    // verificăm semnatura "BM"
    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Fisierul nu este BMP: %s\n", numeFisier);
        fclose(f);
        return 0;
    }

    int offsetDate = *(int*)&header[10];
    int latime = *(int*)&header[18];
    int inaltime = *(int*)&header[22];
    short bitiPePixel = *(short*)&header[28];

    if (bitiPePixel != 24 && bitiPePixel != 32)
    {
        printf("BMP cu %d bpp nu este suportat (doar 24 sau 32): %s\n",
            bitiPePixel, numeFisier);
        fclose(f);
        return 0;
    }

    int bytesPerPixel = bitiPePixel / 8;
    int bytesPeRand = latime * bytesPerPixel;
    int randCuPadding = (bytesPeRand + 3) & (~3);

    unsigned char* randBMP = (unsigned char*)malloc(randCuPadding);
    unsigned char* sursa = (unsigned char*)malloc(latime * inaltime * bytesPerPixel);

    if (!randBMP || !sursa)
    {
        printf("Nu exista suficienta memorie pentru imagine: %s\n", numeFisier);
        if (randBMP) free(randBMP);
        if (sursa)   free(sursa);
        fclose(f);
        return 0;
    }

    fseek(f, offsetDate, SEEK_SET); // ne intoarcem la inceputul datelor de imagine ( unde incep pixelii )

    // BMP stochează liniile de jos în sus
    for (int y = 0; y < inaltime; y++)
    {
        fread(randBMP, 1, randCuPadding, f);
        int indexDest = (inaltime - 1 - y) * bytesPeRand;
        for (int x = 0; x < bytesPeRand; x++)
        {
            sursa[indexDest + x] = randBMP[x];
        }
    }

    fclose(f);
    free(randBMP);

    // Construim buffer RGBA și eliminăm fundalul negru (color key)
    unsigned char* rgba = (unsigned char*)malloc(latime * inaltime * 4);
    if (!rgba)
    {
        printf("Nu exista memorie pentru RGBA: %s\n", numeFisier);
        free(sursa);
        return 0;
    }

    for (int i = 0; i < latime * inaltime; i++)
    {
        unsigned char b = sursa[i * bytesPerPixel + 0];
        unsigned char g = sursa[i * bytesPerPixel + 1];
        unsigned char r = sursa[i * bytesPerPixel + 2];
        unsigned char a = 255;

        // daca exista alpha original (32 bpp) îl putem citi
        if (bitiPePixel == 32)
        {
            unsigned char alphaOriginal = sursa[i * bytesPerPixel + 3];
            a = alphaOriginal;
        }

        // daca pixelul este aproape negru => tranparent
        if (r < 5 && g < 5 && b < 5)
        {
            a = 0;
        }

        rgba[i * 4 + 0] = r;
        rgba[i * 4 + 1] = g;
        rgba[i * 4 + 2] = b;
        rgba[i * 4 + 3] = a;
    }

    free(sursa); // eliberam bufferul

    // Textura RGBA
    GLuint idTextura;
    glGenTextures(1, &idTextura);
    glBindTexture(GL_TEXTURE_2D, idTextura);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(
        GL_TEXTURE_2D, 0,
        GL_RGBA,         // format intern
        latime, inaltime,
        0,
        GL_RGBA,         // format sursa (RGBA)
        GL_UNSIGNED_BYTE, // unsigned byte (0-255)
        rgba
    );

    free(rgba); // stergerea bufferului din ram

    return idTextura;
}

// 
//  Stele / desen cercuri / soare
// ======================================================================

// generează poziții aleatoare pentru stele în zona vizibilă
void genereazaStele(float xmin, float xmax, float ymin, float ymax)
{
    for (int i = 0; i < NR_STELE; i++)
    {
        float rx = (float)rand() / RAND_MAX; // [0,1]
        float ry = (float)rand() / RAND_MAX;

        //
        steaX[i] = xmin + rx * (xmax - xmin);
        steaY[i] = ymin + ry * (ymax - ymin);
    }
}

// --- DESENARE CERC (orbita) ---
void deseneazaCercContur(float raza, int segmente)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segmente; i++)
    {
        float a = 2.0f * PI * i / segmente;
        glVertex2f(cosf(a) * raza, sinf(a) * raza);
    }
    glEnd();
}

// --- DESENARE DISC PLIN ---
void deseneazaDiscPlin(float raza, int segmente)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= segmente; i++)
    {
        float a = 2.0f * PI * i / segmente;
        glVertex2f(cosf(a) * raza, sinf(a) * raza);
    }
    glEnd();
}

// --- DISC TEXTURAT (pentru planete / soare) ---
void deseneazaDiscTexturat(float raza, int segmente)
{
    glBegin(GL_TRIANGLE_FAN);

    // centru texturii
    glTexCoord2f(0.5f, 0.5f); // centrul imaginii BMP
    glVertex2f(0.0f, 0.0f); // centrul cercului pe ecran

    for (int i = 0; i <= segmente; i++)
    {
        float a = 2.0f * PI * i / segmente;
        float x = cosf(a) * raza;
        float y = sinf(a) * raza;

        // mapare cos/sin [-1,1] -> [0,1]
        float s = (cosf(a) + 1.0f) * 0.5f;
        float t = (sinf(a) + 1.0f) * 0.5f;

        glTexCoord2f(s, t);
        glVertex2f(x, y);
    }

    glEnd();
}

// deseneaza stelele de pe fundal
void deseneazaSteleFundal()
{
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NR_STELE; i++)
    {
        glColor3f(0.9f, 0.9f, 1.0f); // culoare stele fundal
        glVertex2f(steaX[i], steaY[i]);
    }
    glEnd();
}

// ---  SOARE CU TEXTURA
void deseneazaSoare()
{
    glPushMatrix();

    // Soarele texturat
    if (texturaSoare != 0)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaSoare);
        glColor3f(1.0f, 1.0f, 1.0f);  

        deseneazaDiscTexturat(0.12f, 60); // raza soarelui
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        // Daca nu se incarca textura
        glColor3f(1.0f, 0.9f, 0.0f);
        deseneazaDiscPlin(0.12f, 60);
    }

    glPopMatrix();
}

// desen text folosind coordonate de ecran (0..latFereastra, 0..inaltimeFereastra)
void deseneazaText2D(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    while (*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

// Functia de afisare
void afisare()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // --- scena (sistemul solar) ---
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // stele pe fundal
    deseneazaSteleFundal();

    // soare
    deseneazaSoare();

    // orbite tip "radar"
    glColor3f(0.2f, 0.5f, 0.3f);
    for (int i = 0; i < nrPlanete; i++)
        deseneazaCercContur(planete[i].razaOrbitei, 120);

    // PLANETE
    for (int i = 0; i < nrPlanete; i++)
    {
        Planeta& p = planete[i];

        glPushMatrix();

        // 1) rotatie în jurul soarelui (origine)
        glRotatef(p.unghiDeg, 0.0f, 0.0f, 1.0f);

        // 2) translatie pe axa X cu raza orbitei
        glTranslatef(p.razaOrbitei, 0.0f, 0.0f);

        if (texturiPlanete[i] != 0)
        {
            // PLANETA TEXTURATA
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texturiPlanete[i]);
            glColor3f(1.0f, 1.0f, 1.0f);
            deseneazaDiscTexturat(p.razaPlanetei, 40);
            glDisable(GL_TEXTURE_2D);
        }
        else
        {
            // IN CAZ CA NU SE INCARCA TEXTURILE: culoare simpla
            if (i == 0) glColor3f(0.6f, 0.6f, 0.6f);      // Mercur
            else if (i == 1) glColor3f(1.0f, 0.8f, 0.4f); // Venus
            else if (i == 2) glColor3f(0.2f, 0.4f, 1.0f); // Pământ
            else if (i == 3) glColor3f(1.0f, 0.3f, 0.2f); // Marte
            else if (i == 4) glColor3f(0.9f, 0.7f, 0.3f); // Jupiter
            else if (i == 5) glColor3f(0.9f, 0.8f, 0.5f); // Saturn
            else if (i == 6) glColor3f(0.5f, 0.8f, 0.9f); // Uranus
            else            glColor3f(0.3f, 0.5f, 1.0f);  // Neptun

            deseneazaDiscPlin(p.razaPlanetei, 40);
        }

        // Inel Saturn
        if (i == 5)
        {
            glColor3f(0.9f, 0.8f, 0.6f);
            float razaInel = p.razaPlanetei * 1.5f;
            deseneazaCercContur(razaInel, 80);
        }

        glPopMatrix();
    }

    // Text - colt
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, latFereastra, 0, inaltimeFereastra);   // coordonate în pixeli

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // text alb
    glColor3f(1.0f, 1.0f, 1.0f);

    float xText = 10.0f;
    float ySus = inaltimeFereastra - 25.0f;

    deseneazaText2D(xText, ySus, "Proiect - Sistem solar");
    deseneazaText2D(xText, ySus - 25.0f, "Georgescu Andrei");
    deseneazaText2D(xText, ySus - 45.0f, "Saftoiu Genica Liliana");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

// actualizeaza unghiurile planetelor (animatie)
void actualizare(int value)
{
    for (int i = 0; i < nrPlanete; i++)
    {
        planete[i].unghiDeg += planete[i].viteza;
        if (planete[i].unghiDeg > 360.0f)
            planete[i].unghiDeg -= 360.0f;
    }

    glutPostRedisplay(); // redesenarea ferestrei
    glutTimerFunc(16, actualizare, 0); // ~60 FPS (16 ms) // programeaza urmatorul  update 
}

// mentinem sistemul solar centrat si cercurile rotunde la resize
void redimensioneaza(int w, int h)
{
    latFereastra = w;
    inaltimeFereastra = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / (float)h;

    float xmin, xmax, ymin, ymax;

    if (aspect >= 1.0f) {
        // fereastra mai lata decat inalta
        xmin = -1.1f * aspect;
        xmax = 1.1f * aspect;
        ymin = -1.1f;
        ymax = 1.1f;
    }
    else {
        // fereastra mai înalta decât lata
        xmin = -1.1f;
        xmax = 1.1f;
        ymin = -1.1f / aspect;
        ymax = 1.1f / aspect;
    }

    gluOrtho2D(xmin, xmax, ymin, ymax);

    // regeneram stelele pentru intreaga zona vizibila
    genereazaStele(xmin, xmax, ymin, ymax);

    glMatrixMode(GL_MODELVIEW);
}

// initializare OpenGL / texturi / stele
void initializare()
{
    // fundal albastru inchis (spatiu)
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);

    // blending pentru alpha (texturi cu transparenta)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // antialiasing optional
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    srand((unsigned int)time(NULL));

    // incarcam texturile pentru fiecare planeta
    for (int i = 0; i < nrPlanete; i++)
    {
        texturiPlanete[i] = incarcaBMP(fisiereTexturiPlanete[i]);
        if (texturiPlanete[i] == 0)
        {
            printf("Atentie: textura pentru %s (%s) nu a fost incarcata!\n",
                planete[i].nume, fisiereTexturiPlanete[i]);
        }
    }

    // incarcam textura pentru soare
    texturaSoare = incarcaBMP(fisierTexturaSoare);
    if (texturaSoare == 0)
    {
        printf("Atentie: textura pentru Soare (%s) nu a fost incarcata!\n",
            fisierTexturaSoare);
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(latFereastra, inaltimeFereastra);
    glutCreateWindow("Sistem Solar 2D - Stil Radar");

    initializare();

    // apelam o data redimensionare pentru a seta proiectia si stelele initiale
    redimensioneaza(latFereastra, inaltimeFereastra);

    glutDisplayFunc(afisare); // apeleaza afisare() atunci cand cand fereastra trebuie redesenata
    glutTimerFunc(16, actualizare, 0); // apeleaza actualizare() la fiecare 16ms ~= 60 frame/s
    glutReshapeFunc(redimensioneaza);

    glutMainLoop();
    return 0;
}
