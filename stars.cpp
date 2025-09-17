#include "stars.h"
#include <cstdlib>

Star stars[STAR_COUNT];
bool showStars = true;

void initStars(unsigned seed)
{
    srand(seed);
    const float SPREAD = FAR_PLANE * 0.5f;
    for (int i=0;i<STAR_COUNT;i++){
        float x = SPREAD * (2.0f * ((float)rand()/RAND_MAX) - 1.0f);
        float z = SPREAD * (2.0f * ((float)rand()/RAND_MAX) - 1.0f);
        stars[i] = {x, 0.0f, z, 1.0f + 1.5f*((float)rand()/RAND_MAX)};
    }
}

void drawStars()
{
    if (!showStars) return;
    glDisable(GL_LIGHTING);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    for (int i=0;i<STAR_COUNT;i++){
        glVertex2f(stars[i].x, stars[i].z);
    }
    glEnd();
}
