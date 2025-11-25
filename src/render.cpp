#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include "../include/input.h"


GLUquadric* quad;

GLint sliceCount = 60;
GLint stackCount = 60;

GLint oceanBottomRadius = 1500;
GLdouble oceanBottomPos = -500;

GLdouble oceanTop = 200;
GLdouble oceanDepth = 800;
/*
    vector vs array:
    vector: java arraylist
    array : java array
*/
// looks like: { {1, 2, 3}, {4, 5, 6}, {x, y, z} }
std::vector<std::array<GLfloat, 3>> submarineVertexList;
std::vector<std::array<GLfloat, 3>> submarineNormalList;

// an arraylist stroing an array of pairs
// looks like : { { {1,999}, {3, 999}, {5, 999} } }
std::vector<std::array<std::array<GLint, 2>, 3>> submarineFaceListWithNormal;

void drawOriginDebugger() {

    // draw sphere with lighting
    GLfloat white[] = {1,1,1,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    gluSphere(quad, 0.5, sliceCount, stackCount);

    // draw axes without lighting
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glLineWidth(5.0);

    // X axis
    glColor3f(1,0,0); 
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
    glEnd();
    // Y axis
    glColor3f(0,1,0); 
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
    glEnd();
    // Z axis
    glColor3f(0,0,1); 
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
    glEnd();
    // restore
    glEnable(GL_LIGHTING); 
}


// load file -> read line by line -> ignore lines that do not start with g/v/vn/f -> store vertices -> store normals -> follow instr
// note : just figured out the indices are cumulative.
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
        if(identifer != "v" && identifer != "vn" && identifer != "f"){
            continue;
        }

        if(identifer == "f"){
            // token looks like : "123//456"
            std::string token1, token2, token3;
            curTokenizedLine >> token1;
            curTokenizedLine >> token2;
            curTokenizedLine >> token3;

            std::array<GLint, 2> parsedToken1 = parseToken(token1);
            std::array<GLint, 2> parsedToken2 = parseToken(token2);
            std::array<GLint, 2> parsedToken3 = parseToken(token3);

            submarineFaceListWithNormal.push_back({parsedToken1, parsedToken2, parsedToken3});
            continue;
        }
        // add x,y,z to list accordingly
        GLfloat x, y, z;
        curTokenizedLine >> x;
        curTokenizedLine >> y;
        curTokenizedLine >> z;
        if(identifer == "v"){
            submarineVertexList.push_back({x, y, z});
        }
        if(identifer == "vn"){
            submarineNormalList.push_back({x, y, z});
        }
    }

}

void initialiseSubmarineMaterial(){
    // ambient gray (low brightness)
    GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};

    // diffuse gray (main visible color)
    GLfloat diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};

    // specular highlight (steel hull has some shine)
    GLfloat specular[] = {0.4f, 0.4f, 0.4f, 1.0f};

    GLfloat emission[] = {0.1f, 0.1f, 0.1f, 1.0f};

    // shininess controls highlight size (higher = sharper)
    GLfloat shininess = 32.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT,  ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

}

void drawSubmarine(){
    initialiseSubmarineMaterial();
    glPushMatrix();
    glTranslatef(submarinCurrentPos[0], submarinCurrentPos[1], submarinCurrentPos[2]);

    glBegin(GL_TRIANGLES);
        /*
            looks like :{       
                            { {1,1}, {3, 1}, {5, 1} }, <-- face
                            { {1,2}, {3, 2}, {5, 2} },
                            { {v1,n}, {v2,n}, {v3,,n}}
                        }
        */ 
        for(auto &face : submarineFaceListWithNormal){
            int normalIdxInNormalList = face[0][1] - 1;

            int vertexIdx1InVertexList = face[0][0] - 1;

            int vertexIdx2InVertexList = face[1][0] - 1;

            int vertexIdx3InVertexList = face[2][0] - 1;
            
            GLfloat* vertex1 = submarineVertexList[vertexIdx1InVertexList].data();
            GLfloat* vertex2 = submarineVertexList[vertexIdx2InVertexList].data();
            GLfloat* vertex3 = submarineVertexList[vertexIdx3InVertexList].data();

            GLfloat* curNormal = submarineNormalList[normalIdxInNormalList].data();

            glNormal3fv(curNormal);
            glVertex3fv(vertex1);
            glVertex3fv(vertex2);
            glVertex3fv(vertex3);
        }
    glEnd();
    glPopMatrix();
}

/*  
    disk should lies on y = -100
*/

// add highly emissive material
void applySandEmission(){
    GLfloat ambient[] = {0.30f, 0.25f, 0.15f, 1.0f};  // warm sandy brown
    GLfloat diffuse[] = {0.70f, 0.65f, 0.45f, 1.0f};  // visible sand color
    GLfloat specular[] = {0.10f, 0.10f, 0.10f, 1.0f};  // sand is matte, not shiny
    GLfloat emission[] = {0.1f, 0.08f, 0.04f, 1.0f};  // subtle golden self-light
    GLfloat shininess = 8.0f;                         // wide dull highlights
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

}

void drawDisk(){
    applySandEmission();
    // *** The last transform you call affects the object first. ***
    glPushMatrix();
    glTranslatef(0, oceanBottomPos, 0);
    glRotatef(-90, 1, 0, 0);
    
    // inner : 0 -> filled
    // loop : 1 -> ring
    /*
        gluDisk(GLUquadric* quad,
            GLdouble innerRadius,
            GLdouble outerRadius,
            GLint slices,
            GLint loops);
    */
    gluDisk(quad, 0, oceanBottomRadius, sliceCount, 1);
    glPopMatrix();
}

void drawCylinder(){
    applySandEmission();
    glPushMatrix();
    glTranslatef(0, oceanBottomPos - 50, 0);
    glRotatef(-90, 1, 0, 0);
    /*
        gluCylinder(GLUquadric *quad,
            GLdouble baseRadius,
            GLdouble topRadius,
            GLdouble height,
            GLint slices,
            GLint stacks);
    */
    gluCylinder(quad, oceanBottomRadius -20, 
                oceanBottomRadius - 20, 
                oceanDepth, 
                sliceCount, 
                stackCount
    );

    glPopMatrix();
}