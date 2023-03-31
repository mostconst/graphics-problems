#pragma once
#include <vector>

#include "Color.h"
#include "Vertex.h"

namespace nsk_cg
{
class GeometryObject final
{
public:
    GeometryObject(std::vector<Vertex> vertices, std::vector<utils::Color> colors,
        std::vector<unsigned int> indices);

    [[nodiscard]] const std::vector<Vertex>& GetVertices() const;
    [[nodiscard]] const std::vector<utils::Color>& GetColors() const;
    [[nodiscard]] const std::vector<unsigned int>& GetIndices() const;

private:
    // HW_ITEM 10
    const std::vector<Vertex> m_vertices;
    std::vector<utils::Color> m_colors;
    const std::vector<unsigned int> m_indices;
};

GeometryObject makeRainbowCube();
GeometryObject makePlatform(float size);
}