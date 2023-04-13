#pragma once
#include "IndexedVertices.h"
#include "Mesh.h"

namespace nsk_cg
{
IndexedVertices makeCubeWithUniqueVertices();
Mesh makeCubeForLighting();
Mesh makeSphere(int tesselationLevel);
}
