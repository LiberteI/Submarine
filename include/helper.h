GLuint createProgram(const char* vertexShaderSrc, const char* fragShaderSrc);
std::string loadFile(const char* path);
GLuint loadTexture(const char* filePath);
extern void uploadSurfaceToGPU();
std::array<GLint, 2> parseToken(std::string token);
void drawOriginDebugger();