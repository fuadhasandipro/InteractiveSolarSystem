#include "camera.h"
#include <cstdio>
#include <cstring>

const int FOCUS_MANUAL_PAN = -2;

float zoomFactor = 1.0f;
float viewCenterX = 0.0f;
float viewCenterY = 0.0f;

int   lastMouseX = 0;
int   lastMouseY = 0;
bool  panningCam = false;
int   focusIndex = -1;

// Shared globals
bool paused = false;
float timeScale = 1.0f;
double lastTimeMs = 0.0;
bool showOrbits = true;
bool lightingOn = false;

void applyCamera() {}

void drawBitmapText(float x, float y, const char* s, void* font)
{
    glRasterPos2f(x,y);
    for (const char* p = s; *p; ++p) glutBitmapCharacter(font, *p);
}

void drawHUD()
{
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0,0,0,0.4f);
    glBegin(GL_QUADS);
        glVertex2f(10, h-10); glVertex2f(400, h-10);
        glVertex2f(400, h-190); glVertex2f(10, h-190);
    glEnd();

    glColor3f(1,1,1);
    char buf[256];
    snprintf(buf, sizeof(buf), "2D Solar System | Speed: %.2fx | %s",
             timeScale, paused ? "Paused [SPACE]" : "Running [SPACE]");
    drawBitmapText(20, h-40, buf);

    drawBitmapText(20, h-70, "Mouse: Left-drag to Pan");
    drawBitmapText(20, h-95, "Arrows: Up/Down speed, Left/Right Pan");
    drawBitmapText(20, h-120,"Keys:  W/S Zoom, A/D/Q/E Pan");
    drawBitmapText(20, h-145,"       0=center, 1..8 focus planet, R=reset");
    drawBitmapText(20, h-170,"       O=toggle orbits, N=toggle stars");

    if (focusIndex == FOCUS_MANUAL_PAN) {
        drawBitmapText(20, h-195, "Focus: Manual Pan");
    } else if (focusIndex < 0) {
        drawBitmapText(20, h-195, "Focus: SUN / System center");
    } else {
        snprintf(buf, sizeof(buf), "Focus: %s", planets[focusIndex].name);
        drawBitmapText(20, h-195, buf);
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    float panSpeed = 5.0f * zoomFactor;
    switch (key) {
        case 27: exit(0); break;
        case ' ': paused = !paused; break;
        case 'r': case 'R':
            zoomFactor=1.0f; viewCenterX=0.0f; viewCenterY=0.0f;
            timeScale=1.0f; paused=false; focusIndex=-1;
            sunSelfAngle = 0.0f;
            for (int i=0;i<NUM_PLANETS;i++){
                planets[i].orbitAngle = planets[i].selfAngle = 0.0f;
                planets[i].moonOrbitAngle = planets[i].moonSelfAngle = 0.0f;
            }
            break;

        case 'o': case 'O': showOrbits = !showOrbits; break;
        case 'n': case 'N': showStars = !showStars; break;

        case 'w': case 'W': zoomFactor /= 1.1f; break;
        case 's': case 'S': zoomFactor *= 1.1f; break;

        case 'a': case 'A': viewCenterX -= panSpeed; focusIndex = FOCUS_MANUAL_PAN; break;
        case 'd': case 'D': viewCenterX += panSpeed; focusIndex = FOCUS_MANUAL_PAN; break;
        case 'q': case 'Q': viewCenterY += panSpeed; focusIndex = FOCUS_MANUAL_PAN; break;
        case 'e': case 'E': viewCenterY -= panSpeed; focusIndex = FOCUS_MANUAL_PAN; break;

        case '0': focusIndex = -1; break;
        case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8':
            focusIndex = (key - '1');
            if (focusIndex >= NUM_PLANETS) focusIndex = NUM_PLANETS-1;
            break;
    }
    if (zoomFactor < 0.05f) zoomFactor = 0.05f;
    if (zoomFactor > 10.0f) zoomFactor = 10.0f;
}

void specialKeys(int key, int x, int y)
{
    float panSpeed = 5.0f * zoomFactor;
    switch (key) {
        case GLUT_KEY_UP:   timeScale *= 1.1f; if (timeScale > 1000.f) timeScale=1000.f; break;
        case GLUT_KEY_DOWN: timeScale /= 1.1f; if (timeScale < 0.01f)  timeScale=0.01f;  break;
        case GLUT_KEY_LEFT: viewCenterX -= panSpeed; focusIndex = FOCUS_MANUAL_PAN; break;
        case GLUT_KEY_RIGHT:viewCenterX += panSpeed; focusIndex = FOCUS_MANUAL_PAN; break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        panningCam = (state == GLUT_DOWN);
        lastMouseX = x; lastMouseY = y;
    }
    if (button == 3 && state == GLUT_DOWN) { zoomFactor /= 1.1f; }
    if (button == 4 && state == GLUT_DOWN) { zoomFactor *= 1.1f; }

    if (zoomFactor < 0.05f) zoomFactor = 0.05f;
    if (zoomFactor > 10.0f) zoomFactor = 10.0f;
}

void motion(int x, int y)
{
    if (panningCam) {
        focusIndex = FOCUS_MANUAL_PAN;
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        float dx = (float)(x - lastMouseX) * (300.0f/w) * zoomFactor;
        float dy = (float)(y - lastMouseY) * (300.0f/h) * zoomFactor;
        viewCenterX -= dx;
        viewCenterY += dy;
        lastMouseX = x; lastMouseY = y;
    }
}
