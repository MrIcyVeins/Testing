#define NOMINMAX
#include <windows.h>
#include <gl/freeglut.h>
#include <cmath>
#include <cstdlib>

// Camera orbit (alpha/beta + dist)
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;
float alpha = 0.35f, beta = 0.0f;
float dist = 30.0f;
float Obsx, Obsy, Obsz;

// Time (animatii: frunze, barca)
float gTime = 0.0f;

// Scene params
const float Z_OCEAN = 0.05f;
const float R_ISLAND_BASE = 18.0f;
const float R_SAND_BASE = 14.0f;

// Coast shelf (smooth transition to ocean)
const float COAST_WIDTH = 2.2f;
const float COAST_UNDER_Z = Z_OCEAN - 0.03f;

// Light = sun position
GLfloat lightPos[4] = { 42.0f, -34.0f, 30.0f, 1.0f };

// Fog
GLfloat fogColor[4] = { 0.70f, 0.92f, 0.98f, 1.0f };

// Helpers
static inline float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static inline float smoothstep(float edge0, float edge1, float x)
{
    float t = clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

static inline void setMatColor(float r, float g, float b, float a = 1.0f)
{
    GLfloat col[4] = { r, g, b, a };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
}

// Deterministic random [0,1)
static inline float rand01(unsigned int& s)
{
    s = 1664525u * s + 1013904223u;
    return (float)(s & 0x00FFFFFF) / (float)0x01000000;
}

// Projection
void reshapeAndProjection(int w, int h)
{
    if (h == 0) h = 1;
    float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(60.0f, ratio, 0.1f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
}

// Simple lighting (Light0)
void aplicaIluminareSimpla()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);

    GLfloat ambientL[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat diffuseL[4] = { 1.00f, 1.00f, 0.95f, 1.0f };
    GLfloat specularL[4] = { 1.00f, 1.00f, 1.00f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientL);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseL);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularL);

    GLfloat matSpec[4] = { 0.30f, 0.30f, 0.30f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 24.0f);
}

// Fog setup
void aplicaCeata()
{
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_START, 30.0f);
    glFogf(GL_FOG_END, 140.0f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
}

// Sky gradient (drawn in screen space)
void desenCerGradient()
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
    glColor3f(1, 1, 1);
}

// Irregular island outline
static inline float islandRadius(float theta)
{
    const float a1 = 1.6f, a2 = 0.9f, a3 = 0.5f;

    float r = R_ISLAND_BASE
        + a1 * std::sinf(3.0f * theta + 0.3f)
        + a2 * std::sinf(7.0f * theta + 1.1f)
        + a3 * std::cosf(11.0f * theta - 0.7f);

    return clampf(r, R_ISLAND_BASE - 3.0f, R_ISLAND_BASE + 3.0f);
}

// Sand ring (inside shore)
static inline float sandRadius(float theta)
{
    const float b1 = 1.0f, b2 = 0.6f;

    float r = R_SAND_BASE
        + b1 * std::sinf(3.0f * theta + 0.8f)
        + b2 * std::cosf(6.0f * theta - 0.4f);

    float maxAllowed = islandRadius(theta) - 2.0f;
    return clampf(r, R_SAND_BASE - 2.5f, maxAllowed);
}

static inline void polar(float x, float y, float& r, float& theta)
{
    r = std::sqrt(x * x + y * y);
    theta = std::atan2(y, x);
}

// Height field for island (hill + beach + coast shelf)
float islandHeight(float x, float y)
{
    float r, theta;
    polar(x, y, r, theta);

    float shore = islandRadius(theta);
    float Sloc = sandRadius(theta);

    float rn = r / shore;
    float h = 6.0f * std::exp(-(rn * rn) * 2.2f);

    h += 0.18f * std::sinf(0.35f * x + 0.2f) * std::cosf(0.30f * y - 0.1f);

    const float beachH = 0.20f;

    float tBeach = smoothstep(Sloc - 2.8f, Sloc, r);
    h = h * (1.0f - tBeach) + beachH * tBeach;

    float tCoast = smoothstep(shore - COAST_WIDTH, shore, r);
    h = h * (1.0f - tCoast) + COAST_UNDER_Z * tCoast;

    return h;
}

// Approx normal from height field (finite differences)
void islandNormal(float x, float y, float& nx, float& ny, float& nz)
{
    const float e = 0.2f;
    float hL = islandHeight(x - e, y);
    float hR = islandHeight(x + e, y);
    float hD = islandHeight(x, y - e);
    float hU = islandHeight(x, y + e);

    nx = -(hR - hL);
    ny = -(hU - hD);
    nz = 2.0f * e;

    float len = std::sqrt(nx * nx + ny * ny + nz * nz);
    if (len < 1e-6f) { nx = 0; ny = 0; nz = 1; return; }
    nx /= len; ny /= len; nz /= len;
}

// Island mesh (triangle strips, clipped by outline)
void desenInsula()
{
    glDisable(GL_CULL_FACE);

    const int N = 95;
    const float S = 22.0f;
    const float step = (2.0f * S) / N;

    const float grassR = 0.18f, grassG = 0.62f, grassB = 0.24f;
    const float sandR = 0.92f, sandG = 0.86f, sandB = 0.55f;
    const float wetR = 0.84f, wetG = 0.78f, wetB = 0.54f;

    for (int j = 0; j < N; j++) {
        float y0 = -S + j * step;
        float y1 = y0 + step;

        bool stripOpen = false;

        for (int i = 0; i <= N; i++) {
            float x = -S + i * step;

            float r0, th0; polar(x, y0, r0, th0);
            float r1, th1; polar(x, y1, r1, th1);

            float shore0 = islandRadius(th0);
            float shore1 = islandRadius(th1);

            bool inside = (r0 <= shore0) && (r1 <= shore1);

            if (inside) {
                if (!stripOpen) { glBegin(GL_TRIANGLE_STRIP); stripOpen = true; }

                float h0 = islandHeight(x, y0);
                float h1 = islandHeight(x, y1);

                float nx, ny, nz;

                islandNormal(x, y0, nx, ny, nz);
                glNormal3f(nx, ny, nz);

                float Sloc0 = sandRadius(th0);
                float tSand0 = smoothstep(Sloc0 - 2.8f, Sloc0 + 0.2f, r0);
                float tWet0 = smoothstep(shore0 - COAST_WIDTH, shore0, r0);

                float baseR0 = grassR * (1.0f - tSand0) + sandR * tSand0;
                float baseG0 = grassG * (1.0f - tSand0) + sandG * tSand0;
                float baseB0 = grassB * (1.0f - tSand0) + sandB * tSand0;

                float outR0 = baseR0 * (1.0f - tWet0) + wetR * tWet0;
                float outG0 = baseG0 * (1.0f - tWet0) + wetG * tWet0;
                float outB0 = baseB0 * (1.0f - tWet0) + wetB * tWet0;

                setMatColor(outR0, outG0, outB0);
                glVertex3f(x, y0, h0);

                islandNormal(x, y1, nx, ny, nz);
                glNormal3f(nx, ny, nz);

                float Sloc1 = sandRadius(th1);
                float tSand1 = smoothstep(Sloc1 - 2.8f, Sloc1 + 0.2f, r1);
                float tWet1 = smoothstep(shore1 - COAST_WIDTH, shore1, r1);

                float baseR1 = grassR * (1.0f - tSand1) + sandR * tSand1;
                float baseG1 = grassG * (1.0f - tSand1) + sandG * tSand1;
                float baseB1 = grassB * (1.0f - tSand1) + sandB * tSand1;

                float outR1 = baseR1 * (1.0f - tWet1) + wetR * tWet1;
                float outG1 = baseG1 * (1.0f - tWet1) + wetG * tWet1;
                float outB1 = baseB1 * (1.0f - tWet1) + wetB * tWet1;

                setMatColor(outR1, outG1, outB1);
                glVertex3f(x, y1, h1);
            }
            else if (stripOpen) {
                glEnd();
                stripOpen = false;
            }
        }

        if (stripOpen) glEnd();
    }

    glEnable(GL_CULL_FACE);
}

// Ocean plane
void desenOceanSimplu()
{
    glDisable(GL_CULL_FACE);

    setMatColor(0.10f, 0.45f, 0.75f, 1.0f);
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

// Foam band on sand (alpha blending)
void desenSpumaTarm()
{
    const int K = 240;

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.90f, 0.98f, 1.0f, 0.22f);
    glNormal3f(0, 0, 1);

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= K; i++) {
        float a = (2.0f * 3.1415926f) * (i / (float)K);
        float ca = std::cos(a), sa = std::sin(a);

        float shore = islandRadius(a);
        float sand = sandRadius(a);

        float minR = sand + 0.25f;
        float maxR = shore - 0.22f;

        float desired = 1.10f;
        float maxBand = maxR - minR;
        float band = clampf(desired, 0.25f, maxBand * 0.90f);

        float noise = 0.18f * std::sinf(5.0f * a + 0.6f) + 0.10f * std::cosf(9.0f * a - 1.1f);
        float r0 = (shore - 0.95f) + noise;
        r0 = clampf(r0, minR, maxR - band);
        float r1 = r0 + band;

        float x0 = r0 * ca, y0 = r0 * sa;
        float x1 = r1 * ca, y1 = r1 * sa;

        float z0 = islandHeight(x0, y0) + 0.03f;
        float z1 = islandHeight(x1, y1) + 0.03f;

        glVertex3f(x0, y0, z0);
        glVertex3f(x1, y1, z1);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glColor3f(1, 1, 1);
}

// Sun sphere (visual only)
void desenSoare()
{
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.95f, 0.6f);

    glPushMatrix();
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    glutSolidSphere(1.4, 18, 18);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

// Palm tree (trunk + leaves with wind animation)
void desenPalmierMic(float x, float y, unsigned int seed)
{
    float z = islandHeight(x, y);

    unsigned int s = seed;
    float hTrunk = 1.4f + 0.6f * rand01(s);
    float rTrunk = 0.10f + 0.04f * rand01(s);
    float tiltX = -10.0f + 20.0f * rand01(s);
    float tiltY = -10.0f + 20.0f * rand01(s);
    float rotY = 360.0f * rand01(s);

    float windPhase = 6.2831853f * rand01(s);
    float windAmp = 6.0f + 6.0f * rand01(s);
    float windSpeed = 0.7f + 0.5f * rand01(s);

    float r, th; polar(x, y, r, th);
    float shore = islandRadius(th);
    float nearShore = smoothstep(shore - 2.0f, shore - 0.6f, r);
    float scale = 1.0f - 0.35f * nearShore;

    glPushMatrix();
    glTranslatef(x, y, z + 0.02f);
    glScalef(scale, scale, scale);

    glRotatef(rotY, 0, 0, 1);
    glRotatef(tiltX, 1, 0, 0);
    glRotatef(tiltY, 0, 1, 0);

    setMatColor(0.55f, 0.38f, 0.20f, 1.0f);
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, rTrunk * 1.05f, rTrunk * 0.75f, hTrunk, 10, 4);

    glTranslatef(0.0f, 0.0f, hTrunk);

    float w = std::sinf(gTime * windSpeed + windPhase);
    float w2 = std::sinf(gTime * (windSpeed * 0.63f) + windPhase * 1.7f);
    float wind = 0.65f * w + 0.35f * w2;
    float windDeg = windAmp * wind;

    setMatColor(0.10f, 0.55f, 0.18f, 1.0f);

    const int leaves = 7;
    for (int i = 0; i < leaves; i++) {
        float a = (360.0f / leaves) * i + 12.0f * rand01(s);

        float leafPhase = 6.2831853f * rand01(s);
        float leafAmp = 4.0f + 3.0f * rand01(s);
        float leafTwist = 2.0f * std::sinf(gTime * (windSpeed * 1.2f) + leafPhase);

        float pitchUp = 18.0f + 10.0f * rand01(s);
        float L = 1.2f + 0.6f * rand01(s);
        float W = 0.25f + 0.12f * rand01(s);
        float droop = 0.55f + 0.25f * rand01(s);

        glPushMatrix();
        glRotatef(a, 0, 0, 1);

        float leafWind = leafAmp * std::sinf(gTime * (windSpeed * 1.35f) + leafPhase);
        glRotatef(pitchUp + windDeg + leafWind, 1, 0, 0);
        glRotatef(leafTwist, 0, 0, 1);

        glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 0.3f, 0.95f);

        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-W, L * 0.55f, -droop * 0.55f);
        glVertex3f(0.0f, L, -droop);

        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(W, L * 0.55f, -droop * 0.55f);
        glVertex3f(0.0f, L, -droop);
        glEnd();

        glPopMatrix();
    }

    gluDeleteQuadric(q);
    glPopMatrix();
}

// Fake contact shadow (dark radial fan, alpha)
void desenPataContact(float x, float y, float radius, float alphaCenter)
{
    float z = islandHeight(x, y) + 0.04f;

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.0f, 0.0f, 0.0f, alphaCenter);
    glVertex3f(x, y, z);

    const int K = 26;
    for (int i = 0; i <= K; i++) {
        float a = (2.0f * 3.1415926f) * (i / (float)K);
        float px = x + radius * std::cos(a);
        float py = y + radius * std::sin(a);
        float pz = islandHeight(px, py) + 0.04f;

        glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
        glVertex3f(px, py, pz);
    }
    glEnd();
}

void desenContactPalmieri()
{
    const int COUNT = 10;
    unsigned int s = 1234567u;

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glNormal3f(0, 0, 1);

    for (int i = 0; i < COUNT; i++) {
        float ang = 2.0f * 3.1415926f * rand01(s);
        float ca = std::cos(ang), sa = std::sin(ang);

        float shore = islandRadius(ang);
        float sand = sandRadius(ang);

        float rMin = sand - 1.8f;
        float rMax = shore - 2.8f;
        if (rMax < rMin + 0.5f) { rMax = rMin + 0.5f; }

        float rr = rMin + (rMax - rMin) * rand01(s);
        float x = rr * ca;
        float y = rr * sa;

        unsigned int local = s + i * 777u;
        float radius = 0.55f + 0.25f * rand01(local);
        float aCenter = 0.18f + 0.08f * rand01(local);

        desenPataContact(x, y, radius * 1.15f, aCenter * 0.70f);
        desenPataContact(x, y, radius * 0.75f, aCenter);
    }

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glColor3f(1, 1, 1);
}

// Place palm trees on sand/grass band
void desenPalmieri()
{
    const int COUNT = 10;
    unsigned int s = 1234567u;

    for (int i = 0; i < COUNT; i++) {
        float ang = 2.0f * 3.1415926f * rand01(s);
        float ca = std::cos(ang), sa = std::sin(ang);

        float shore = islandRadius(ang);
        float sand = sandRadius(ang);

        float rMin = sand - 1.8f;
        float rMax = shore - 2.8f;
        if (rMax < rMin + 0.5f) { rMax = rMin + 0.5f; }

        float rr = rMin + (rMax - rMin) * rand01(s);
        float x = rr * ca;
        float y = rr * sa;

        desenPalmierMic(x, y, s + i * 100u);
    }
}

// Fishing boat near shore
void desenBarcaPescuit()
{
    float a = -0.65f;
    float shore = islandRadius(a);
    float r = shore + 4.0f;
    float x = r * std::cos(a);
    float y = r * std::sin(a);
    float z = Z_OCEAN + 0.02f;

    glPushMatrix();
    glTranslatef(x, y, z);

    float yawDeg = (a * 180.0f / 3.1415926f) + 90.0f;
    glRotatef(yawDeg, 0, 0, 1);

    float roll = 3.0f * std::sinf(gTime * 0.9f);
    glRotatef(roll, 1, 0, 0);

    setMatColor(0.35f, 0.18f, 0.08f, 1.0f);
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

    setMatColor(0.55f, 0.34f, 0.18f, 1.0f);
    glPushMatrix();
    glTranslatef(0, 0, 0.12f);
    glScalef(1.6f, 0.55f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    setMatColor(0.45f, 0.45f, 0.45f, 1.0f);
    GLUquadric* q = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.1f, 0.0f, 0.25f);
    gluCylinder(q, 0.04, 0.03, 0.9, 10, 2);
    glTranslatef(0, 0, 0.85f);

    glDisable(GL_LIGHTING);
    glColor3f(0.95f, 0.15f, 0.15f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0, 0, 0);
    glVertex3f(0.55f, 0, 0.10f);
    glVertex3f(0.55f, 0, -0.10f);
    glEnd();
    glEnable(GL_LIGHTING);

    glPopMatrix();
    gluDeleteQuadric(q);

    glPopMatrix();
}

// Stone ruins on island peak
void desenRuineVarf()
{
    float cx = 0.0f, cy = 0.0f;

    struct P { float x, y; };
    P pts[] = {
        {  0.0f,  0.0f },   // column 1
        {  0.9f, -0.2f },   // column 2
        { -0.6f,  0.7f },   // block 1
        {  0.2f,  1.1f },   // block 2
        { -1.0f, -0.6f },   // block 3
    };

    auto stone = [&](float k = 1.0f) {
        setMatColor(0.55f * k, 0.56f * k, 0.58f * k, 1.0f);
        };

    GLUquadric* q = gluNewQuadric();
    gluQuadricNormals(q, GLU_SMOOTH);

    // Column 1
    {
        float x = cx + pts[0].x, y = cy + pts[0].y;
        float z = islandHeight(x, y);

        glPushMatrix();
        glTranslatef(x, y, z - 0.02f);
        glRotatef(10.0f, 1, 0, 0);
        glRotatef(-20.0f, 0, 1, 0);

        stone(1.0f);
        gluCylinder(q, 0.28, 0.24, 2.2, 16, 5);

        stone(1.05f);
        glTranslatef(0, 0, 2.15f);
        glScalef(0.75f, 0.75f, 0.25f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Column 2
    {
        float x = cx + pts[1].x, y = cy + pts[1].y;
        float z = islandHeight(x, y);

        glPushMatrix();
        glTranslatef(x, y, z - 0.02f);
        glRotatef(-18.0f, 1, 0, 0);
        glRotatef(25.0f, 0, 1, 0);

        stone(0.98f);
        gluCylinder(q, 0.24, 0.20, 1.55, 16, 4);

        stone(1.03f);
        glTranslatef(0, 0, 1.50f);
        glScalef(0.65f, 0.65f, 0.22f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Fallen beam
    {
        float x = cx + 0.45f, y = cy - 0.10f;
        float z = islandHeight(x, y);

        glPushMatrix();
        glTranslatef(x, y, z + 0.15f);
        glRotatef(35.0f, 0, 0, 1);
        glRotatef(12.0f, 1, 0, 0);

        stone(0.95f);
        glScalef(1.6f, 0.35f, 0.25f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Blocks
    auto bloc = [&](float x, float y, float sx, float sy, float sz, float rotZ)
        {
            float z = islandHeight(x, y);
            glPushMatrix();
            glTranslatef(x, y, z - 0.01f);
            glRotatef(rotZ, 0, 0, 1);
            stone(0.92f);
            glScalef(sx, sy, sz);
            glutSolidCube(1.0);
            glPopMatrix();
        };

    bloc(cx + pts[2].x, cy + pts[2].y, 0.65f, 0.45f, 0.35f, 22.0f);
    bloc(cx + pts[3].x, cy + pts[3].y, 0.50f, 0.60f, 0.25f, -15.0f);
    bloc(cx + pts[4].x, cy + pts[4].y, 0.80f, 0.40f, 0.30f, 40.0f);

    gluDeleteQuadric(q);
}

// Fishing hut (roof + ramp)
void desenCabanaPescuit()
{
    float x = 10.0f;
    float y = -6.5f;

    float r, th; polar(x, y, r, th);
    float shore = islandRadius(th);
    float sand = sandRadius(th);
    float maxR = shore - 0.9f;
    float minR = sand + 0.6f;

    if (r > maxR) { float k = maxR / r; x *= k; y *= k; }
    if (r < minR) { float k = minR / (r + 1e-6f); x *= k; y *= k; }

    float z = islandHeight(x, y);

    glPushMatrix();
    glTranslatef(x, y, z + 0.02f);
    glRotatef(35.0f, 0, 0, 1);

    const float floorZ = 1.00f;
    const float floorT = 0.15f;
    const float wallZ = 1.45f;
    const float wallH = 0.90f;
    const float wallTopZ = wallZ + 0.5f * wallH;
    const float roofZ = wallTopZ + 0.02f;

    setMatColor(0.45f, 0.30f, 0.18f, 1.0f);
    GLUquadric* q = gluNewQuadric();

    auto post = [&](float px, float py) {
        glPushMatrix();
        glTranslatef(px, py, -0.10f);
        gluCylinder(q, 0.10, 0.09, 1.10, 10, 3);
        glPopMatrix();
        };

    post(-1.0f, -0.7f);
    post(1.0f, -0.7f);
    post(-1.0f, 0.7f);
    post(1.0f, 0.7f);

    setMatColor(0.55f, 0.38f, 0.22f, 1.0f);
    glPushMatrix();
    glTranslatef(0, 0, floorZ);
    glScalef(2.4f, 1.8f, floorT);
    glutSolidCube(1.0);
    glPopMatrix();

    setMatColor(0.60f, 0.43f, 0.25f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, wallZ);
    glScalef(1.8f, 1.3f, wallH);
    glutSolidCube(1.0);
    glPopMatrix();

    GLboolean wasCull = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    setMatColor(0.35f, 0.22f, 0.14f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, roofZ);

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

    // Ramp down to ground
    setMatColor(0.52f, 0.36f, 0.20f, 1.0f);

    const float rampLen = 3.0f;
    const float rampWide = 0.65f;
    const float rampThick = 0.10f;
    const float pitchDeg = 28.0f;

    const float hingeX = 1.25f;
    const float hingeY = -0.15f;
    const float hingeZ = floorZ + 0.5f * floorT;

    glPushMatrix();
    glTranslatef(hingeX, hingeY, hingeZ);
    glRotatef(pitchDeg, 0, 1, 0);
    glTranslatef(rampLen * 0.5f, 0.0f, -rampThick * 0.5f);
    glScalef(rampLen, rampWide, rampThick);
    glutSolidCube(1.0);
    glPopMatrix();

    setMatColor(0.45f, 0.30f, 0.18f, 1.0f);
    glPushMatrix();
    glTranslatef(hingeX + 2.7f, hingeY - 0.25f, 0.25f);
    gluCylinder(q, 0.06, 0.05, 0.55, 10, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(hingeX + 2.7f, hingeY + 0.25f, 0.25f);
    gluCylinder(q, 0.06, 0.05, 0.55, 10, 2);
    glPopMatrix();

    gluDeleteQuadric(q);
    glPopMatrix();
}

// Init
void init()
{
    glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    aplicaCeata();
}

// Render frame
void display()
{
    gTime = (float)glutGet(GLUT_ELAPSED_TIME) * 0.001f;

    // Draw sky first (no depth)
    glClear(GL_COLOR_BUFFER_BIT);
    desenCerGradient();
    glClear(GL_DEPTH_BUFFER_BIT);

    // Orbit camera
    Obsx = Refx + dist * std::cos(alpha) * std::cos(beta);
    Obsy = Refy + dist * std::cos(alpha) * std::sin(beta);
    Obsz = Refz + dist * std::sin(alpha);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(Obsx, Obsy, Obsz, Refx, Refy, Refz, 0, 0, 1);

    aplicaIluminareSimpla();
    aplicaCeata();

    // Draw order
    desenOceanSimplu();
    desenInsula();
    desenRuineVarf();
    desenSpumaTarm();
    desenBarcaPescuit();
    desenCabanaPescuit();
    desenContactPalmieri();
    desenPalmieri();
    desenSoare();

    glutSwapBuffers();
    glFlush();
}

// Input: camera orbit
void processSpecialKeys(int key, int, int)
{
    if (key == GLUT_KEY_LEFT)  beta -= 0.05f;
    if (key == GLUT_KEY_RIGHT) beta += 0.05f;
    if (key == GLUT_KEY_UP)    alpha = clampf(alpha + 0.05f, -1.55f, 1.55f);
    if (key == GLUT_KEY_DOWN)  alpha = clampf(alpha - 0.05f, -1.55f, 1.55f);

    glutPostRedisplay();
}

// Input: zoom + exit
void processNormalKeys(unsigned char key, int, int)
{
    if (key == '+' || key == '=') dist = clampf(dist - 1.0f, 6.0f, 120.0f);
    if (key == '-')               dist = clampf(dist + 1.0f, 6.0f, 120.0f);
    if (key == 27) exit(0);

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(900, 650);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("Scena 3D dinamica: Insula tropicala");

    init();

    glutReshapeFunc(reshapeAndProjection);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

    glutMainLoop();
    return 0;
}
