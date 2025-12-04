#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include "../include/render.h"
#include "../include/input.h"
#include "../include/lighting.h"
#include "../include/global.h"
#include "../include/coral.h"
#include <cmath>
#include <array>
#include <algorithm>
#include "../include/helper.h"
#include "../include/submarine.h"

GLfloat cameraOffset = 500;
GLfloat horizontalAngle = 0;
GLfloat verticalAngle = 0;
std::array<GLfloat, 3> currentCamPos = {0, 40, cameraOffset};

void computeOffsetAngles(){
    GLfloat interpolatedHorizontal = deltaMouseX * mouseSensitivity;
    GLfloat interpolatedVertical = deltaMouseY * mouseSensitivity;
    
    horizontalAngle += interpolatedHorizontal;
    verticalAngle += interpolatedVertical;

    // clamp vertical angle so that camera will not teleport
    verticalAngle = std::clamp(verticalAngle, -1.5f, 1.5f);
    
    // reset deltas so that camera will not spin
    deltaMouseX = 0;
    deltaMouseY = 0;
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

void setUpCamera(){
    // glu look at : camera pos {x,y,z}, look at{x,y,z}, up pos{x,y,z}
    gluLookAt(currentCamPos[0], currentCamPos[1], currentCamPos[2],
              submarinCurrentPos[0], submarinCurrentPos[1], submarinCurrentPos[2], 
              0, 1, 0);
}

void myDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    
    glLightfv(GL_LIGHT0, GL_POSITION, sunlightDir);

    setUpCamera();
    
    drawOriginDebugger();
    
    drawOceanSurface();
    // --- textured objs
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sandTexture);
    drawDisk();

    drawCylinder();

    drawCorals();

    drawFishes();
    // --- untextured obj
    glDisable(GL_TEXTURE_2D);
    drawSubmarine();
    
    glutSwapBuffers();
}
