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
#include "../include/oceanSurface.h"

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
