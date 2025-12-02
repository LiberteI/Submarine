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
#include "../include/submarine.h"
#include "../include/helper.h"
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
