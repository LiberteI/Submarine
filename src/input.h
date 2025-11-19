#include <GLUT/glut.h>
void myKeyboardDown(unsigned char key, int x, int y);

void myKeyboardUp(unsigned char key, int x, int y);

void trySwitchPolygonMode();

void recordNormalWindow();

void tryResizeWindow();
extern GLint windowWidth;
extern GLint windowHeight;
extern GLint uPressedTimes;
extern GLint fPressedTimes;

extern GLint windowXPos;
extern GLint windowYPos;