#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <stdlib.h>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include "src/input.h"

GLUquadric* quad;
GLint sliceCount = 60;
GLint stackCount = 60;



/*
    define a original point with a white ball, x y z colored debug lines
*/
std::vector<std::array<GLfloat, 3>> submarineVertexList;
std::vector<std::array<GLfloat, 3>> submarineNormalList;



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
        if(identifer != "v" && identifer != "vn"){
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

}

// used for parsing tokens looking like "123//456". return an array{123, 456}
std::array<GLint, 2> parseToken(std::string token){
    std::array<GLint, 2> array;

    int vertexIndex, normalIndex;

    // find the first slash
    size_t slash1 = token.find('/');
    // find the second slash positioned right after slash1 (index based)
    size_t slash2 = token.find('/', slash1 + 1);

    // stoi: parse string to int
    // find the number before the first slash  index: [0 - slash1)
    vertexIndex = std::stoi(token.substr(0, slash1));

    // find the number after the second slash [slash2 + 1 - end)
    normalIndex = std::stoi(token.substr(slash2 + 1));

    array[0] = vertexIndex;
    array[1] = normalIndex;

    return array;
}

void drawSubmarine(){
    std::ifstream submarineObjFile("assets/submarine.obj");
    if(!submarineObjFile.is_open()){
        printf("cannot draw submarine because failure in opening submarine file");
        return;
    }

    std::string currentLine;

    glBegin(GL_TRIANGLES);
    while(std::getline(submarineObjFile, currentLine)){
        // we use an identifer to make operatios to the lines
        std::string identifer;
        std::stringstream tokenisedCurrentLine(currentLine);

        tokenisedCurrentLine >> identifer;

        // skip other lines except for f because we only need the instructions
        if(identifer != "f"){
            continue;
        }
        // token looks like : "123//456"
        std::string token1, token2, token3;
        tokenisedCurrentLine >> token1;
        tokenisedCurrentLine >> token2;
        tokenisedCurrentLine >> token3;

        std::array<GLint, 2> parsedToken1 = parseToken(token1);
        std::array<GLint, 2> parsedToken2 = parseToken(token2);
        std::array<GLint, 2> parsedToken3 = parseToken(token3);
        
        
        // glVertex3fv expects GLfloat*. use data() to parse array obj to it
        glNormal3fv(submarineNormalList[parsedToken1[1] - 1].data());
        glVertex3fv(submarineVertexList[parsedToken1[0] - 1].data());
        glVertex3fv(submarineVertexList[parsedToken2[0] - 1].data());
        glVertex3fv(submarineVertexList[parsedToken3[0] - 1].data());
        
    }
    glEnd();
}

void myDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(0, 20, 200,
              0, 0, 0, 
              0, 1, 0);

    drawOriginDebugger();

    drawSubmarine();

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
    recordNormalWindow();
    // create quadric
    quad = gluNewQuadric();
    loadSubmarineFile();
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
    glutMainLoop();
    return 1;
}