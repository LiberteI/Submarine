// Utility helpers for loading shaders, textures, and OBJ parsing
#pragma once

#include <string>
#include <array>
#include <GL/glew.h>

GLuint createProgram(const char* vertexShaderSrc, const char* fragShaderSrc);
std::string loadFile(const char* path);
GLuint loadTexture(const char* filePath);
void uploadSurfaceToGPU();
std::array<GLint, 2> parseToken(std::string token);
void drawOriginDebugger();
