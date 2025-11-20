#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <stdlib.h>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include "src/input.h"
#include "src/render.h"
#include "src/renderScene.h"
// #include "src/"



int main(int argc, char** argv){
    // intialise GLUT
    glutInit(&argc, argv);
    // double buffer + color + deep buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // initialise window
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("SUBMARINE");
    recordNormalWindow();
    // create quadric
    quad = gluNewQuadric();
    loadSubmarineFile();
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    // change into projection mode so that we can change the camera properties
    glMatrixMode(GL_PROJECTION);
    // load the identity matrix into the projection matrix
    glLoadIdentity();
    // gluPerspective(fovy, aspect, near, far)
    gluPerspective(45, (float)windowWidth / (float)windowHeight, 0.1, 2000.0f);
    // change into model-view so that we can change the object positions
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboardDown);
    glutKeyboardUpFunc(myKeyboardUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutMainLoop();
    return 1;
}