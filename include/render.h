#include <GLUT/glut.h>
#include <array>
#include "../include/helper.h"

void drawSubmarine();
void loadSubmarineFile();
void drawDisk();
void drawCylinder();

void drawOceanSurface();

void drawCoral(const MeshGPU& coral, const std::array<GLfloat, 2> pos);


extern GLUquadric* quad;

extern GLint sliceCount;
extern GLint stackCount;

extern GLint oceanBottomRadius;

