#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include "../include/input.h"
#include "../include/stb_image.h"
#include "../include/renderScene.h"
#include "../include/global.h"
#include "../include/submarine.h"
#include "../include/helper.h"

GLUquadric* quad;

GLint sliceCount = 60;
GLint stackCount = 60;

GLint oceanBottomRadius = 1500;
GLdouble oceanBottomPos = -500;

GLfloat oceanSurfaceUnitLength = 50;
// subdivide the surface into stripe - 1 pieces
GLint oceanSurfaceVertexCount = 101;
std::vector<std::array<GLfloat, 3>> oceanSurfaceVertices;
std::vector<GLint> oceanSurfaceIndices;

GLdouble oceanTop = 500;
GLdouble oceanDepth = 1500;

GLuint VAO;
GLuint VBO;
GLuint EBO;

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

/*
    1. use oceanSurfaceVertexCount = stripe, odd. 
    2. define origin1 (x, z) = (0, 0) -> world space(x - n / 2 * unitLength, z - n / 2 * unitLength)
    3. loop from z = 0, x = 0, 1, 2, ... ,n - 1, take adjacent vertices to draw 2 triangles. 
    4. then z = 1, 2, 3, ... , n - 1, repeat step 3.
    5. ** convert 2d index(x, z) to 1d index: index = z * stripe + x.
    6. calculate world space vertices and store them to surfaceVertices
*/ 
void generateSurfaceVertices(){
    GLint indexHalf = oceanSurfaceVertexCount / 2;
    for(int z = 0; z < oceanSurfaceVertexCount; z ++){
        
        for(int x = 0; x < oceanSurfaceVertexCount; x ++){
            GLfloat oceanVertexZ = (z - indexHalf) * oceanSurfaceUnitLength;
            GLfloat oceanVertexX = (x - indexHalf) * oceanSurfaceUnitLength;
            GLfloat oceanVertexY = 0;
        
            oceanSurfaceVertices.push_back({oceanVertexX, oceanVertexY, oceanVertexZ});
        }
    }
}

/*
    The unit squre: 
    A, B
    C, D
*/
// create index buffer : cpu side EBO
// flat 1d arraylist
void generateSurfaceMesh(){
    for(int z = 0; z < oceanSurfaceVertexCount - 1; z++){
        for(int x = 0; x < oceanSurfaceVertexCount - 1; x++){

            // compute a unit square index collection
            // mapping from 2d (x, z) -> 1d index
            // index = row * row_length + col
            int vertexAIndex = z * oceanSurfaceVertexCount + x;
            int vertexBIndex = vertexAIndex + 1;
            int vertexCIndex = (z + 1) * oceanSurfaceVertexCount + x;
            int vertexDIndex = vertexCIndex + 1;

            // following triangle order for future drawing
            // COUNTER CLOCKWISE
            // triangle A -> B -> D
            oceanSurfaceIndices.push_back(vertexAIndex);
            oceanSurfaceIndices.push_back(vertexBIndex);
            oceanSurfaceIndices.push_back(vertexDIndex);
            // triangle A -> D -> C
            oceanSurfaceIndices.push_back(vertexAIndex);
            oceanSurfaceIndices.push_back(vertexDIndex);
            oceanSurfaceIndices.push_back(vertexCIndex);
        }
    }
}

/*  
    VAO: vertex array object : how vertex data is structured 

    VB0: Vertex buffer object : a box of vertex numbers

    EBO: element buffer object : indices for vertices
*/


/*
    VAO remembers:

    1. Which VBO to read vertices from

    2. Which EBO to read indices from

    3. How many floats per attribute

    4. How to stride through vertices

    5. Which attributes are enabled
*/

void createVertexArrayObj(){
    // request one vertex buffer obj
    glGenVertexArrays(1, &VAO);
    // all buffer bindings and attribute definitions are stored inside VAO
    glBindVertexArray(VAO);
}

void createVertexBufferObj(){
    // create a GPU buffer obj for vertex data
    glGenBuffers(1, &VBO);
    // bind
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*
        glBufferData(
            target, 
            size_in_bytes,  
            pointer_to_data, 
            usage_hint (static | dynamic | stream)
        );
    */
    glBufferData(
        GL_ARRAY_BUFFER, // array buffer
        oceanSurfaceVertices.size() * sizeof(std::array<GLfloat,3>), // whole size of the std::vector
        oceanSurfaceVertices.data(), // the data sent to GPU
        GL_STATIC_DRAW // rarely updated
    );
}

void createElementBufferObj(){
    // upload EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, // EBO buffer type
        oceanSurfaceIndices.size() * sizeof(GLuint), // total size
        oceanSurfaceIndices.data(), // its data
        GL_STATIC_DRAW
    );
}

// upload vertices + indices to GPU
void uploadSurfaceToGPU(){
    // call to generate vertices and corresponding indices
    // stored in RAM
    generateSurfaceVertices();
    generateSurfaceMesh();
    
    // VAO
    createVertexArrayObj();

    // VBO
    createVertexBufferObj();
    
    // EBO
    createElementBufferObj();
    
//  void glVertexAttribPointer(
//          GLuint index,   // 0 = position | 1 = normal | 2 = UV(texture coordinate)
//          GLint size,     // how many components this attribute has
//          GLenum type,    // type of each component
//          GLboolean normalized,   // for pos always false
//          GLsizei stride,   // size of [x, y, z]
//          const void *pointer //pos offset
//  );

    // instruct GPU how to read raw vertex data
    // feed attribute vec3 aPos; to ocean.vert
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(std::array<GLfloat,3>),
        (void*)0
    );
    // activate the vertex attribute stream at location 0.
    glEnableVertexAttribArray(0);

    // stop recording VAO
    glBindVertexArray(0);
}

// called every frame
void drawOceanSurface(){
    if(oceanShaderProgram == 0){
        return;
    }
    // bind shader
    glUseProgram(oceanShaderProgram);
    // get GLSL variables
    GLint uFrequency = glGetUniformLocation(oceanShaderProgram, "frequency");
    GLint uWaveAmplitude = glGetUniformLocation(oceanShaderProgram, "waveAmplitude");
    GLint uWaveSpeed = glGetUniformLocation(oceanShaderProgram, "waveSpeed");
    // push attributes to GLSL
    glUniform1f(uFrequency, 0.5f);
    glUniform1f(uWaveAmplitude, 60.0f);
    glUniform1f(uWaveSpeed, 4.0f);

    glPushMatrix();
    glTranslatef(0, 720.0f, 0);
    
    // start record VAO
    glBindVertexArray(VAO);
    /*
        void glDrawElements(
            GLenum mode,        // drawing mode
            GLsizei count,      // how many indices to read from EBO
            GLenum type,        // each index's type
            const void *indices // offset
        )
    */
    
    glDrawElements(GL_TRIANGLES, oceanSurfaceIndices.size(), GL_UNSIGNED_INT, 0);

    // restore state so fixed-function drawing continues to work
    glBindVertexArray(0);

    glPopMatrix();

    // GLUT expects the enum; divide after retrieving milliseconds
    // get time
    float curTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    // get Uniform time
    GLint uCurTime = glGetUniformLocation(oceanShaderProgram, "curTime");
    // push uniform curTime
    glUniform1f(uCurTime, curTime);
    // use Program
    glUseProgram(0);
}
