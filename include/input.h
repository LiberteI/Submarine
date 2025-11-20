#include <GLUT/glut.h>
void myKeyboardDown(unsigned char key, int x, int y);

void myKeyboardUp(unsigned char key, int x, int y);

void trySwitchPolygonMode();

void recordNormalWindow();

void tryResizeWindow();

void specialKeyDown(int key, int, int);

void specialKeyUp(int key, int, int);

void myMousePassiveMotion(int x, int y);

void tryMove();

extern GLint windowWidth;
extern GLint windowHeight;
extern GLint uPressedTimes;
extern GLint fPressedTimes;

extern GLint windowXPos;
extern GLint windowYPos;

extern std::array<GLfloat, 3> cameraCurrentPos;

extern GLfloat speed;
extern std::array<GLfloat, 3> submarinCurrentPos;

extern bool wHeld;
extern bool sHeld;
extern bool aHeld;
extern bool dHeld;

extern bool upHeld;
extern bool downHeld;

extern GLfloat curMouseXPos;
extern GLfloat curMouseYPos;