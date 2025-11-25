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

    // ***use stb to load file***
    // stbi_load(const char *filename, int *x, int *y, int *comp, int req_comp)
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 4);

    if(!data){
        printf("Failed to load texture: %s\n", filePath);
        return 0;
    }
    // printf("success\n");

    GLuint texture;

    // generate a texture obj on the GPU and store its handle in "texture"
    // this allocates a texture space in GPU memory but has not uploaded my texture img yet
    glGenTextures(1, &texture);

    // bind the texture so that all subsequent texture operations affect this texture obj
    // we gonna modify this texture obj by using this to select the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // ***TEXTURE CONFIGURATIONS***

    // to avoid distant distortion and noise
    // void glTexParameteri(GLenum target, GLenum pname, GLint param
    // how the texture is filtered when it is displayed smaller than ori resolution
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // GL_LINEAR: smooth interpolation between pixels
    // how it is filtered if bigger
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // infinite scrolling for UV coordinates
    // GL_REPEAT = tile the texture
    // S -> U axis(horizontal)
    // T -> V axis(vertical)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // ***UPLOAD CPU DATA TO GPU***
    /*
        glTexImage2D(
            GLenum target, 
            GLint level,
            GLint internalFormat,
            GLsizei width,
            GLsizei height,
            GLint border,
            GLenum format,
            GLenum type,
            const void * data
        );
    */
    glTexImage2D(
        GL_TEXTURE_2D,   // 2D texture to upload
        0,               // mipmap starts at level 0
        GL_RGBA,         // feed RGBA to GPU
        width,           // img width
        height,          // img height
        0,               // border = 0
        GL_RGBA,         // input pixel format RGBA
        GL_UNSIGNED_BYTE,// 1 byte per channel
        data             // pointer to pixel buffer in RAM
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    // free CPU side pixel buffer
    stbi_image_free(data);

    // return OpenGL texture ID
    return texture;
}

void initialiseSceneResources(){
    // enable a specific light source as the sun(GL_LIGHT0)
    glEnable(GL_LIGHT0);

    launchLightings();
    
    loadSubmarineFile();

    // flip coordinate
    stbi_set_flip_vertically_on_load(true);

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