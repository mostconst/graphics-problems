#pragma once
#include <vector>

#include "VertexArray.h"

namespace nsk_cg
{
class Mesh;
class ElementBuffer;
class ArrayBuffer;
VertexArray loadBuffers(const nsk_cg::Mesh& object, std::vector<nsk_cg::ArrayBuffer>& arrayBuffers,
                        std::vector<nsk_cg::ElementBuffer>& elementBuffers);
VertexArray loadScreenQuadBuffers(std::vector<nsk_cg::ArrayBuffer>& arrayBuffers,
    std::vector<nsk_cg::ElementBuffer>& elementBuffers);
}
