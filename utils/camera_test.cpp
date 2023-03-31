#include <gtest/gtest.h>
#include <glm/glm.hpp>

#include "Color.h"
#include "math_util.h"
#include "glm/gtx/string_cast.hpp"

glm::vec3 doPerspectiveProjection(const glm::mat4& perspective, const glm::vec3& v)
{
    const glm::vec4 vProj = perspective * glm::vec4(v, 1.0f);
    return { vProj / vProj.w };
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