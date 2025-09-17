#include "render.h"
#include "stars.h"
#include "planets.h"
#include "camera.h"
#include <cstdio>

const int FOCUS_MANUAL_PAN = -2;

void setupLights()
{
    glDisable(GL_LIGHTING);
}

void initGL()
{
    glClearColor(0.02f,0.02f,0.05f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    initStars();
    initPlanets();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    double now = glutGet(GLUT_ELAPSED_TIME);
    float dt = (float)((now - lastTimeMs) / 1000.0);
    if (lastTimeMs == 0.0) dt = 0.016f;
    lastTimeMs = now;

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    float aspect = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (focusIndex >= 0 && focusIndex < NUM_PLANETS) {
        Planet& p = planets[focusIndex];
        float a = deg2rad(p.orbitAngle);
        viewCenterX = p.orbitRadius * cosf(a);
        viewCenterY = p.orbitRadius * sinf(a);
    } else if (focusIndex == -1) {
        viewCenterX = 0;
        viewCenterY = 0;
    }


    float viewHeight = 180.0f * zoomFactor;
    float viewWidth = viewHeight * aspect;

    gluOrtho2D(viewCenterX - viewWidth / 2.0f,
               viewCenterX + viewWidth / 2.0f,
               viewCenterY - viewHeight / 2.0f,
               viewCenterY + viewHeight / 2.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawStars();
    drawSun(dt);
    drawPlanets(dt);
    drawHUD();

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0,0,w,h);
}
