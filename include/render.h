#include <GLUT/glut.h>
#include <array>
void drawOriginDebugger();
std::array<GLint, 2> parseToken(std::string token);
void drawSubmarine();
void loadSubmarineFile();
void drawDisk();
void drawCylinder();
GLuint loadTexture(const char* filePath);
void drawOceanSurface();
void generateSurfaceMesh();
GLuint createProgram(const char* vertexShaderSrc, const char* fragShaderSrc);

extern GLUquadric* quad;

extern GLint sliceCount;
extern GLint stackCount;

extern std::vector<std::array<GLfloat, 3>> submarineVertexList;
extern std::vector<std::array<GLfloat, 3>> submarineNormalList;

extern GLint oceanBottomRadius;

extern void uploadSurfaceToGPU();