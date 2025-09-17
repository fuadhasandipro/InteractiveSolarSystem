#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

void applyCamera();
void drawBitmapText(float x, float y, const char* s, void* font = GLUT_BITMAP_HELVETICA_18);
void drawHUD();

void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

#endif
