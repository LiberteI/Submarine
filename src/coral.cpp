#include "../include/helper.h"

MeshGPU coralMesh;

void loadCorals(){
    GPUdata coral1 = getGPUData("assets/coral/coral_1.obj");
    // GPUdata coral2 = getGPUData("assets/coral/coral_2.obj");
    // GPUdata coral3 = getGPUData("assets/coral/coral_3.obj");
    // GPUdata coral4 = getGPUData("assets/coral/coral_4.obj");
    // GPUdata coral5 = getGPUData("assets/coral/coral_5.obj");
    // GPUdata coral6 = getGPUData("assets/coral/coral_6.obj");
    // GPUdata coral7 = getGPUData("assets/coral/coral_7.obj");
    // GPUdata coral8 = getGPUData("assets/coral/coral_8.obj");
    // GPUdata coral9 = getGPUData("assets/coral/coral_9.obj");
    // GPUdata coral10 = getGPUData("assets/coral/coral_10.obj");
    // GPUdata coral11 = getGPUData("assets/coral/coral_11.obj");
    // GPUdata coral12 = getGPUData("assets/coral/coral_12.obj");
    // GPUdata coral13 = getGPUData("assets/coral/coral_13.obj");
    // GPUdata coral14 = getGPUData("assets/coral/coral_14.obj");

    coralMesh = uploadToGPU(coral1);
    // printf("coralMesh.VAO %d\n", coralMesh.VAO);
    // coralMeshes.push_back(uploadToGPU(coral2));
    // coralMeshes.push_back(uploadToGPU(coral3));
    // coralMeshes.push_back(uploadToGPU(coral4));
    // coralMeshes.push_back(uploadToGPU(coral5));
    // coralMeshes.push_back(uploadToGPU(coral6));
    // coralMeshes.push_back(uploadToGPU(coral7));
    // coralMeshes.push_back(uploadToGPU(coral8));
    // coralMeshes.push_back(uploadToGPU(coral9));
    // coralMeshes.push_back(uploadToGPU(coral10));
    // coralMeshes.push_back(uploadToGPU(coral11));
    // coralMeshes.push_back(uploadToGPU(coral12));
    // coralMeshes.push_back(uploadToGPU(coral13));
    // coralMeshes.push_back(uploadToGPU(coral14));
}
