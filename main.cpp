#include "render.h"
#include "camera.h"
#include <GL/glut.h>

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(INIT_WIN_W, INIT_WIN_H);
    glutCreateWindow("OpenGL/GLUT - Interactive Solar System (Group Project)");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    lastTimeMs = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
    return 0;
}
