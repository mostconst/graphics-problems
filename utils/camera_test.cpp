#include <algorithm>
#include <gtest/gtest.h>
#include <glm/glm.hpp>

#include "Color.h"
#include "comparison.h"
#include "math_util.h"
#include "Mesh.h"
#include "tesselate.h"
#include "VertexTriangle.h"
#include "glm/gtx/string_cast.hpp"

glm::vec3 doPerspectiveProjection(const glm::mat4& perspective, const glm::vec3& v)
{
    const glm::vec4 vProj = perspective * glm::vec4(v, 1.0f);
    return { vProj / vProj.w };
}

namespace nsk_cg
{
    bool verticesUnique(const std::vector<Vertex>& vertices)
    {
        if (vertices.size() < 2)
        {
            return true;
        }

        for (size_t i = 0; i < vertices.size() - 1; ++i)
        {
            const Vertex& vertex1 = vertices[i];
            for (size_t j = i + 1; j < vertices.size(); ++j)
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

    std::vector<VertexTriangle> GetVertexTriangles(const Mesh& mesh)
    {
        std::vector<VertexTriangle> res;
        const auto& vertices = mesh.GetVertices();
        for (const auto& triangle : mesh.GetTriangles())
        {
            res.emplace_back(vertices[triangle.First()], vertices[triangle.Second()], vertices[triangle.Third()]);
        }

        return res;
    }

}

TEST(UtilTest, UniqueVertices)
{
    const std::vector<nsk_cg::Vertex> vertices{ {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f } };
    ASSERT_FALSE(verticesUnique(vertices));
}

TEST(UtilTest, TriangleNormal)
{
    nsk_cg::VertexTriangle triangle(
        { 0.0f, 0.0f, 0.0f },
        { 4.0f, 0.0f, 0.0f },
        { 4.0f, 3.0f, 0.0f }
        );
    ASSERT_TRUE(nsk_cg::vectorsEqual(nsk_cg::getNormalVector(triangle), glm::vec3(0.0f, 0.0f, 1.0f)));
}

TEST(TesselateTest, UniqueVertices)
{
    const auto mesh = nsk_cg::tesselateIterative(1);
    ASSERT_TRUE(verticesUnique(mesh.GetVertices()));
    const std::vector<nsk_cg::VertexTriangle> vertexTriangles = nsk_cg::GetVertexTriangles(mesh);
    const bool trianglesCorrect = std::all_of(vertexTriangles.begin(), vertexTriangles.end(), nsk_cg::isCorrect);
    ASSERT_TRUE(trianglesCorrect);
}

TEST(TriangleTest, IncorrectTriangles)
{
    const nsk_cg::VertexTriangle triangle1({ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f });
    ASSERT_FALSE(nsk_cg::isCorrect(triangle1));
    const nsk_cg::VertexTriangle triangle2({ 1.0f, 0.0f, 0.0f }, { 2.0f, 0.0f, 0.0f }, { 3.0f, 0.0f, 0.0f });
    ASSERT_FALSE(nsk_cg::isCorrect(triangle2));
}


TEST(HelloTest, TestMatrix)
{
    const glm::mat4 matrix = nsk_cg::math_utils::perspective(2.0f, 4.0f, 2.0f, 4.0f);

    EXPECT_EQ(glm::vec3(1.0f, 1.0f, -1.0f),
              doPerspectiveProjection(matrix, glm::vec3(1.0f, 2.0f, -2.0f))
    );
    EXPECT_EQ(glm::vec3(-1.0f, 1.0f, -1.0f),
        doPerspectiveProjection(matrix, glm::vec3(-1.0f, 2.0f, -2.0f))
    );
    EXPECT_EQ(glm::vec3(1.0f, 1.0f, 1.0f),
        doPerspectiveProjection(matrix, glm::vec3(2.0f, 4.0f, -4.0f))
    );
}

TEST(ColorTest, Colors)
{
    const nsk_cg::utils::Color blue(0x0000FF);
    EXPECT_EQ(0.0f, blue.GetRed());
    EXPECT_EQ(1.0f, blue.GetBlue());
    EXPECT_EQ(0.0f, blue.GetGreen());

    const nsk_cg::utils::Color green(0x00FF00);
    EXPECT_EQ(0.0f, green.GetRed());
    EXPECT_EQ(0.0f, green.GetBlue());
    EXPECT_EQ(1.0f, green.GetGreen());

    const nsk_cg::utils::Color red(0xFF0000);
    EXPECT_EQ(1.0f, red.GetRed());
    EXPECT_EQ(0.0f, red.GetBlue());
    EXPECT_EQ(0.0f, red.GetGreen());
}