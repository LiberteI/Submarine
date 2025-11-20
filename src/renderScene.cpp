#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include "render.h"
void myDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(0, 20, 200,
              0, 0, 0, 
              0, 1, 0);

    drawOriginDebugger();

    drawSubmarine();

    glutSwapBuffers();
}