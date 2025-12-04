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
#include "../include/oceanSurface.h"
#include "../include/coral.h"
GLUquadric* quad;

GLint sliceCount = 60;
GLint stackCount = 60;

GLint oceanBottomRadius = 1500;
GLdouble oceanBottomPos = -500;

void drawSubmarine(){
    initialiseSubmarineMaterial();
    glPushMatrix();
    glTranslatef(submarinCurrentPos[0], submarinCurrentPos[1], submarinCurrentPos[2]);

    glBegin(GL_TRIANGLES);
        /*
            looks like :{       
                            { {1,1}, {3, 1}, {5, 1} }, <-- face
                            { {1,2}, {3, 2}, {5, 2} },
                            { {v1,n}, {v2,n}, {v3,,n}}
                        }
        */ 
        for(auto &face : submarineFaceListWithNormal){
            int normalIdxInNormalList = face[0][1] - 1;

            int vertexIdx1InVertexList = face[0][0] - 1;

            int vertexIdx2InVertexList = face[1][0] - 1;

            int vertexIdx3InVertexList = face[2][0] - 1;
            
            GLfloat* vertex1 = submarineVertexList[vertexIdx1InVertexList].data();
            GLfloat* vertex2 = submarineVertexList[vertexIdx2InVertexList].data();
            GLfloat* vertex3 = submarineVertexList[vertexIdx3InVertexList].data();

            GLfloat* curNormal = submarineNormalList[normalIdxInNormalList].data();

            glNormal3fv(curNormal);
            glVertex3fv(vertex1);
            glVertex3fv(vertex2);
            glVertex3fv(vertex3);
        }
    glEnd();
    glPopMatrix();
}

/*  
    disk should lies on y = -100
*/

// add highly emissive material
void applySandEmission(){
    GLfloat ambient[] = {0.30f, 0.25f, 0.15f, 1.0f};  // warm sandy brown
    GLfloat diffuse[] = {0.70f, 0.65f, 0.45f, 1.0f};  // visible sand color
    GLfloat specular[] = {0.10f, 0.10f, 0.10f, 1.0f};  // sand is matte, not shiny
    GLfloat emission[] = {0.1f, 0.08f, 0.04f, 1.0f};  // subtle golden self-light
    GLfloat shininess = 8.0f;                         // wide dull highlights
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

}

void drawDisk(){
    applySandEmission();
    // *** The last transform call affects the object first. ***
    glPushMatrix();
    glTranslatef(0, oceanBottomPos, 0);
    glRotatef(-90, 1, 0, 0);
    
    // inner : 0 -> filled
    // loop : 1 -> ring
    /*
        gluDisk(GLUquadric* quad,
            GLdouble innerRadius,
            GLdouble outerRadius,
            GLint slices,
            GLint loops);
    */
    gluDisk(quad, 0, oceanBottomRadius, sliceCount, 1);
    glPopMatrix();
}

void drawCylinder(){
    applySandEmission();
    glPushMatrix();
    glTranslatef(0, oceanBottomPos - 50, 0);
    glRotatef(-90, 1, 0, 0);
    /*
        gluCylinder(GLUquadric *quad,
            GLdouble baseRadius,
            GLdouble topRadius,
            GLdouble height,
            GLint slices,
            GLint stacks);
    */
    gluCylinder(quad, oceanBottomRadius -20, 
                oceanBottomRadius - 20, 
                oceanDepth, 
                sliceCount, 
                stackCount
    );

    glPopMatrix();
}

void updateOceanShader(){
    // get GLSL variables
    GLint uFrequency = glGetUniformLocation(oceanShaderProgram, "frequency");
    GLint uWaveAmplitude = glGetUniformLocation(oceanShaderProgram, "waveAmplitude");
    GLint uWaveSpeed = glGetUniformLocation(oceanShaderProgram, "waveSpeed");
    // push attributes to GLSL
    glUniform1f(uFrequency, 0.5f);
    glUniform1f(uWaveAmplitude, 60.0f);
    glUniform1f(uWaveSpeed, 4.0f);

    // GLUT expects the enum; divide after retrieving milliseconds
    // get time
    float curTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    // get Uniform time
    GLint uCurTime = glGetUniformLocation(oceanShaderProgram, "curTime");
    // push uniform curTime
    glUniform1f(uCurTime, curTime);

    // ------- lighting -------

    GLint uLightPos = glGetUniformLocation(oceanShaderProgram, "lightPos");
    GLint uLightColor = glGetUniformLocation(oceanShaderProgram, "lightColor");
    GLint uMatDiffuse = glGetUniformLocation(oceanShaderProgram, "matDiffuse");
    GLint uMatSpecular = glGetUniformLocation(oceanShaderProgram, "matSpecular");
    GLint uMatShininess = glGetUniformLocation(oceanShaderProgram, "matShininess");
    glUniform3f(uLightPos,  600.0f, 300.0f, 200.0f);  // e.g. sun above water
    glUniform3f(uLightColor, 1.0f, 1.0f, 0.95f);

    // material tuning
    glUniform3f(uMatDiffuse,  0.0f, 0.3f, 0.8f);  // underwater teal
    glUniform3f(uMatSpecular, 0.6f, 0.6f, 0.6f);
    glUniform1f(uMatShininess, 64.0f);
}

// called every frame
void drawOceanSurface(){
    if(oceanShaderProgram == 0){
        return;
    }
    // bind shader
    glUseProgram(oceanShaderProgram);
    
    updateOceanShader();

    glPushMatrix();
    glTranslatef(0, 720.0f, 0);

    // start record VAO
    glBindVertexArrayAPPLE(VAO);
    glDrawElements(GL_TRIANGLES, oceanSurfaceIndices.size(), GL_UNSIGNED_INT, 0);
    // restore state so fixed-function drawing continues to work
    glBindVertexArrayAPPLE(0);

    glPopMatrix();

    // unuse Program
    glUseProgram(0);
}

void drawCoral(const MeshGPU& coral, const std::array<GLfloat, 2> pos){
    glPushMatrix();
    // scale coral up

    glTranslatef(pos[0], -500, pos[1]);
    glScalef(200, 200, 200);
    
    glBindVertexArrayAPPLE(coral.VAO);
    glDrawElements(GL_TRIANGLES, coral.indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArrayAPPLE(0);
    glPopMatrix();
}

void drawCorals(){
    drawCoral(coralMesh1, coralPoses[1]);
    drawCoral(coralMesh2, coralPoses[2]);
    drawCoral(coralMesh3, coralPoses[3]);
    drawCoral(coralMesh4, coralPoses[4]);
    drawCoral(coralMesh5, coralPoses[5]);
    drawCoral(coralMesh6, coralPoses[6]);
    drawCoral(coralMesh7, coralPoses[7]);
    drawCoral(coralMesh8, coralPoses[8]);
    drawCoral(coralMesh9, coralPoses[9]);
    drawCoral(coralMesh10, coralPoses[10]);
    drawCoral(coralMesh11, coralPoses[11]);
    drawCoral(coralMesh12, coralPoses[12]);
    drawCoral(coralMesh13, coralPoses[13]);
    drawCoral(coralMesh14, coralPoses[14]);
}