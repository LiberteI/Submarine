// Utility helpers for loading shaders, textures, and OBJ parsing
#pragma once

#include <string>
#include <array>
#include <vector>
#include <GLUT/glut.h>
GLuint createProgram(const char* vertexShaderSrc, const char* fragShaderSrc);
std::string loadFile(const char* path);
GLuint loadTexture(const char* filePath);

struct ObjData{
    std::vector<GLint> vertexInstruction;
    std::vector<GLint> normalInstruction;
    std::vector<GLfloat> vertexList;
    std::vector<GLfloat> normalList;
};
ObjData getObjDataFromFile(const char* filePath);
struct GPUdata{
    // expect v3, n3, v3, n3
    std::vector<GLfloat> VBO;
    std::vector<GLuint> EBO; // 0 1 2 3 4 5....
};
GPUdata getGPUData(const char* filePath);
void uploadSurfaceToGPU();
std::array<GLint, 2> parseToken(std::string token);
void drawOriginDebugger();
