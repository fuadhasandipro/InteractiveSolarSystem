#ifndef PLANETS_H
#define PLANETS_H

#include "common.h"

void initPlanets();
void drawSun(float dt);
void drawPlanets(float dt);
void drawRing(float innerR, float outerR, int slices=64);

#endif
