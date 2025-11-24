#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include "../include/render.h"
#include "../include/input.h"
#include "../include/lighting.h"
#include <cmath>
#include <array>
#include <algorithm>

GLfloat cameraOffset = 500;
GLfloat horizontalAngle;
GLfloat verticalAngle;
std::array<GLfloat, 3> currentCamPos;;
/*  
    offset angle: [-1/4pi - 1/4pi]
*/
void computeOffsetAngles(){
    GLfloat interpolatedHorizontal = curMouseXPos / windowWidth - 0.5; // -0.5 - 0.5
    GLfloat interpolatedVertical = curMouseYPos / windowHeight - 0.5; // -0.5 - 0.5
    GLfloat pi = M_PI;
    
    horizontalAngle = interpolatedHorizontal * pi;
    verticalAngle = interpolatedVertical * pi;

    // clamp vertical angle so that camera will not teleport
    verticalAngle = std::clamp(verticalAngle, -1.2f, 1.2f);

}

// it is like rotating around a planet in a2
/*
    x = cx + r*cos(φ)*cos(θ)
    y = cy + r*sin(φ)
    z = cz + r*cos(φ)*sin(θ)
*/
void computeCurCamPos(){
    // x
    currentCamPos[0] = submarinCurrentPos[0] + cos(verticalAngle) * cos(horizontalAngle) * cameraOffset;
    // y
    currentCamPos[1] = submarinCurrentPos[1] + sin(verticalAngle) * cameraOffset;
    // z
    currentCamPos[2] = submarinCurrentPos[2] + cos(verticalAngle) * sin(horizontalAngle) * cameraOffset;
}

void myUpdate(){
    tryMove();

    computeOffsetAngles();

    computeCurCamPos();

    glutPostRedisplay();
}

void myDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    // glu look at : camera pos {x,y,z}, look at{x,y,z}, up pos{x,y,z}
    gluLookAt(currentCamPos[0], currentCamPos[1], currentCamPos[2],
              submarinCurrentPos[0], submarinCurrentPos[1], submarinCurrentPos[2], 
              0, 1, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, sunlightDir);

    drawOriginDebugger();

    drawSubmarine();

    drawDisk();

    drawCylinder();

    glutSwapBuffers();
}