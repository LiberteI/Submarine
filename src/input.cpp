#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <cstdio>
#include <stdlib.h>
#include <array>
GLint uPressedTimes = 0;

GLint fPressedTimes = 0;

GLint windowWidth = 1000;
GLint windowHeight = 800;
GLint windowXPos = 0;
GLint windowYPos = 0;

GLfloat speed = 0.5;
std::array<GLfloat, 3> submarinCurrentPos = {0, 0, 0};

bool upHeld = false;
bool downHeld = false;

bool wHeld = false;
bool sHeld = false;
bool aHeld = false;
bool dHeld = false;

// lil helper to record normal window pos and size so that we can recover after
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

void tryMove(){
    if(wHeld){
        submarinCurrentPos[0] -= speed;
        // printf("wheld!: %f\n", speed);
    }
    if(sHeld){
        submarinCurrentPos[0] += speed;
        // printf("sheld!: %f\n", speed);
    }

    if(aHeld){
        submarinCurrentPos[2] += speed;
    }
    if(dHeld){
        submarinCurrentPos[2] -= speed;
    }
    
    if(upHeld){
        submarinCurrentPos[1] += speed;
    }
    if(downHeld){
        submarinCurrentPos[1] -= speed;
    }
}
// screen toggle / polygon mode toggle / quit button
void myKeyboardDown(unsigned char key, int, int){
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
    
    if(key == 'w'){
        // move forward
        wHeld = true;
    }
    if(key == 's'){
        // move backwards
        sHeld = true;
    }
    if(key == 'a'){
        // move left
        aHeld = true;
    }
    if(key == 'd'){
        // move right
        dHeld = true;
    }
}

void myKeyboardUp(unsigned char key, int, int){
    if(key == 'w'){
        // move forward
        wHeld = false;
    }
    if(key == 's'){
        // move backwards
        sHeld = false;
    }
    if(key == 'a'){
        // move left
        aHeld = false;
    }
    if(key == 'd'){
        // move right
        dHeld = false;
    }
}

void specialKeyDown(int key, int, int){
    if(key == GLUT_KEY_UP){
        // move up
        upHeld = true;
    }
    if(key == GLUT_KEY_DOWN){
        // move down
        downHeld = true;
    }
}

void specialKeyUp(int key, int, int){
    if(key == GLUT_KEY_UP){
        // move up
        upHeld = false;
    }
    if(key == GLUT_KEY_DOWN){
        // move down
        downHeld = false;
    }
}

void myMousePassiveMotion(int, int){
    // mouse control
}