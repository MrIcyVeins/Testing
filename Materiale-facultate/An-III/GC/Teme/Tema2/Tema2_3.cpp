#include <GL/glut.h>
#include <math.h>

// dimensiune fereastra
int winWidth = 900, winHeight = 600;

// display lists
GLuint buildingList;
GLuint lampFarList;

// ----------------- text (optional) -----------------
void drawText(float x, float y, const char* t)
{
    glRasterPos2f(x, y);
    while (*t)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *t++);
}

// ----------------- DISPLAY LIST: CLĂDIRE ÎN DEPARTARE -----------------
void buildBuildingList()
{
    buildingList = glGenLists(1);
    glNewList(buildingList, GL_COMPILE);

    // corpul cladirii (un dreptunghi ingust)
    glColor3f(0.06f, 0.06f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(-0.04f, 0.0f);
    glVertex2f( 0.04f, 0.0f);
    glVertex2f( 0.04f, 0.5f);
    glVertex2f(-0.04f, 0.5f);
    glEnd();

    // ferestre luminate (puncte/quads mici)
    glColor3f(1.0f, 0.9f, 0.5f);
    float w = 0.015f, h = 0.03f;
    for (float y = 0.05f; y < 0.48f; y += 0.08f)
    {
        for (float x = -0.03f; x <= 0.03f; x += 0.03f)
        {
            glBegin(GL_QUADS);
            glVertex2f(x,       y);
            glVertex2f(x + w,   y);
            glVertex2f(x + w,   y + h);
            glVertex2f(x,       y + h);
            glEnd();
        }
    }

    glEndList();
}

// ----------------- DISPLAY LIST: FELINAR ÎN DEPARTARE -----------------
void buildLampFarList()
{
    lampFarList = glGenLists(1);
    glNewList(lampFarList, GL_COMPILE);

    // stâlp simplu
    glColor3f(0.7f, 0.7f, 0.8f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, -0.1f);
    glVertex2f(0.0f,  0.1f);
    glEnd();

    // o bara mica orizontala
    glBegin(GL_LINES);
    glVertex2f(-0.03f, 0.09f);
    glVertex2f( 0.03f, 0.09f);
    glEnd();

    // bec mic
    glColor3f(1.0f, 0.9f, 0.6f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.11f);
    float R = 0.015f;
    for (int i = 0; i <= 16; ++i)
    {
        float a = 2.0f * 3.1415926f * i / 16;
        glVertex2f(0.0f + R * cosf(a), 0.11f + R * sinf(a));
    }
    glEnd();

    glEndList();
}

// ----------------- CER + STELE + LUNĂ -----------------
void drawSky()
{
    // cer gradient
    glBegin(GL_QUADS);
    glColor3f(0.04f, 0.03f, 0.15f); // jos, aproape de orizont
    glVertex2f(-1.0f, 0.0f);
    glVertex2f( 1.0f, 0.0f);
    glColor3f(0.01f, 0.01f, 0.05f); // sus, foarte inchis
    glVertex2f( 1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // stele
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glColor3f(0.9f, 0.9f, 1.0f);
    float sx[] = {-0.9f,-0.8f,-0.7f,-0.5f,-0.3f,-0.1f,0.1f,0.3f,0.5f,0.7f,0.9f,
                  -0.85f,-0.65f,-0.45f,-0.25f,-0.05f,0.15f,0.35f,0.55f,0.75f};
    float sy[] = {0.9f,0.8f,0.85f,0.78f,0.82f,0.88f,0.86f,0.9f,0.83f,0.87f,0.84f,
                  0.7f,0.65f,0.68f,0.72f,0.69f,0.66f,0.71f,0.67f,0.7f};
    for (int i = 0; i < 20; ++i)
        glVertex2f(sx[i], sy[i]);
    glEnd();

    // lună
    glColor3f(0.98f, 0.97f, 0.85f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-0.7f, 0.75f);
    float R = 0.10f;
    for (int i = 0; i <= 40; ++i)
    {
        float a = 2.0f * 3.1415926f * i / 40;
        glVertex2f(-0.7f + R * cosf(a), 0.75f + R * sinf(a));
    }
    glEnd();
}

// ----------------- ORAȘ ÎN DEPARTARE -----------------
void drawCity()
{
    // bandă de "teren" pe care stau cladirile
    glBegin(GL_QUADS);
    glColor3f(0.02f, 0.02f, 0.05f);
    glVertex2f(-1.0f, -0.05f);
    glVertex2f( 1.0f, -0.05f);
    glVertex2f( 1.0f,  0.0f);
    glVertex2f(-1.0f,  0.0f);
    glEnd();

    // clădiri repetate pe orizont
    for (float x = -0.95f; x <= 0.95f; x += 0.12f)
    {
        glPushMatrix();
        glTranslatef(x, -0.05f, 0.0f);

        // inaltimi usor diferite
        float hScale = 0.5f + 0.5f * (0.5f + 0.5f * sinf(5.0f * x));
        glScalef(1.0f, hScale, 1.0f);

        glCallList(buildingList);
        glPopMatrix();
    }
}

// ----------------- TROTUAR (BALUSTRADĂ) -----------------
void drawSidewalk()
{
    // bandă îngustă de trotuar
    glBegin(GL_QUADS);
    glColor3f(0.10f, 0.10f, 0.18f);
    glVertex2f(-1.0f, -0.35f);
    glVertex2f( 1.0f, -0.35f);
    glVertex2f( 1.0f, -0.25f);
    glVertex2f(-1.0f, -0.25f);
    glEnd();

    // balustradă (linie întunecată)
    glColor3f(0.05f, 0.05f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.25f);
    glVertex2f( 1.0f, -0.25f);
    glVertex2f( 1.0f, -0.20f);
    glVertex2f(-1.0f, -0.20f);
    glEnd();
}

// ----------------- FELINARE LA DISTANȚĂ -----------------
void drawFarLamps()
{
    // mici, repetate pe balustradă
    for (float x = -0.9f; x <= 0.9f; x += 0.3f)
    {
        glPushMatrix();
        glTranslatef(x, -0.20f, 0.0f);   // „stau” pe balustradă
        glScalef(0.4f, 0.4f, 1.0f);      // foarte mici, departe
        glCallList(lampFarList);
        glPopMatrix();
    }
}

// ----------------- APA -----------------
void drawWater()
{
    glBegin(GL_QUADS);
    glColor3f(0.03f, 0.04f, 0.18f); // sus, langa trotuar
    glVertex2f(-1.0f, -0.35f);
    glVertex2f( 1.0f, -0.35f);
    glColor3f(0.01f, 0.02f, 0.08f); // jos
    glVertex2f( 1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // cateva reflexii aproximative de lumini pe apa
    glColor3f(0.4f, 0.4f, 0.7f);
    glBegin(GL_LINES);
    for (float x = -0.8f; x <= 0.8f; x += 0.2f)
    {
        glVertex2f(x, -0.40f);
        glVertex2f(x, -0.80f);
    }
    glEnd();
}

// ----------------- DISPLAY -----------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawCity();
    drawSidewalk();
    drawFarLamps();
    drawWater();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.95f, 0.9f, "Noapte la malul apei - oraș indepartat si felinare pe trotuar");

    glutSwapBuffers();
}

// ----------------- RESHAPE -----------------
void reshape(int w, int h)
{
    winWidth = w;
    winHeight = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ----------------- INIT -----------------
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    buildBuildingList();
    buildLampFarList();
}

// ----------------- MAIN -----------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Noapte - oras in departare si apa in prim-plan");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
