#include "utils.h"

#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "buffer_loading.h"
#include "constants.h"
#include "DrawData.h"
#include "lighting_utils.h"
#include "MeshBuilding.h"
#include "VertexArray.h"

namespace nsk_cg
{
Material makeTransparentMaterial(const glm::vec4& color)
{
    return {
        glm::vec3(color),
        {1.0f, 0.5f, 0.31f},
        {0.5f, 0.5f, 0.5f},
        32.0f,
        color.a
    };
}

struct RectangleParameters
{
    const glm::vec3 location;
    const Material material;
};

const std::vector<ExtendedDrawData>& SceneObjects::GetOpaqueObjects() const
{
    return opaqueObjects;
}

SceneObjects::SceneObjects(std::vector<ExtendedDrawData> opaqueObjects,
                           std::vector<ExtendedDrawData> transparentObjects) :
    opaqueObjects(std::move(opaqueObjects)),
    transparentObjects(std::move(transparentObjects))
{
}

const std::vector<ExtendedDrawData>& SceneObjects::GetTransparentObjects() const
{
    return transparentObjects;
}

SceneObjects prepareScene(std::vector<ArrayBuffer>& arrayBuffers, std::vector<ElementBuffer>& elementBuffers,
                          std::vector<
                              VertexArray>& vertexArrays)
{
    const Mesh cube = makeCubeForLighting();
    const Mesh rectangleMesh = makeRectangle(2.0f);
    vertexArrays.push_back(loadBuffers(cube, arrayBuffers, elementBuffers));
    vertexArrays.push_back(loadBuffers(rectangleMesh, arrayBuffers, elementBuffers));
    const VertexArray& cubeVao = *(vertexArrays.rbegin() + 1);
    const VertexArray& rectangleVao = *vertexArrays.rbegin();

    constexpr glm::vec3 cubeLocation(0.0f, 0.0f, 0.0f);
    const Material cubeMaterial = makeTransparentMaterial({1.0f, 0.5f, 0.31f, 0.2f});

    std::vector<RectangleParameters> rectangles = {
        //{{1.0f, 1.0f, 1.0f}, makeTransparentMaterial({0.0f, 1.0f, 0.0f, 0.5f})},
        {{0.0f, 0.0f, 0.0f}, makeTransparentMaterial({0.0f, 0.0f, 1.0f, 0.5f})},
        {{0.0f, 1.0f, 2.0f}, makeTransparentMaterial({1.0f, 0.0f, 0.0f, 0.5f})}
    };

    std::vector<ExtendedDrawData> opaqueObjects = {
        ExtendedDrawData(&rectangleVao, makeTransparentMaterial({1.0f, 0.0f, 0.0f, 1.0f}),
                                         glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                                         rectangleMesh.GetIndices().size())
    };

    std::vector<ExtendedDrawData> transparentObjects;
    for (const auto& rectangle : rectangles)
    {
        auto drawData = ExtendedDrawData(&rectangleVao, rectangle.material,
                                         translate(glm::mat4(1.0f), rectangle.location),
                                         rectangleMesh.GetIndices().size());
        transparentObjects.push_back(drawData);
    }

    return {std::move(opaqueObjects), std::move(transparentObjects)};
}
}
