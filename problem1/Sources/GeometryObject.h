#pragma once
#include <vector>

class GeometryObject
{
public:
    GeometryObject(std::vector<float> vertices, std::vector<float> colors,
        std::vector<unsigned> indices);

    [[nodiscard]] const std::vector<float>& GetVertices() const;
    [[nodiscard]] const std::vector<float>& GetColors() const;
    [[nodiscard]] const std::vector<unsigned>& GetIndices() const;

private:
    const std::vector<float> m_vertices;
    std::vector<float> m_colors;
    const std::vector<unsigned int> m_indices;
};

GeometryObject makeRainbowCube();
GeometryObject makePlatform();