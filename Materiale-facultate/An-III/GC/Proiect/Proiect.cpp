#define NOMINMAX
#include <windows.h>
#include <gl/freeglut.h>
#include <cmath>
#include <cstdlib>

// ============================================================
// Camera (orbit in jurul centrului scenei)
// ============================================================
float tintaX = 0.0f, tintaY = 0.0f, tintaZ = 0.0f;
float alfaCamera = 0.35f, betaCamera = 0.0f;   // alfa = sus/jos, beta = stanga/dreapta
float distCamera = 30.0f;
float camX, camY, camZ;

// Timp (secunde)
float timpSec = 0.0f;

// ============================================================
// Text overlay (stanga sus)
// ============================================================
int latimeFereastra = 900;
int inaltimeFereastra = 650;

const char* TEXT_L1 = "Scena 3D dinamica: Insula tropicala";
const char* TEXT_L2 = "Georgescu Andrei";
const char* TEXT_L3 = "Saftoiu Genica Liliana";

// ============================================================
// Parametri scena
// ============================================================
const float Z_OCEAN = 0.05f;
const float R_INSULA_BAZA = 18.0f;
const float R_NISIP_BAZA = 14.0f;

// Zona de tarm putin sub apa (pentru tranzitie)
const float LATIME_TARM = 2.2f;
const float Z_TARM = Z_OCEAN - 0.005f;

// Pozitia soarelui (si a luminii)
GLfloat pozSoare[4] = { 42.0f, -34.0f, 30.0f, 1.0f };

// Culoare ceata (mai neutra, ca sa nu coloreze nisipul in albastru)
GLfloat culoareCeata[4] = { 0.88f, 0.92f, 0.95f, 1.0f };

// ============================================================
// Functii ajutatoare
// ============================================================
static inline float Limiteaza(float v, float minV, float maxV)
{
    if (v < minV) return minV;
    if (v > maxV) return maxV;
    return v;
}

static inline float SmoothStep(float e0, float e1, float x)
{
    float t = Limiteaza((x - e0) / (e1 - e0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

// Setare culoare material (folosit cand LIGHTING este activ)
static inline void SeteazaMaterial(float r, float g, float b, float a = 1.0f)
{
    GLfloat col[4] = { r, g, b, a };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
}

// Random [0..1] folosind rand() (mai usor de explicat)
static inline float Random01()
{
    return (float)rand() / (float)RAND_MAX;
}

static inline void InPolare(float x, float y, float& r, float& theta)
{
    r = std::sqrt(x * x + y * y);
    theta = std::atan2(y, x);
}

// ============================================================
// Text 2D (overlay)
// ============================================================
void DeseneazaText2D(int x, int y, const char* text)
{
    glRasterPos2i(x, y);
    for (const char* p = text; *p; p++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
}

void DeseneazaOverlayText()
{
    // Overlay 2D peste scena
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, latimeFereastra, 0, inaltimeFereastra);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.0f, 0.0f, 0.0f);

    int margin = 12;
    int y = inaltimeFereastra - margin - 18;
    DeseneazaText2D(margin, y, TEXT_L1);
    y -= 22; DeseneazaText2D(margin, y, TEXT_L2);
    y -= 22; DeseneazaText2D(margin, y, TEXT_L3);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glColor4f(1, 1, 1, 1);
}

// ============================================================
// Proiectie
// ============================================================
void ReshapeSiProiectie(int w, int h)
{
    if (h == 0) h = 1;

    latimeFereastra = w;
    inaltimeFereastra = h;

    float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(60.0f, ratio, 0.1f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
// Iluminare + ceata
// ============================================================
void AplicaIluminareFaraPozitie()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);

    GLfloat ambientL[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat difuzL[4] = { 1.00f, 1.00f, 0.95f, 1.0f };
    GLfloat specL[4] = { 1.00f, 1.00f, 1.00f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientL);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuzL);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specL);

    GLfloat matSpec[4] = { 0.30f, 0.30f, 0.30f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 24.0f);
}

void AplicaCeata()
{
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, culoareCeata);

    // Ceata mai departe (sa nu coloreze insula)
    glFogf(GL_FOG_START, 55.0f);
    glFogf(GL_FOG_END, 220.0f);

    glHint(GL_FOG_HINT, GL_DONT_CARE);
}

// ============================================================
// Cer (gradient simplu)
// ============================================================
void DeseneazaCerGradient()
{
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    glColor3f(0.40f, 0.78f, 0.98f);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);

    glColor3f(0.78f, 0.95f, 0.99f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);

    glColor4f(1, 1, 1, 1);
}

// ============================================================
// Contur insula neregulat
// ============================================================
static inline float RazaInsula(float theta)
{
    float r = R_INSULA_BAZA
        + 1.6f * std::sinf(3.0f * theta + 0.3f)
        + 0.9f * std::sinf(7.0f * theta + 1.1f)
        + 0.5f * std::cosf(11.0f * theta - 0.7f);

    return Limiteaza(r, R_INSULA_BAZA - 3.0f, R_INSULA_BAZA + 3.0f);
}

static inline float RazaNisip(float theta)
{
    float r = R_NISIP_BAZA
        + 1.0f * std::sinf(3.0f * theta + 0.8f)
        + 0.6f * std::cosf(6.0f * theta - 0.4f);

    float maxAdmis = RazaInsula(theta) - 2.0f;
    return Limiteaza(r, R_NISIP_BAZA - 2.5f, maxAdmis);
}

// Inaltimea insulei (deal + tranzitie spre nisip + tarm)
float InaltimeInsula(float x, float y)
{
    float r, theta;
    InPolare(x, y, r, theta);

    float rTarm = RazaInsula(theta);
    float rNisip = RazaNisip(theta);

    float rn = r / rTarm;
    float h = 6.0f * std::exp(-(rn * rn) * 2.2f);

    // mica rugozitate
    h += 0.18f * std::sinf(0.35f * x + 0.2f) * std::cosf(0.30f * y - 0.1f);

    // zona nisip
    const float hPlaja = 0.20f;
    float tNisip = SmoothStep(rNisip - 2.8f, rNisip, r);
    h = h * (1.0f - tNisip) + hPlaja * tNisip;

    // zona tarm (aproape de apa)
    float tTarm = SmoothStep(rTarm - LATIME_TARM, rTarm, r);
    h = h * (1.0f - tTarm) + Z_TARM * tTarm;

    return h;
}

// Normala aproximata prin diferente finite
void NormalaInsula(float x, float y, float& nx, float& ny, float& nz)
{
    const float e = 0.2f;
    float hL = InaltimeInsula(x - e, y);
    float hR = InaltimeInsula(x + e, y);
    float hD = InaltimeInsula(x, y - e);
    float hU = InaltimeInsula(x, y + e);

    nx = -(hR - hL);
    ny = -(hU - hD);
    nz = 2.0f * e;

    float len = std::sqrt(nx * nx + ny * ny + nz * nz);
    if (len < 1e-6f) { nx = 0; ny = 0; nz = 1; return; }
    nx /= len; ny /= len; nz /= len;
}

// ============================================================
// Insula (culori brute: verde + galben, fara iluminare)
// ============================================================
void DeseneazaInsulaCuloriSafe()
{
    glDisable(GL_CULL_FACE);

    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    const int N = 95;
    const float S = 22.0f;
    const float pas = (2.0f * S) / N;

    const float verdeR = 0.12f, verdeG = 0.65f, verdeB = 0.18f;
    const float galbenR = 0.98f, galbenG = 0.92f, galbenB = 0.45f;

    for (int j = 0; j < N; j++) {
        float y0 = -S + j * pas;
        float y1 = y0 + pas;

        bool deschis = false;

        for (int i = 0; i <= N; i++) {
            float x = -S + i * pas;

            float r0, th0; InPolare(x, y0, r0, th0);
            float r1, th1; InPolare(x, y1, r1, th1);

            float rTarm0 = RazaInsula(th0);
            float rTarm1 = RazaInsula(th1);

            bool inInterior = (r0 <= rTarm0) && (r1 <= rTarm1);

            if (inInterior) {
                if (!deschis) { glBegin(GL_TRIANGLE_STRIP); deschis = true; }

                float h0 = InaltimeInsula(x, y0);
                float h1 = InaltimeInsula(x, y1);

                float rNisip0 = RazaNisip(th0);
                float rNisip1 = RazaNisip(th1);

                float tNisip0 = SmoothStep(rNisip0 - 2.8f, rNisip0 + 0.2f, r0);
                float tNisip1 = SmoothStep(rNisip1 - 2.8f, rNisip1 + 0.2f, r1);

                float nx, ny, nz;

                // v0
                NormalaInsula(x, y0, nx, ny, nz);
                glNormal3f(nx, ny, nz);

                glColor3f(verdeR * (1.0f - tNisip0) + galbenR * tNisip0,
                    verdeG * (1.0f - tNisip0) + galbenG * tNisip0,
                    verdeB * (1.0f - tNisip0) + galbenB * tNisip0);
                glVertex3f(x, y0, h0);

                // v1
                NormalaInsula(x, y1, nx, ny, nz);
                glNormal3f(nx, ny, nz);

                glColor3f(verdeR * (1.0f - tNisip1) + galbenR * tNisip1,
                    verdeG * (1.0f - tNisip1) + galbenG * tNisip1,
                    verdeB * (1.0f - tNisip1) + galbenB * tNisip1);
                glVertex3f(x, y1, h1);
            }
            else if (deschis) {
                glEnd();
                deschis = false;
            }
        }

        if (deschis) glEnd();
    }

    glEnable(GL_CULL_FACE);
    glColor4f(1, 1, 1, 1);
}

// ============================================================
// Ocean simplu
// ============================================================
void DeseneazaOcean()
{
    glDisable(GL_CULL_FACE);

    SeteazaMaterial(0.10f, 0.45f, 0.75f, 1.0f);
    glNormal3f(0, 0, 1);

    const float S = 90.0f;
    glBegin(GL_QUADS);
    glVertex3f(-S, -S, Z_OCEAN);
    glVertex3f(S, -S, Z_OCEAN);
    glVertex3f(S, S, Z_OCEAN);
    glVertex3f(-S, S, Z_OCEAN);
    glEnd();

    glEnable(GL_CULL_FACE);
}

// ============================================================
// Spuma la tarm (blending)
// ============================================================
void DeseneazaSpumaTarm()
{
    const int K = 240;

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, 0.18f);
    glNormal3f(0, 0, 1);

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= K; i++) {
        float a = (2.0f * 3.1415926f) * (i / (float)K);
        float ca = std::cos(a), sa = std::sin(a);

        float rTarm = RazaInsula(a);
        float rNisip = RazaNisip(a);

        float minR = rNisip + 0.25f;
        float maxR = rTarm - 0.22f;

        float dorit = 1.10f;
        float maxBand = maxR - minR;
        float band = Limiteaza(dorit, 0.25f, maxBand * 0.90f);

        float zgomot = 0.18f * std::sinf(5.0f * a + 0.6f) + 0.10f * std::cosf(9.0f * a - 1.1f);
        float r0 = (rTarm - 0.95f) + zgomot;
        r0 = Limiteaza(r0, minR, maxR - band);
        float r1 = r0 + band;

        float x0 = r0 * ca, y0 = r0 * sa;
        float x1 = r1 * ca, y1 = r1 * sa;

        float z0 = InaltimeInsula(x0, y0) + 0.03f;
        float z1 = InaltimeInsula(x1, y1) + 0.03f;

        glVertex3f(x0, y0, z0);
        glVertex3f(x1, y1, z1);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glColor4f(1, 1, 1, 1);
}

// ============================================================
// Soare (obiect vizual)
// ============================================================
void DeseneazaSoare()
{
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.95f, 0.6f);

    glPushMatrix();
    glTranslatef(pozSoare[0], pozSoare[1], pozSoare[2]);
    glutSolidSphere(1.4, 18, 18);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glColor4f(1, 1, 1, 1);
}

// ============================================================
// Palmieri (parametri generati o singura data)
// ============================================================
struct ParamFrunza
{
    float unghiDeg;
    float faza;
    float amp;
    float pitchSus;
    float L, W, cazatura;
};

struct ParamPalmier
{
    float x, y;
    float inaltimeTrunchi, razaTrunchi;
    float inclinX, inclinY, rotZ;
    float fazaVant, ampVant, vitezaVant;
    float scala;

    float razaPata;
    float alfaPata;

    ParamFrunza frunze[7];
};

const int NR_PALMIERI = 10;
ParamPalmier palmieri[NR_PALMIERI];

void DeseneazaPalmier(const ParamPalmier& p)
{
    float z = InaltimeInsula(p.x, p.y);

    glPushMatrix();
    glTranslatef(p.x, p.y, z + 0.02f);
    glScalef(p.scala, p.scala, p.scala);

    glRotatef(p.rotZ, 0, 0, 1);
    glRotatef(p.inclinX, 1, 0, 0);
    glRotatef(p.inclinY, 0, 1, 0);

    // Trunchi
    SeteazaMaterial(0.55f, 0.38f, 0.20f, 1.0f);
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, p.razaTrunchi * 1.05f, p.razaTrunchi * 0.75f, p.inaltimeTrunchi, 10, 4);
    glTranslatef(0.0f, 0.0f, p.inaltimeTrunchi);

    // Vant global
    float w1 = std::sinf(timpSec * p.vitezaVant + p.fazaVant);
    float w2 = std::sinf(timpSec * (p.vitezaVant * 0.63f) + p.fazaVant * 1.7f);
    float vant = 0.65f * w1 + 0.35f * w2;
    float vantDeg = p.ampVant * vant;

    // Frunze
    SeteazaMaterial(0.10f, 0.55f, 0.18f, 1.0f);

    for (int i = 0; i < 7; i++) {
        const ParamFrunza& f = p.frunze[i];

        float twist = 2.0f * std::sinf(timpSec * (p.vitezaVant * 1.2f) + f.faza);
        float vantFrunza = f.amp * std::sinf(timpSec * (p.vitezaVant * 1.35f) + f.faza);

        glPushMatrix();
        glRotatef(f.unghiDeg, 0, 0, 1);
        glRotatef(f.pitchSus + vantDeg + vantFrunza, 1, 0, 0);
        glRotatef(twist, 0, 0, 1);

        glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 0.3f, 0.95f);

        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-f.W, f.L * 0.55f, -f.cazatura * 0.55f);
        glVertex3f(0.0f, f.L, -f.cazatura);

        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(f.W, f.L * 0.55f, -f.cazatura * 0.55f);
        glVertex3f(0.0f, f.L, -f.cazatura);
        glEnd();

        glPopMatrix();
    }

    gluDeleteQuadric(q);
    glPopMatrix();
}

void DeseneazaPalmieri()
{
    for (int i = 0; i < NR_PALMIERI; i++)
        DeseneazaPalmier(palmieri[i]);
}

// Pata de contact (umbra fake) sub palmieri
void DeseneazaPataContact(float x, float y, float raza, float alfaCentru)
{
    float z = InaltimeInsula(x, y) + 0.04f;

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0, 0, 0, alfaCentru);
    glVertex3f(x, y, z);

    const int K = 26;
    for (int i = 0; i <= K; i++) {
        float a = (2.0f * 3.1415926f) * (i / (float)K);
        float px = x + raza * std::cos(a);
        float py = y + raza * std::sin(a);
        float pz = InaltimeInsula(px, py) + 0.04f;

        glColor4f(0, 0, 0, 0);
        glVertex3f(px, py, pz);
    }
    glEnd();
}

void DeseneazaContactPalmieri()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glNormal3f(0, 0, 1);

    for (int i = 0; i < NR_PALMIERI; i++) {
        const ParamPalmier& p = palmieri[i];
        DeseneazaPataContact(p.x, p.y, p.razaPata * 1.15f, p.alfaPata * 0.70f);
        DeseneazaPataContact(p.x, p.y, p.razaPata * 0.75f, p.alfaPata);
    }

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glColor4f(1, 1, 1, 1);
}

// ============================================================
// Barca de pescuit
// ============================================================
void DeseneazaBarcaPescuit()
{
    float a = -0.65f;
    float rTarm = RazaInsula(a);
    float r = rTarm + 4.0f;
    float x = r * std::cos(a);
    float y = r * std::sin(a);
    float z = Z_OCEAN + 0.02f;

    glPushMatrix();
    glTranslatef(x, y, z);

    float yawDeg = (a * 180.0f / 3.1415926f) + 90.0f;
    glRotatef(yawDeg, 0, 0, 1);

    float roll = 3.0f * std::sinf(timpSec * 0.9f);
    glRotatef(roll, 1, 0, 0);

    SeteazaMaterial(0.35f, 0.18f, 0.08f, 1.0f);
    glPushMatrix();
    glScalef(2.2f, 0.75f, 0.35f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.1f, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    glutSolidCone(0.38, 0.7, 16, 4);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.1f, 0.0f, 0.0f);
    glRotatef(-90, 0, 1, 0);
    glutSolidCone(0.38, 0.7, 16, 4);
    glPopMatrix();

    SeteazaMaterial(0.55f, 0.34f, 0.18f, 1.0f);
    glPushMatrix();
    glTranslatef(0, 0, 0.12f);
    glScalef(1.6f, 0.55f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    SeteazaMaterial(0.45f, 0.45f, 0.45f, 1.0f);
    GLUquadric* q = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.1f, 0.0f, 0.25f);
    gluCylinder(q, 0.04, 0.03, 0.9, 10, 2);
    glTranslatef(0, 0, 0.85f);

    GLboolean wasCull = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    glDisable(GL_LIGHTING);
    glColor3f(0.95f, 0.15f, 0.15f);

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.12f);
    glVertex3f(0.55f, 0.0f, 0.08f);
    glVertex3f(0.55f, 0.0f, -0.08f);
    glVertex3f(0.0f, 0.0f, -0.12f);
    glEnd();

    glEnable(GL_LIGHTING);
    glColor4f(1, 1, 1, 1);

    if (wasCull) glEnable(GL_CULL_FACE);

    glPopMatrix();
    gluDeleteQuadric(q);

    glPopMatrix();
}

// ============================================================
// Ruine varf
// ============================================================
void DeseneazaRuineVarf()
{
    float cx = 0.0f, cy = 0.0f;

    struct P { float x, y; };
    P pts[] = { {0,0},{0.9f,-0.2f},{-0.6f,0.7f},{0.2f,1.1f},{-1.0f,-0.6f} };

    auto Piatra = [&](float k = 1.0f) {
        SeteazaMaterial(0.55f * k, 0.56f * k, 0.58f * k, 1.0f);
        };

    GLUquadric* q = gluNewQuadric();
    gluQuadricNormals(q, GLU_SMOOTH);

    {
        float x = cx + pts[0].x, y = cy + pts[0].y;
        float z = InaltimeInsula(x, y);

        glPushMatrix();
        glTranslatef(x, y, z - 0.02f);
        glRotatef(10.0f, 1, 0, 0);
        glRotatef(-20.0f, 0, 1, 0);

        Piatra(1.0f);
        gluCylinder(q, 0.28, 0.24, 2.2, 16, 5);

        Piatra(1.05f);
        glTranslatef(0, 0, 2.15f);
        glScalef(0.75f, 0.75f, 0.25f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    {
        float x = cx + pts[1].x, y = cy + pts[1].y;
        float z = InaltimeInsula(x, y);

        glPushMatrix();
        glTranslatef(x, y, z - 0.02f);
        glRotatef(-18.0f, 1, 0, 0);
        glRotatef(25.0f, 0, 1, 0);

        Piatra(0.98f);
        gluCylinder(q, 0.24, 0.20, 1.55, 16, 4);

        Piatra(1.03f);
        glTranslatef(0, 0, 1.50f);
        glScalef(0.65f, 0.65f, 0.22f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    {
        float x = cx + 0.45f, y = cy - 0.10f;
        float z = InaltimeInsula(x, y);

        glPushMatrix();
        glTranslatef(x, y, z + 0.15f);
        glRotatef(35.0f, 0, 0, 1);
        glRotatef(12.0f, 1, 0, 0);

        Piatra(0.95f);
        glScalef(1.6f, 0.35f, 0.25f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    auto Bloc = [&](float x, float y, float sx, float sy, float sz, float rotZ)
        {
            float z = InaltimeInsula(x, y);
            glPushMatrix();
            glTranslatef(x, y, z - 0.01f);
            glRotatef(rotZ, 0, 0, 1);
            Piatra(0.92f);
            glScalef(sx, sy, sz);
            glutSolidCube(1.0);
            glPopMatrix();
        };

    Bloc(cx + pts[2].x, cy + pts[2].y, 0.65f, 0.45f, 0.35f, 22.0f);
    Bloc(cx + pts[3].x, cy + pts[3].y, 0.50f, 0.60f, 0.25f, -15.0f);
    Bloc(cx + pts[4].x, cy + pts[4].y, 0.80f, 0.40f, 0.30f, 40.0f);

    gluDeleteQuadric(q);
}

// ============================================================
// Cabana pescuit
// ============================================================
void DeseneazaCabanaPescuit()
{
    float x = 10.0f;
    float y = -6.5f;

    float r, th; InPolare(x, y, r, th);
    float rTarm = RazaInsula(th);
    float rNisip = RazaNisip(th);
    float maxR = rTarm - 0.9f;
    float minR = rNisip + 0.6f;

    if (r > maxR) { float k = maxR / r; x *= k; y *= k; }
    if (r < minR) { float k = minR / (r + 1e-6f); x *= k; y *= k; }

    float zSol = InaltimeInsula(x, y);

    glPushMatrix();
    glTranslatef(x, y, zSol + 0.02f);
    glRotatef(35.0f, 0, 0, 1);

    const float zPodea = 1.00f;
    const float tPodea = 0.15f;
    const float zPereti = 1.45f;
    const float hPereti = 0.90f;
    const float zSusPereti = zPereti + 0.5f * hPereti;
    const float zAcoperis = zSusPereti + 0.02f;

    SeteazaMaterial(0.45f, 0.30f, 0.18f, 1.0f);
    GLUquadric* q = gluNewQuadric();

    auto Stalp = [&](float px, float py) {
        glPushMatrix();
        glTranslatef(px, py, -0.10f);
        gluCylinder(q, 0.10, 0.09, 1.10, 10, 3);
        glPopMatrix();
        };

    Stalp(-1.0f, -0.7f);
    Stalp(1.0f, -0.7f);
    Stalp(-1.0f, 0.7f);
    Stalp(1.0f, 0.7f);

    SeteazaMaterial(0.55f, 0.38f, 0.22f, 1.0f);
    glPushMatrix();
    glTranslatef(0, 0, zPodea);
    glScalef(2.4f, 1.8f, tPodea);
    glutSolidCube(1.0);
    glPopMatrix();

    SeteazaMaterial(0.60f, 0.43f, 0.25f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, zPereti);
    glScalef(1.8f, 1.3f, hPereti);
    glutSolidCube(1.0);
    glPopMatrix();

    GLboolean wasCull = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    SeteazaMaterial(0.35f, 0.22f, 0.14f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, zAcoperis);

    float L = 2.20f;
    float W = 1.70f;
    float H = 0.80f;

    glBegin(GL_TRIANGLES);
    glNormal3f(0, -1, 0.4f);
    glVertex3f(-L * 0.5f, -W * 0.5f, 0.0f);
    glVertex3f(L * 0.5f, -W * 0.5f, 0.0f);
    glVertex3f(0.0f, -W * 0.5f, H);
    glEnd();

    glBegin(GL_TRIANGLES);
    glNormal3f(0, 1, 0.4f);
    glVertex3f(-L * 0.5f, W * 0.5f, 0.0f);
    glVertex3f(0.0f, W * 0.5f, H);
    glVertex3f(L * 0.5f, W * 0.5f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0.35f);
    glVertex3f(-L * 0.5f, -W * 0.5f, 0.0f);
    glVertex3f(-L * 0.5f, W * 0.5f, 0.0f);
    glVertex3f(0.0f, W * 0.5f, H);
    glVertex3f(0.0f, -W * 0.5f, H);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0.35f);
    glVertex3f(L * 0.5f, -W * 0.5f, 0.0f);
    glVertex3f(0.0f, -W * 0.5f, H);
    glVertex3f(0.0f, W * 0.5f, H);
    glVertex3f(L * 0.5f, W * 0.5f, 0.0f);
    glEnd();

    glPopMatrix();

    if (wasCull) glEnable(GL_CULL_FACE);

    SeteazaMaterial(0.52f, 0.36f, 0.20f, 1.0f);

    const float lungRampa = 3.0f;
    const float latRampa = 0.65f;
    const float grosRampa = 0.10f;
    const float pitchDeg = 28.0f;

    const float balamaX = 1.25f;
    const float balamaY = -0.15f;
    const float balamaZ = zPodea + 0.5f * tPodea;

    glPushMatrix();
    glTranslatef(balamaX, balamaY, balamaZ);
    glRotatef(pitchDeg, 0, 1, 0);
    glTranslatef(lungRampa * 0.5f, 0.0f, -grosRampa * 0.5f);
    glScalef(lungRampa, latRampa, grosRampa);
    glutSolidCube(1.0);
    glPopMatrix();

    SeteazaMaterial(0.45f, 0.30f, 0.18f, 1.0f);

    glPushMatrix();
    glTranslatef(balamaX + 2.7f, balamaY - 0.25f, -0.02f);
    gluCylinder(q, 0.06, 0.05, 0.55, 10, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(balamaX + 2.7f, balamaY + 0.25f, -0.02f);
    gluCylinder(q, 0.06, 0.05, 0.55, 10, 2);
    glPopMatrix();

    gluDeleteQuadric(q);
    glPopMatrix();
}

// ============================================================
// Generare palmieri (o singura data)
// ============================================================
void GenereazaPalmieri()
{
    for (int i = 0; i < NR_PALMIERI; i++) {
        ParamPalmier& p = palmieri[i];

        float ang = 2.0f * 3.1415926f * Random01();
        float ca = std::cos(ang), sa = std::sin(ang);

        float rTarm = RazaInsula(ang);
        float rNisip = RazaNisip(ang);

        float rMin = rNisip - 1.8f;
        float rMax = rTarm - 2.8f;
        if (rMax < rMin + 0.5f) rMax = rMin + 0.5f;

        float rr = rMin + (rMax - rMin) * Random01();
        p.x = rr * ca;
        p.y = rr * sa;

        float r, th; InPolare(p.x, p.y, r, th);
        float aproapeTarm = SmoothStep(rTarm - 2.0f, rTarm - 0.6f, r);
        p.scala = 1.0f - 0.35f * aproapeTarm;

        p.inaltimeTrunchi = 1.4f + 0.6f * Random01();
        p.razaTrunchi = 0.10f + 0.04f * Random01();
        p.inclinX = -10.0f + 20.0f * Random01();
        p.inclinY = -10.0f + 20.0f * Random01();
        p.rotZ = 360.0f * Random01();

        p.fazaVant = 6.2831853f * Random01();
        p.ampVant = 6.0f + 6.0f * Random01();
        p.vitezaVant = 0.7f + 0.5f * Random01();

        p.razaPata = 0.55f + 0.25f * Random01();
        p.alfaPata = 0.18f + 0.08f * Random01();

        for (int k = 0; k < 7; k++) {
            ParamFrunza& f = p.frunze[k];
            f.unghiDeg = (360.0f / 7.0f) * k + 12.0f * Random01();
            f.faza = 6.2831853f * Random01();
            f.amp = 4.0f + 3.0f * Random01();
            f.pitchSus = 18.0f + 10.0f * Random01();
            f.L = 1.2f + 0.6f * Random01();
            f.W = 0.25f + 0.12f * Random01();
            f.cazatura = 0.55f + 0.25f * Random01();
        }
    }
}

// ============================================================
// Init / Display
// ============================================================
void init()
{
    srand(1234567);
    GenereazaPalmieri();

    glClearColor(culoareCeata[0], culoareCeata[1], culoareCeata[2], 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glDisable(GL_COLOR_MATERIAL);
    glColor4f(1, 1, 1, 1);

    AplicaCeata();
}

void display()
{
    timpSec = (float)glutGet(GLUT_ELAPSED_TIME) * 0.001f;

    glClear(GL_COLOR_BUFFER_BIT);
    DeseneazaCerGradient();
    glClear(GL_DEPTH_BUFFER_BIT);

    camX = tintaX + distCamera * std::cos(alfaCamera) * std::cos(betaCamera);
    camY = tintaY + distCamera * std::cos(alfaCamera) * std::sin(betaCamera);
    camZ = tintaZ + distCamera * std::sin(alfaCamera);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, tintaX, tintaY, tintaZ, 0, 0, 1);

    glDisable(GL_COLOR_MATERIAL);
    glColor4f(1, 1, 1, 1);

    AplicaIluminareFaraPozitie();
    glLightfv(GL_LIGHT0, GL_POSITION, pozSoare);

    AplicaCeata();
    glEnable(GL_FOG);
    DeseneazaOcean();

    glDisable(GL_FOG);
    DeseneazaInsulaCuloriSafe();

    glEnable(GL_LIGHTING);

    DeseneazaRuineVarf();
    DeseneazaSpumaTarm();
    DeseneazaBarcaPescuit();
    DeseneazaCabanaPescuit();

    DeseneazaContactPalmieri();
    DeseneazaPalmieri();

    DeseneazaSoare();

    // Text in stanga sus (peste scena)
    DeseneazaOverlayText();

    glutSwapBuffers();
    glFlush();
}

// ============================================================
// Input
// ============================================================
void processSpecialKeys(int key, int, int)
{
    if (key == GLUT_KEY_LEFT)  betaCamera -= 0.05f;
    if (key == GLUT_KEY_RIGHT) betaCamera += 0.05f;
    if (key == GLUT_KEY_UP)    alfaCamera = Limiteaza(alfaCamera + 0.05f, -1.55f, 1.55f);
    if (key == GLUT_KEY_DOWN)  alfaCamera = Limiteaza(alfaCamera - 0.05f, -1.55f, 1.55f);
    glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int, int)
{
    if (key == '+' || key == '=') distCamera = Limiteaza(distCamera - 1.0f, 6.0f, 120.0f);
    if (key == '-')               distCamera = Limiteaza(distCamera + 1.0f, 6.0f, 120.0f);
    if (key == 27) exit(0);
    glutPostRedisplay();
}

// ============================================================
// Main
// ============================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(900, 650);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("Scena 3D dinamica: Insula tropicala");

    init();

    glutReshapeFunc(ReshapeSiProiectie);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

    glutMainLoop();
    return 0;
}
