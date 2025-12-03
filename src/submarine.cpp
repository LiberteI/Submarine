#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>

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
    // warm yellow base with subtle highlight
    GLfloat ambient[] = {0.25f, 0.20f, 0.05f, 1.0f};
    GLfloat diffuse[] = {0.9f, 0.75f, 0.15f, 1.0f};
    GLfloat specular[] = {0.3f, 0.25f, 0.15f, 1.0f};
    GLfloat emission[] = {0.05f, 0.04f, 0.01f, 1.0f};
    GLfloat shininess = 24.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT,  ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

}
