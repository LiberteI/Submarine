#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include "../include/global.h"

// x, y, z(pos | direction(vector)) ,w (1 -> point light | 0 -> directional light)
GLfloat sunlightDir[] = {1.0f, 1.0f, 0.2f, 0.0f};

void sunlight(){
    // config r, g, b, a(ignored)
    // strong white sunlight
    GLfloat diffuseConfig[]  = {1.0f, 1.0f, 1.0f, 1.0f};

    // white highlights from the sun
    GLfloat specularConfig[] = {1.0f, 1.0f, 1.0f, 1.0f};

    // soft ambient bounce light(global sky illumination)
    GLfloat ambientConfig[]  = {0.2f, 0.2f, 0.2f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseConfig);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularConfig);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientConfig);
}

void launchLightings(){
    sunlight();
}