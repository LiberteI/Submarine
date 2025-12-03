#define GL_SILENCE_DEPRECATION
#include "../include/helper.h"
#include <vector>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <fstream>
#include <sstream>
#include <array>
struct objData{
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
objData getObjDataFromFile(const char* filePath){
    objData data;
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
    std::vector<GLint> EBO;
};

GPUdata getGPUData(){
    /*
        read instruction and form a std::vector 3v 3n 3v 3n ....
    */
   
}

