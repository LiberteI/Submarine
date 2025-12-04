#pragma once

#include "helper.h"

MeshGPU uploadFishToGPU(const GPUdata& dataToUpload);

extern MeshGPU fish1;
extern MeshGPU fish2;

void loadFish();
