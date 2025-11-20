#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <stdlib.h>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include "include/input.h"
#include "include/render.h"
#include "include/renderScene.h"
// #include "include/"

void registerInputFuncs(){
    // dump of input functions
    glutKeyboardFunc(myKeyboardDown);
    glutKeyboardUpFunc(myKeyboardUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutPassiveMotionFunc(myMousePassiveMotion);
}

void registerDisplay(){
    glutDisplayFunc(myDisplay);
}

void initialiseCamera(){
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
    
}

void initialiseWindow(){
    // initialise window
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("SUBMARINE");
    recordNormalWindow();
}

void initialiseBuffer(){
    // double buffer + color + deep buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
}

int main(int argc, char** argv){
    // intialise GLUT
    glutInit(&argc, argv);

    initialiseBuffer();

    initialiseWindow();

    // create quadric
    quad = gluNewQuadric();

    loadSubmarineFile();
    
    initialiseCamera();

    registerDisplay();

    registerInputFuncs();

    glutMainLoop();
    return 1;
}