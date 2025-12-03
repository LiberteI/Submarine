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
};
/*  
    CORAL.CPP
    This file performs 
    1. reading files
    2. uploading VAO, VBO, EBO to GPU
    3. specifying corals' material and upload it to GLSL
*/

std::vector<GLfloat> getVertexListFromFile(const char* filePath){
    std::vector<GLfloat> vertexList;
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
        if(identifier != "v"){
            continue;
        }

        GLfloat token1, token2, token3;
        curTokenizedLine >> token1;
        curTokenizedLine >> token2;
        curTokenizedLine >> token3;

        vertexList.push_back(token1);
        vertexList.push_back(token2);
        vertexList.push_back(token3);
    }
    return vertexList;
}

std::vector<GLfloat> getNormalListFromFile(const char* filePath){
    std::vector<GLfloat> normalList;
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
        if(identifier != "vn"){
            continue;
        }

        GLfloat token1, token2, token3;
        curTokenizedLine >> token1;
        curTokenizedLine >> token2;
        curTokenizedLine >> token3;

        normalList.push_back(token1);
        normalList.push_back(token2);
        normalList.push_back(token3);
        
    }
    return normalList;
}

// return a struct 
objData getFaceFromFile(const char* filePath){
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

