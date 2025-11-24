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
#include "include/lighting.h"
// #include "include/"

void registerInputFuncs(){
    // dump of input functions
    glutKeyboardFunc(myKeyboardDown);
    glutKeyboardUpFunc(myKeyboardUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutPassiveMotionFunc(myMousePassiveMotion);
}

void registerRender(){
    glutIdleFunc(myUpdate);
    glutDisplayFunc(myDisplay);
}

void initialiseProjection(){
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    // change into projection mode so that we can change the camera properties
    glMatrixMode(GL_PROJECTION);
    // load the identity matrix into the projection matrix
    glLoadIdentity();
    // gluPerspective(fovy, aspect, near, far)
    gluPerspective(45, (float)windowWidth / (float)windowHeight, 0.1, 5000.0f);
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

void initialiseGlobalStates(){
    // enable GL's lighting system
    glEnable(GL_LIGHTING);
    // enable a specific light source as the sun(GL_LIGHT0)
    glEnable(GL_LIGHT0);
    // allow glColor() to affect material diffuse color
    glEnable(GL_COLOR_MATERIAL);
    // use normal
    glEnable(GL_NORMALIZE);
    // use smooth shading
    glShadeModel(GL_SMOOTH);

 
}
void initialiseQuad(){
    // create quadric
    quad = gluNewQuadric();
    // enable smooth normals for quad users
    gluQuadricNormals(quad, GLU_SMOOTH);
}

void initialiseSceneResources(){
    launchLightings();

    initialiseQuad();
    
    loadSubmarineFile();
}

void initialiseCallbackRegistrations(){
    registerRender();

    registerInputFuncs();

}

// i try to let main keep the same level of abstraction and i think main is self-explainary
int main(int argc, char** argv){
    // intialise GLUT, playmode, window
    glutInit(&argc, argv);
    initialiseBuffer();
    initialiseWindow();

    initialiseGlobalStates();
  
    initialiseProjection();

    initialiseSceneResources();

    initialiseCallbackRegistrations();

    glutMainLoop();
    
    return 1;
}