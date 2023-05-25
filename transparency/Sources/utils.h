#pragma once

#include <vector>
#include "DrawData.h"
#include "ArrayBuffer.h"
#include "VertexArray.h"


namespace nsk_cg
{
    class SceneObjects
    {
    public:
        SceneObjects(std::vector<ExtendedDrawData> opaqueObjects,
            std::vector<ExtendedDrawData> transparentObjects);
        const std::vector<ExtendedDrawData>& GetOpaqueObjects() const;
        const std::vector<ExtendedDrawData>& GetTransparentObjects() const;
    private:
        const std::vector<ExtendedDrawData> opaqueObjects;
        const std::vector<ExtendedDrawData> transparentObjects;
    };

    SceneObjects prepareScene(std::vector<ArrayBuffer>& arrayBuffers, std::vector<ElementBuffer>& elementBuffers, std::vector<
                                  VertexArray>& vertexArrays);
}