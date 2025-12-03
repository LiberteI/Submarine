#include "../include/helper.h"

MeshGPU coralMesh1;
MeshGPU coralMesh2;
MeshGPU coralMesh3;
MeshGPU coralMesh4;
MeshGPU coralMesh5;
MeshGPU coralMesh6;
MeshGPU coralMesh7;
MeshGPU coralMesh8;
MeshGPU coralMesh9;
MeshGPU coralMesh10;
MeshGPU coralMesh11;
MeshGPU coralMesh12;
MeshGPU coralMesh13;
MeshGPU coralMesh14;

void loadCorals(){
    GPUdata coral1 = getGPUData("assets/coral/coral_1.obj");
    GPUdata coral2 = getGPUData("assets/coral/coral_2.obj");
    GPUdata coral3 = getGPUData("assets/coral/coral_3.obj");
    GPUdata coral4 = getGPUData("assets/coral/coral_4.obj");
    GPUdata coral5 = getGPUData("assets/coral/coral_5.obj");
    GPUdata coral6 = getGPUData("assets/coral/coral_6.obj");
    GPUdata coral7 = getGPUData("assets/coral/coral_7.obj");
    GPUdata coral8 = getGPUData("assets/coral/coral_8.obj");
    GPUdata coral9 = getGPUData("assets/coral/coral_9.obj");
    GPUdata coral10 = getGPUData("assets/coral/coral_10.obj");
    GPUdata coral11 = getGPUData("assets/coral/coral_11.obj");
    GPUdata coral12 = getGPUData("assets/coral/coral_12.obj");
    GPUdata coral13 = getGPUData("assets/coral/coral_13.obj");
    GPUdata coral14 = getGPUData("assets/coral/coral_14.obj");

    coralMesh1 = uploadToGPU(coral1);
    coralMesh2 = uploadToGPU(coral2);
    coralMesh3 = uploadToGPU(coral3);
    coralMesh4 = uploadToGPU(coral4);
    coralMesh5 = uploadToGPU(coral5);
    coralMesh6 = uploadToGPU(coral6);
    coralMesh7 = uploadToGPU(coral7);
    coralMesh8 = uploadToGPU(coral8);
    coralMesh9 = uploadToGPU(coral9);
    coralMesh10 = uploadToGPU(coral10);
    coralMesh11 = uploadToGPU(coral11);
    coralMesh12 = uploadToGPU(coral12);
    coralMesh13 = uploadToGPU(coral13);
    coralMesh14 = uploadToGPU(coral14);
    // printf("coralMesh.VAO %d\n", coralMesh.VAO);
    
}
