#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include "../include/render.h"
#include "../include/input.h"
#include <cmath>

GLfloat cameraOffset = 100;
GLfloat horizontalAngle;
GLfloat verticalAngle;
/*  
    offset angle: [-1/4pi - 1/4pi]
*/
void sychronizeCameraWithSubmarine(){
    GLfloat interpolatedHorizontal = curMouseXPos / windowWidth - 0.5; // -0.5 - 0.5
    GLfloat interpolatedVertical = curMouseYPos / windowHeight - 0.5; // -0.5 - 0.5
    GLfloat pi = M_PI;
    
    horizontalAngle = interpolatedHorizontal * 1 / 2 * pi;
    verticalAngle = interpolatedVertical * 1 / 2 * pi;
}
void myUpdate(){
    tryMove();

    sychronizeCameraWithSubmarine();

    glutPostRedisplay();
}
void myDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    // glu look at : camera pos {x,y,z}, look at{x,y,z}, up pos{x,y,z}
    gluLookAt(submarinCurrentPos[0] + cos(horizontalAngle) * cameraOffset, submarinCurrentPos[1] + sin(verticalAngle) * cameraOffset, submarinCurrentPos[2] + sin(horizontalAngle) * cameraOffset,
              submarinCurrentPos[0], submarinCurrentPos[1], submarinCurrentPos[2], 
              0, 1, 0);

    drawOriginDebugger();

    drawSubmarine();

    glutSwapBuffers();
}