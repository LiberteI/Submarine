#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
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
#include "include/global.h"

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
    glutCreateWindow("SUBMARINE_SIMULATOR");
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
    // enable alpha blending for transparency (e.g., ocean surface)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // use texture
    glEnable(GL_TEXTURE_2D);
}

void initialiseFog(){
    // enable fog
    glEnable(GL_FOG);
    // blue
    // ** note A is aborted
    GLfloat fogColor[] = {0.0f, 0.35f, 0.7f, 1};
    glFogfv(GL_FOG_COLOR, fogColor);
    
    // define fog mode as exponential
    glFogi(GL_FOG_MODE, GL_EXP2);

    // define fog density 
    glFogf(GL_FOG_DENSITY, 0.0005);
}

void initialiseSceneResources(){
    // enable a specific light source as the sun(GL_LIGHT0)
    glEnable(GL_LIGHT0);

    launchLightings();
    
    loadSubmarineFile();

    // flip coordinate
    stbi_set_flip_vertically_on_load(true);

    sandTexture = loadTexture("assets/sand.jpg");

    initialiseFog();

    uploadSurfaceToGPU();


    // compile glsl programs
    oceanShaderProgram = createProgram("glsl/ocean.vert", "glsl/ocean.frag");
    
    // make current with glUseProgram
    if(oceanShaderProgram != 0){
        glUseProgram(oceanShaderProgram);
    }
    else{
        printf("Ocean shader failed to compile/link, falling back to fixed pipeline rendering.\n");
    }
}

void initialiseCallbackRegistrations(){
    registerRenderer();

    registerInputFuncs();
}

void initialiseGLEW(){
    glewExperimental = GL_TRUE;
    glewInit();
}


// i try to let main keep the same level of abstraction and i think main is self-explainary
int main(int argc, char** argv){
    // intialise GLUT, playmode, window
    glutInit(&argc, argv);
    initialiseBuffer();
    initialiseWindow();

    initialiseGLEW();

    initialiseGlobalStates();
  
    initialiseProjection();

    initialiseGeometryTools();

    initialiseSceneResources();

    initialiseCallbackRegistrations();

    glutMainLoop();
    
    return 1;
}
