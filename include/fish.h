#pragma once

#include "helper.h"

MeshGPU uploadFishToGPU(const GPUdata& dataToUpload);

extern MeshGPU fish;

void loadFish();
