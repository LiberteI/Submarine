#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <stdlib.h>

GLint windowWidth = 1000;
GLint windowHeight = 800;

GLUquadric* quad;
GLint sliceCount = 60;
GLint stackCount = 60;

/*
    define a original point with a white ball, x y z colored debug lines
*/
void myKeyboard(unsigned char key, int x, int y){
    if(key == 'q'){
        exit(0);
    }
    
}
void drawOriginDebugger(){
    // draw white sphere
    //void gluSphere(GLUquadric *quad, GLdouble radius, GLint slices, GLint stacks)
    glColor3f(1, 1, 1);
    gluSphere(quad, 0.5, sliceCount, stackCount);
    
    // x debugger
    glColor3f(1, 0, 0);
    glLineWidth(5.0);
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(20, 0 ,0);
    glEnd();

    // y debugger
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 20 ,0);
    glEnd();

    // z debugger
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0 ,20);
    glEnd();
}

void myDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(0, 20, 200,
              0, 0, 0, 
              0, 1, 0);
    drawOriginDebugger();

    glutSwapBuffers();
}
int main(int argc, char** argv){
    // intialise GLUT
    glutInit(&argc, argv);
    // double buffer + color + deep buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // initialise window
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("SUBMARINE");
    // create quadric
    quad = gluNewQuadric();
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
    glutKeyboardFunc(myKeyboard);
    glutMainLoop();
    return 1;
}