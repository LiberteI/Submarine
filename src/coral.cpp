#include "../include/helper.h"



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

    MeshGPU coral1mesh = uploadToGPU(coral1);
    MeshGPU coral2mesh = uploadToGPU(coral2);
    MeshGPU coral3mesh = uploadToGPU(coral3);
    MeshGPU coral4mesh = uploadToGPU(coral4);
    MeshGPU coral5mesh = uploadToGPU(coral5);
    MeshGPU coral6mesh = uploadToGPU(coral6);
    MeshGPU coral7mesh = uploadToGPU(coral7);
    MeshGPU coral8mesh = uploadToGPU(coral8);
    MeshGPU coral9mesh = uploadToGPU(coral9);
    MeshGPU coral10mesh = uploadToGPU(coral10);
    MeshGPU coral11mesh = uploadToGPU(coral11);
    MeshGPU coral12mesh = uploadToGPU(coral12);
    MeshGPU coral13mesh = uploadToGPU(coral13);
    MeshGPU coral14mesh = uploadToGPU(coral14);
}
