#pragma once
#include <vector>

#include "Color.h"
#include "Vertex.h"

class GeometryObject
{
public:
    GeometryObject(std::vector<utils::Vertex> vertices, std::vector<utils::Color> colors,
        std::vector<unsigned> indices);

    [[nodiscard]] const std::vector<utils::Vertex>& GetVertices() const;
    [[nodiscard]] const std::vector<utils::Color>& GetColors() const;
    [[nodiscard]] const std::vector<unsigned>& GetIndices() const;

private:
    // HW_ITEM 10
    const std::vector<utils::Vertex> m_vertices;
    std::vector<utils::Color> m_colors;
    const std::vector<unsigned int> m_indices;
};

GeometryObject makeRainbowCube();
GeometryObject makePlatform(float size);