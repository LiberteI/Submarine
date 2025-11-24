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
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

GLuint sandTexture;

void registerInputFuncs(){
    // dump of input functions
    glutKeyboardFunc(myKeyboardDown);
    glutKeyboardUpFunc(myKeyboardUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutPassiveMotionFunc(myMousePassiveMotion);
}

void registerRenderer(){
    glutIdleFunc(myUpdate);
    glutDisplayFunc(myDisplay);
}

void initialiseProjection(){
    // change into projection mode so that we can change the camera properties
    glMatrixMode(GL_PROJECTION);
    // load the identity matrix into the projection matrix
    glLoadIdentity();
    // gluPerspective(fovy, aspect, near, far)
    gluPerspective(45, (float)windowWidth / (float)windowHeight, 0.1, 5000.0f);
    // change into model-view so that we can change the object positions
    glMatrixMode(GL_MODELVIEW);
    
}

void initialiseGeometryTools(){
    // create quadric
    quad = gluNewQuadric();
    // enable smooth normals for quad users. normals interpolated across surface
    gluQuadricNormals(quad, GLU_SMOOTH);
    // when you generate vertices, also generate UV (texture coordinates) coordinates. (normal xy coordinate system)
    gluQuadricTexture(quad, GL_TRUE);
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
    // allow glColor() to affect material diffuse color
    glEnable(GL_COLOR_MATERIAL);
    // use normal
    glEnable(GL_NORMALIZE);
    // use smooth shading
    glShadeModel(GL_SMOOTH);
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    // use texture
    glEnable(GL_TEXTURE_2D);
}

// load an img file from disk and create an GL texture obj
GLuint loadTexture(const char* filePath){
    int width, height, channels;

    // use stb to load file
    // stbi_load(const char *filename, int *x, int *y, int *comp, int req_comp)
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 4);

    if(!data){
        printf("Failed to load texture: %s\n", filePath);
        return 0;
    }
    printf("success\n");
    return 1;
}

void initialiseSceneResources(){
    // enable a specific light source as the sun(GL_LIGHT0)
    glEnable(GL_LIGHT0);

    launchLightings();
    
    loadSubmarineFile();

    sandTexture = loadTexture("assets/sand.jpg");


}

void initialiseCallbackRegistrations(){
    registerRenderer();

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

    initialiseGeometryTools();

    initialiseSceneResources();

    initialiseCallbackRegistrations();

    glutMainLoop();
    
    return 1;
}