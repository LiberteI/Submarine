
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <cmath>
#include <vector>
#include "../include/helper.h"

// fish geometry on CPU
std::vector<GLfloat> fishVertices;
std::vector<GLuint> fishMesh;

// create single fish's vertices
void generateFishVertices(){
    if(!fishVertices.empty()){
        return;
    }

    fishVertices = {
        // head tip
        0.0f, 0.0f, 0.0f,
        // tail ring
        -30.0f, -50.0f, -30.0f,
        -30.0f, -50.0f, 30.0f,
        30.0f, -50.0f, 30.0f,
        30.0f, -50.0f, -30.0f
    };
}

void generateFishMesh(){
    if(!fishMesh.empty()){
        return;
    }
    // fish's CPU side EBO
    fishMesh = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        1, 2, 3,
        1, 3, 4
    };
}

std::vector<GLfloat> computeFishNormals(){
    // initialise normal list
    std::vector<GLfloat> normals(fishVertices.size(), 0.0f);

    for(size_t i = 0; i + 2 < fishMesh.size(); i += 3){
        // get indices
        GLuint ia = fishMesh[i + 0] * 3;
        GLuint ib = fishMesh[i + 1] * 3;
        GLuint ic = fishMesh[i + 2] * 3;

        // A->B and A->C
        GLfloat abx = fishVertices[ib + 0] - fishVertices[ia + 0];
        GLfloat aby = fishVertices[ib + 1] - fishVertices[ia + 1];
        GLfloat abz = fishVertices[ib + 2] - fishVertices[ia + 2];

        GLfloat acx = fishVertices[ic + 0] - fishVertices[ia + 0];
        GLfloat acy = fishVertices[ic + 1] - fishVertices[ia + 1];
        GLfloat acz = fishVertices[ic + 2] - fishVertices[ia + 2];

        // face normal = cross(AB, AC)
        GLfloat nx = aby * acz - abz * acy;
        GLfloat ny = abz * acx - abx * acz;
        GLfloat nz = abx * acy - aby * acx;

        // accumulate per-vertex
        normals[ia + 0] += nx; normals[ia + 1] += ny; normals[ia + 2] += nz;
        normals[ib + 0] += nx; normals[ib + 1] += ny; normals[ib + 2] += nz;
        normals[ic + 0] += nx; normals[ic + 1] += ny; normals[ic + 2] += nz;
    }

    // normalize accumulated normals
    for(size_t v = 0; v + 2 < normals.size(); v += 3){
        // get per normal
        GLfloat nx = normals[v + 0];
        GLfloat ny = normals[v + 1];
        GLfloat nz = normals[v + 2];
        // compute the length
        GLfloat len = std::sqrt(nx * nx + ny * ny + nz * nz);
        // make normal's unit length = 1
        if(len > 0.0001f){
            normals[v + 0] /= len;
            normals[v + 1] /= len;
            normals[v + 2] /= len;
        }
    }
    return normals;
}

GPUdata buildFishGPUdata(){
    // get CPU side VBO EBO
    generateFishVertices();
    generateFishMesh();

    GPUdata fishGPUdata;
    // get normal list
    std::vector<GLfloat> normals = computeFishNormals();
    // vertex count 
    const size_t vertexCount = fishVertices.size() / 3;

    // resize vector size to be 3v 3n 3v 3n....
    fishGPUdata.VBO.reserve(vertexCount * 6);

    // form VBO vector
    for(size_t i = 0; i < vertexCount; ++i){
        const size_t base = i * 3;
        // position
        fishGPUdata.VBO.push_back(fishVertices[base + 0]);
        fishGPUdata.VBO.push_back(fishVertices[base + 1]);
        fishGPUdata.VBO.push_back(fishVertices[base + 2]);
        // normal
        fishGPUdata.VBO.push_back(normals[base + 0]);
        fishGPUdata.VBO.push_back(normals[base + 1]);
        fishGPUdata.VBO.push_back(normals[base + 2]);
    }

    // insert: copy fishMesh into fishGPUdata.EBO
    // v.insert(v.end(), other.begin(), other.end());
    fishGPUdata.EBO.insert(fishGPUdata.EBO.end(), fishMesh.begin(), fishMesh.end());
    return fishGPUdata;
}

MeshGPU uploadFishToGPU(){
    GPUdata fishGPUdata = buildFishGPUdata();
    return uploadToGPU(fishGPUdata);
}
