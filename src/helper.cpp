#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include "../include/stb_image.h"
#include "../include/render.h"
#include "../include/submarine.h"

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
