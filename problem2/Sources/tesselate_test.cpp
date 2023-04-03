#include "tesselate.h"

#include <gtest/gtest.h>

bool verticesUnique(const std::vector<nsk_cg::Vertex>& vertices)
{
    if (vertices.size() < 2)
    {
        return true;
    }

    for(size_t i = 0; i < vertices.size() - 1; ++i)
    {
        const nsk_cg::Vertex& vertex1 = vertices[i];
        for(size_t j = i + 1; j < vertices.size(); ++j)
        {
            const nsk_cg::Vertex& vertex2 = vertices[j];
            if (equal(vertex1, vertex2))
            {
                return false;
            }
        }
    }

    return true;
}

TEST(UtilTest, UniqueVertices)
{
    const std::vector<nsk_cg::Vertex> vertices{ {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f }};
    ASSERT_FALSE(verticesUnique(vertices));
}

TEST(TesselateTest, UniqueVertices)
{
    const auto mesh = nsk_cg::tesselateIterative(1);
    ASSERT_TRUE(verticesUnique(mesh.GetVertices()));
} 