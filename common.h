#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN32
  #include <windows.h>
#endif

#include <GL/glut.h>
#include <cmath>

static const int INIT_WIN_W = 1280;
static const int INIT_WIN_H = 720;

static const int STAR_COUNT = 8000;
static const float FAR_PLANE  = 5000.0f;
static const float NEAR_PLANE = 0.1f;

inline float clampf(float v, float lo, float hi) { return (v<lo?lo:(v>hi?hi:v)); }
inline float deg2rad(float d){ return d * 3.1415926535f / 180.0f; }

struct Star { float x,y,z; float mag; };
extern Star stars[STAR_COUNT];
extern bool showStars;

struct Planet {
    const char* name;
    float orbitRadius;
    float radius;
    float orbitSpeed;
    float selfRotateSpeed;
    float r,g,b;

    float orbitAngle;
    float selfAngle;

    bool  hasMoon;
    float moonOrbitRadius;
    float moonRadius;
    float moonOrbitSpeed;
    float moonSelfRotate;
    float moonR, moonG, moonB;
    float moonOrbitAngle;
    float moonSelfAngle;
};
extern Planet planets[];
extern const int NUM_PLANETS;

extern float sunRadius;
extern float sunSelfRotate;
extern float sunSelfAngle;

extern float zoomFactor;
extern float viewCenterX;
extern float viewCenterY;
extern int lastMouseX;
extern int lastMouseY;
extern bool panningCam;
extern int focusIndex;

extern bool paused;
extern float timeScale;
extern double lastTimeMs;
extern bool showOrbits;
extern bool lightingOn;

#endif
