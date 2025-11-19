#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <cstdio>
#include <stdlib.h>
GLint uPressedTimes = 0;

GLint fPressedTimes = 0;

GLint windowWidth = 1000;
GLint windowHeight = 800;
GLint windowXPos = 0;
GLint windowYPos = 0;

void recordNormalWindow(){
    windowXPos = glutGet(GLUT_WINDOW_X);
    windowYPos = glutGet(GLUT_WINDOW_Y);
}

void trySwitchPolygonMode(){
    if(uPressedTimes % 2 == 0){
        // void glPolygonMode(GLenum face, GLenum mode) // fill/line/point
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        printf("solid rendering\n");
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        printf("wireframe rendering\n");
    }
}

void tryResizeWindow(){
    if(fPressedTimes % 2 == 0){
        // normal
        glutReshapeWindow(windowWidth, windowHeight);
        glutPositionWindow(windowXPos, windowYPos);
    }
    else{
        // full screen
        glutFullScreen();
    }
}

// screen toggle / polygon mode toggle / quit button
void myKeyboardDown(unsigned char key, int x, int y){
    if(key == 'q'){
        // quit app
        exit(0);
    }
    if(key == 'u'){
        uPressedTimes ++;
        trySwitchPolygonMode();
    }
    if(key == 'f'){
        fPressedTimes ++;
        tryResizeWindow();
    }
    
}

void myKeyboardUp(unsigned char key, int x, int y){
    
}
