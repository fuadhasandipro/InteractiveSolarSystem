#include "planets.h"
#include <cstring>

Planet planets[] = {
    {"Mercury",  18.0f,  1.2f,  47.4f,   10.0f,    0.8f,0.8f,0.7f, 0,0, false,0,0,0,0, 0,0, 0,0},
    {"Venus",    26.0f,  1.8f,  35.0f,  -6.0f,    1.0f,0.9f,0.6f, 0,0, false,0,0,0,0, 0,0, 0,0},
    {"Earth",    36.0f,  2.0f,  29.8f,  50.0f,    0.3f,0.6f,1.0f, 0,0, true,  4.5f,0.6f, 100.0f, 200.0f, 0.8f,0.8f,0.9f, 0,0},
    {"Mars",     48.0f,  1.5f,  24.1f,  45.0f,    1.0f,0.4f,0.3f, 0,0, false,0,0,0,0, 0,0, 0,0},
    {"Jupiter",  70.0f,  6.0f,  13.1f,  90.0f,    0.9f,0.8f,0.6f, 0,0, false,0,0,0,0, 0,0, 0,0},
    {"Saturn",   95.0f,  5.2f,   9.7f,  80.0f,    0.9f,0.8f,0.5f, 0,0, false,0,0,0,0, 0,0, 0,0},
    {"Uranus",  120.0f,  3.8f,   6.8f,  70.0f,    0.7f,0.9f,0.9f, 0,0, false,0,0,0,0, 0,0, 0,0},
    {"Neptune", 150.0f,  3.6f,   5.4f,  65.0f,    0.5f,0.7f,1.0f, 0,0, false,0,0,0,0, 0,0, 0,0}
};
const int NUM_PLANETS = sizeof(planets)/sizeof(planets[0]);

float sunRadius = 10.0f;
float sunSelfRotate = 8.0f;
float sunSelfAngle  = 0.0f;

void initPlanets()
{
    for (int i=0;i<NUM_PLANETS;i++){
        planets[i].orbitAngle = 0.0f;
        planets[i].selfAngle  = 0.0f;
        planets[i].moonOrbitAngle = 0.0f;
        planets[i].moonSelfAngle  = 0.0f;
    }
}

void drawCircle(float radius, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f);
        for(int i = 0; i <= segments; i++) {
            float angle = i * (2.0f * 3.14159265f / segments);
            glVertex2f(cosf(angle) * radius, sinf(angle) * radius);
        }
    glEnd();
}

void drawRing(float innerR, float outerR, int slices)
{
    glColor3f(0.8f,0.75f,0.6f);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i=0;i<=slices;i++){
        float a = (i/(float)slices)*2.0f*3.14159265f;
        float ca = cosf(a), sa = sinf(a);
        glVertex2f(innerR*ca, innerR*sa);
        glVertex2f(outerR*ca, outerR*sa);
    }
    glEnd();
}

void drawSun(float dt)
{
    sunSelfAngle += (paused?0.0f:(sunSelfRotate * dt * timeScale));
    glPushMatrix();
        glColor3f(1.0f, 0.9f, 0.2f);
        drawCircle(sunRadius, 36);
    glPopMatrix();
}

void drawPlanets(float dt)
{
    glDisable(GL_LIGHTING);

    for (int i=0;i<NUM_PLANETS;i++){
        Planet& p = planets[i];
        if (!paused) {
            p.orbitAngle += p.orbitSpeed * dt * timeScale;
            p.selfAngle  += p.selfRotateSpeed * dt * timeScale;
            if (p.hasMoon){
                p.moonOrbitAngle += p.moonOrbitSpeed * dt * timeScale;
                p.moonSelfAngle  += p.moonSelfRotate * dt * timeScale;
            }
        }

        if (showOrbits){
            glColor3f(0.5f,0.5f,0.5f);
            glBegin(GL_LINE_LOOP);
            const int SEG = 180;
            for (int s=0;s<SEG;s++){
                float a = (s/(float)SEG)*2.0f*3.14159265f;
                glVertex2f(p.orbitRadius*cosf(a), p.orbitRadius*sinf(a));
            }
            glEnd();
        }

        float px = p.orbitRadius * cosf(deg2rad(p.orbitAngle));
        float pz = p.orbitRadius * sinf(deg2rad(p.orbitAngle));

        glPushMatrix();
            glTranslatef(px, pz, 0);

            glPushMatrix();
                glColor3f(p.r, p.g, p.b);
                drawCircle(p.radius, 28);
            glPopMatrix();

            if (strcmp(p.name,"Saturn")==0){
                drawRing(p.radius*1.5f, p.radius*2.3f, 72);
            }

            if (p.hasMoon){
                if (showOrbits) {
                    glColor3f(0.6f,0.6f,0.6f);
                    glBegin(GL_LINE_LOOP);
                    for (int k=0;k<90;k++){
                        float a = (k/90.0f)*2.0f*3.14159265f;
                        glVertex2f(p.moonOrbitRadius*cosf(a), p.moonOrbitRadius*sinf(a));
                    }
                    glEnd();
                }

                float mx = p.moonOrbitRadius * cosf(deg2rad(p.moonOrbitAngle));
                float mz = p.moonOrbitRadius * sinf(deg2rad(p.moonOrbitAngle));

                glPushMatrix();
                    glTranslatef(mx, mz, 0);
                    glColor3f(p.moonR, p.moonG, p.moonB);
                    drawCircle(p.moonRadius, 20);
                glPopMatrix();
            }
        glPopMatrix();
    }
}
