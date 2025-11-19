#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <stdlib.h>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include <string>

GLint windowWidth = 1000;
GLint windowHeight = 800;

GLUquadric* quad;
GLint sliceCount = 60;
GLint stackCount = 60;

GLint uPressedTimes = 0;

GLint fPressedTimes = 0;

GLint windowXPos = 0;
GLint windowYPos = 0;
/*
    define a original point with a white ball, x y z colored debug lines
*/
std::vector<std::array<GLfloat, 3>> submarineVertexList;
std::vector<std::array<GLfloat, 3>> submarineNormalList;

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

void recordNormalWindow(){
    windowXPos = glutGet(GLUT_WINDOW_X);
    windowXPos = glutGet(GLUT_WINDOW_Y);
}

// load file -> read line by line -> ignore lines that do not start with g/v/vn/f -> store vertices -> store normals -> follow instr
// note : just figured out the indices are cumulative.
void loadSubmarineFile(){
    // load submarine file
    std::ifstream file("assets/submarine.obj");

    // guard file
    if(!file.is_open()){
        printf("fail to open submarine file");
        return;
    }

    std::string currentLine;
    // read the next line and store it in "currentLine"
    // currentLine: "v x y z"
    while(std::getline(file, currentLine)){
        // get current string stream version of currentLine, so that we can use >> effectively
        std::stringstream curTokenizedLine(currentLine);
        std::string identifer;

        curTokenizedLine >> identifer;
        
        // skip other lines
        if(identifer != "v" || identifer != "vn"){
            continue;
        }
        GLfloat x, y, z;
        curTokenizedLine >> x;
        curTokenizedLine >> y;
        curTokenizedLine >> z;
        // add x,y,z to list accordingly
        if(identifer == "v"){
            submarineVertexList.push_back({x, y, z});
        }
        if(identifer == "vn"){
            submarineNormalList.push_back({x, y, z});
        }
    }

    printf("finished loading!!!");
}

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

    loadSubmarineFile();
    glutMainLoop();
    return 1;
}