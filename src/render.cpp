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


GLUquadric* quad;

GLint sliceCount = 60;
GLint stackCount = 60;

GLint oceanBottomRadius = 1500;
GLdouble oceanBottomPos = -500;

GLfloat oceanSurfaceUnitLength = 60;
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
        GL_RGBA,         // freed RGBA to GPU
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

/*
    Take a file path and return the file content
*/
std::string loadFile(const char* path){
    // load file
    std::ifstream file(path);
    // guard file
    if(!file.is_open()){
        printf("fail to open shader file");
        return "";
    }
    // read tokens
    std::stringstream stringStream;

    // return stringfied file
    stringStream << file.rdbuf();
    return stringStream.str();
}


/*
    take the glsl content string, create a GL shader obj, compile it
    report error message
*/
GLuint compileShader(GLenum type, const char* src){
    // create an empty shader object on GPU with glCreateShader(type). GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
    GLuint shader = glCreateShader(type);
    
    // attach shader source code to the shader using:
    glShaderSource(shader, 1, &src, nullptr);

    // compile source code
    glCompileShader(shader);

    // success = GL_TRUE / GL_FALSE
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    // ------- DEBUG MESSAGE ----------
    if(!success){
        // log length of error message
        GLint logMessageLength = 0;
        // get length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logMessageLength);

        // dynamic char buffer for error text
        std::vector<char> log(logMessageLength);
        glGetShaderInfoLog(shader, logMessageLength, nullptr, log.data());
        if(type == GL_VERTEX_SHADER){
            printf("\n=== Shader Compile Error (VERTEXT) ===\n%s\n\n", log.data());
        }
        else{
            printf("\n=== Shader Compile Error (FRAGMENT) ===\n%s\n\n", log.data());
        }
        // delete shader if fail to compile
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

/*
    load both vertex, fragment shaders, compile them, links them into a program

    check link errors, cleans up the individual shader obj and return the program
*/
GLuint createProgram(const char* vertexShaderPath, const char* fragShaderPath){
    // load vertex shader & fragment shader source code
    std::string vertexShaderSrc = loadFile(vertexShaderPath);
    std::string fragShaderSrc = loadFile(fragShaderPath);
    // get compiled vertex shader
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSrc.c_str());
    // guard
    if(vertexShader == 0){
        printf("fail to compile vertex shader\n");
        return 0;
    }
    // get compiled fragement shader
    GLuint fragmentaShader = compileShader(GL_FRAGMENT_SHADER, fragShaderSrc.c_str());
    // guard
    if(fragmentaShader == 0){
        printf("fail to compile fragment shader\n");
        return 0;
    }
    
    // create an actual GPU shader project obj
    GLuint program = glCreateProgram();
    // attach compiled shaders to program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentaShader);
    // link the program
    glLinkProgram(program);
    // get error message
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if(!success){
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        std::vector<char> log(len);
        glGetProgramInfoLog(program, len, nullptr, log.data());

        printf("\n=== Program Link Error ===\n%s\n\n", log.data());

        // clean up
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentaShader);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentaShader);

    // return valid program
    return program;

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
    glUniform1f(uFrequency, 0.12f);
    glUniform1f(uWaveAmplitude, 30.0f);
    glUniform1f(uWaveSpeed, 2.0f);

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
