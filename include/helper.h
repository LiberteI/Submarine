// Utility helpers for loading shaders, textures, and OBJ parsing
#pragma once

#include <string>
#include <array>

#include <GLUT/glut.h>
GLuint createProgram(const char* vertexShaderSrc, const char* fragShaderSrc);
std::string loadFile(const char* path);
GLuint loadTexture(const char* filePath);

struct ObjData;
ObjData getObjDataFromFile(const char* filePath);
struct GPUdata;
GPUdata getGPUData(const char* filePath);
void uploadSurfaceToGPU();
std::array<GLint, 2> parseToken(std::string token);
void drawOriginDebugger();
