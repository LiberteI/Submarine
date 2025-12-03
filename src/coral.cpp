#define GL_SILENCE_DEPRECATION
#include "../include/helper.h"
#include <vector>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <fstream>
#include <sstream>
#include <array>
struct ObjData{
    std::vector<GLint> vertexInstruction;
    std::vector<GLint> normalInstruction;
    std::vector<GLfloat> vertexList;
    std::vector<GLfloat> normalList;
};
/*  
    CORAL.CPP
    This file performs 
    1. reading files
    2. uploading VAO, VBO, EBO to GPU
    3. specifying corals' material and upload it to GLSL
*/

// return a struct 
ObjData getObjDataFromFile(const char* filePath){
    ObjData data;
    // load file
    std::ifstream file(filePath);
    // guard
    if(!file.is_open()){
        printf("fail to open %s \n", filePath);
        return {};
    }

    std::string currentLine;

    // read the next line and store it in "currentLine"
    while(std::getline(file, currentLine)){
        // get current string stream version of currentLine, so that we can use >> effectively
        std::stringstream curTokenizedLine(currentLine);

        std::string identifier;
        // get the first token
        curTokenizedLine >> identifier;
        if(identifier != "f"){
            if(identifier == "v"){
                GLfloat token1, token2, token3;
                curTokenizedLine >> token1;
                curTokenizedLine >> token2;
                curTokenizedLine >> token3;

                data.vertexList.push_back(token1);
                data.vertexList.push_back(token2);
                data.vertexList.push_back(token3);
            }
            if(identifier == "vn"){
                GLfloat token1, token2, token3;
                curTokenizedLine >> token1;
                curTokenizedLine >> token2;
                curTokenizedLine >> token3;

                data.normalList.push_back(token1);
                data.normalList.push_back(token2);
                data.normalList.push_back(token3);
            }
            continue;
        }

        std::string token1, token2, token3;
        curTokenizedLine >> token1;
        curTokenizedLine >> token2;
        curTokenizedLine >> token3;
        std::array<GLint, 2> parsedToken1 = parseToken(token1);
        std::array<GLint, 2> parsedToken2 = parseToken(token2);
        std::array<GLint, 2> parsedToken3 = parseToken(token3);

        data.vertexInstruction.push_back(parsedToken1[0]);
        data.vertexInstruction.push_back(parsedToken2[0]);
        data.vertexInstruction.push_back(parsedToken3[0]);

        data.normalInstruction.push_back(parsedToken1[1]);
        data.normalInstruction.push_back(parsedToken2[1]);
        data.normalInstruction.push_back(parsedToken3[1]);
    }
    return data;
}

struct GPUdata{
    // expect v3, n3, v3, n3
    std::vector<GLfloat> VBO;
    std::vector<GLuint> EBO; // 0 1 2 3 4 5....
};

GPUdata getGPUData(const char* filePath){
    /*
        read instruction and form a std::vector 3v 3n 3v 3n ....
    */
    GPUdata gpuData;
    ObjData objData = getObjDataFromFile(filePath);
    
    int faceCount = objData.vertexInstruction.size();

    for(int i = 0; i < faceCount; i++){
        // Obj indices are 1 based
        int vIndex = objData.vertexInstruction[i] - 1;
        int nIndex = objData.normalInstruction[i] - 1;

        // stride in raw arrays = 3 floats
        int vBase = vIndex * 3;
        int nBase = nIndex * 3;

        // --- Push Position ---
        gpuData.VBO.push_back(objData.vertexList[vBase + 0]);
        gpuData.VBO.push_back(objData.vertexList[vBase + 1]);
        gpuData.VBO.push_back(objData.vertexList[vBase + 2]);

        // --- Push Normal ---
        gpuData.VBO.push_back(objData.normalList[nBase + 0]);
        gpuData.VBO.push_back(objData.normalList[nBase + 1]);
        gpuData.VBO.push_back(objData.normalList[nBase + 2]);

        // --- EBO ---
        // size = 0, 1, 2, 3, 4.......
        gpuData.EBO.push_back(static_cast<GLuint>(gpuData.EBO.size()));
    }
    return gpuData;
}

